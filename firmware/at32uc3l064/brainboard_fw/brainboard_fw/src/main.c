/**
 *  \file main.c
 *
 *  \brief Brainboard setup and main loop.
 *  \author Graham Kelly
 *  \version 1.0
 *  \date August 2014
 *
 *  All Atmel Software Framework libraries used herein are copyright Atmel and 
 *  subject to their appropriate licenses, which allow free redistribution with
 *  some restrictions. These restrictions are listed in their appropriate files.
 *
 *  \page License
 *
 *  Brainboard firmware code is placed under the MIT license
 *  Copyright (c) 2014 Graham Kelly
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 *
 */

#include <asf.h>
#include "brainboard.h"
#include "comms.h"
#include "ads1299.h"
#include "mpu60x0.h"				

#define FRAME_SIZE							128									// Size of each filtered segment (frame)
#define DSP_PREBUFFER_NB_SAMPLES			256									// # samples buffered between ADC and DSP engine
#ifndef FILT_COEFF_LENGTH
#define FILT_COEFF_LENGTH					65									
#endif
#define FILT_OUTPUT_SIZE					(FRAME_SIZE-FILT_COEFF_LENGTH+1)	// Size of filter output vector
#if ((FILT_OUTPUT_SIZE % 4) != 0)
#error Improper filter output buffer size										/* Must ensure FILT_OUTPUT_SIZE is divisible by 4
																				 * to meet Atmel's optimized DSP library specs
																				 */
#endif
	
/* BCI state machine */
/* Ultimately this should have more states for different paradigms */
typedef enum {
	DATA_STREAM = 0,
	MU_THRESHOLD = 1,
	MU_THRESHOLD_GYRO_MOUSE = 2
} processing_t;
processing_t processing_type;

/* Communication flags */
volatile uint32_t frame_complete_flag;

/* Communication counters and buffer management */
volatile uint32_t usart_tx_buffer_idx;
uint32_t spi_buf_write_idx, spi_buf_read_idx;

/* Buffers for data shuttling */
volatile uint8_t data_usart_rx_dma_buffer[3];
volatile uint32_t spi_rx_ads1299_status;
volatile int32_t spi_rx_data_buffer[DSP_PREBUFFER_NB_SAMPLES][MAX_EEG_CHANNELS];

/* Buffers for digital filtering: not currently implemented */
__attribute__((aligned(4))) dsp16_t dsp_filter_input_buffer[FRAME_SIZE];
__attribute__((aligned(4))) dsp16_t dsp_filter_output_buffer[FRAME_SIZE-FILT_COEFF_LENGTH+1];
/* Average value for mu frequency band: not currently implemented */
dsp32_t mu_band_avg;

int debug_usart;
spi_status_t debug_spi;


/* Data packet struct */
bboard_data24bit_packet_t data_usart_tx_packet =
{
	.start		= 0xA55A,
	.packetnum	= 0,
	.eegstatus  = {0, 0, 0},
	// .eegdata will be init with loop in main()
	// .imudata will be init with loop in main()
};

/* Mouse HID report for mouse mode */
mouse_hid_report_t report =
{
	.header		= 0xFD,
	.length		= 5,
	.descriptor = 2,
	.buttons	= 0x00,
	.dx			= 0,
	.dy			= 0,
	.wheel		= 0
};

/* Function prototypes */
void config_gpios(void);
void config_eic(void);
void config_dma(void);
void config_spi(void);
void config_usart(void);
status_code_t config_twim(void);
status_code_t config_rn42(volatile avr32_usart_t*);
status_code_t ads1299_check_device_id(uint8_t);

__attribute__((__interrupt__)) static void eic_handler(void)
{	
	Disable_global_interrupt();
	eic_clear_interrupt_line(&AVR32_EIC, ADS1299_DRDY_INT);
	
	if (processing_type == DATA_STREAM)
	/* If we're in data stream mode, there is no intermediate buffering, so write directly to USART TX packet */
	{
		/* Read back 24-bit status word and 8 channels of 24-bit data */
		ads1299_rdata24_packet(SPI_ADS1299_MAIN_CHIPNUM, usart_tx_buffer_idx, &data_usart_tx_packet);
		if (gpio_get_pin_value(MPU6050_INT_PIN))
		{
			/* Update inertial values Ax, Ay, Az, Gx, Gy, Gz */
			mpu6050_rdata16_packet(MPU6050_DEFAULT_ADDRESS, usart_tx_buffer_idx, &data_usart_tx_packet);
		}
		if (++usart_tx_buffer_idx == DATA_USART_TX_BUFFER_SIZE)
		{
			usart_tx_buffer_idx = 0;
			pdca_enable(DATA_USART_TX_PDCA_CHANNEL);
		}
	}
	else
	/* We have to pre-buffer the data for processing */
	{
		/* Read back 24-bit status word and 8 channels of 24-bit data; store in 32-bit ints */
		ads1299_rdata32_generic(SPI_ADS1299_MAIN_CHIPNUM, spi_buf_write_idx, spi_rx_ads1299_status, spi_rx_data_buffer);			
		if (gpio_get_pin_value(MPU6050_INT_PIN))
		{
			/* Update inertial values Ax, Ay, Az, Gx, Gy, Gz */
			mpu6050_rdata16_packet(MPU6050_DEFAULT_ADDRESS, usart_tx_buffer_idx, &data_usart_tx_packet);
		}			
		spi_buf_write_idx = (spi_buf_write_idx+1) % DSP_PREBUFFER_NB_SAMPLES;			
		if (spi_buf_write_idx % FRAME_SIZE == 0)
		{
			frame_complete_flag = 1;
		}
	}	
}

__attribute__((__interrupt__)) static void pdca_data_usart_tx_handler(void)
{
	pdca_disable(DATA_USART_TX_PDCA_CHANNEL);
	if (processing_type == DATA_STREAM)
	{
		pdca_load_channel(DATA_USART_TX_PDCA_CHANNEL, (void *)&data_usart_tx_packet, sizeof(data_usart_tx_packet));
		/* Circularly increment packet number */
		data_usart_tx_packet.packetnum = (data_usart_tx_packet.packetnum+1) % 128;
	}
	else
	{
		pdca_load_channel(DATA_USART_TX_PDCA_CHANNEL, (void *)&mu_band_avg, sizeof(mu_band_avg));
	}
}

__attribute__((__interrupt__)) static void pdca_data_usart_rx_handler(void)
{
	uint8_t readback;
	
	Disable_global_interrupt();
	
	DATA_USART->cr = AVR32_USART_CR_RTSDIS_MASK;
	
	/* To simplify DMA buffer implementation (i.e. to prevent me from having to code a timeout ISR
	 * when the user sends too few bytes to fill the buffer), multiple register reads/writes are not
	 * currently supported. For the sake of compatibility with the MATLAB command format of the previous
	 * implementation on Arduino, RREG and WREG commands are still 3 bytes long, but the second byte is
	 * ignored.
	 *
	 * !!!! For the following code to work, all commands MUST be 3 bytes long !!!!
	 */
	
	/* Code would go here to parse command switching between data streaming and frequency band extraction
	 * modes. Make sure value of this command is not included in set of ADS1299 commands.
	 */
	
	switch (data_usart_rx_dma_buffer[0] & 0xE0)
	{
		case 0x40:	// WREG
			ads1299_wreg(SPI_ADS1299_MAIN_CHIPNUM, data_usart_rx_dma_buffer[0] & 0x1F, data_usart_rx_dma_buffer[2]);
			break;
		case 0x20:	// RREG
			ads1299_rreg(SPI_ADS1299_MAIN_CHIPNUM, data_usart_rx_dma_buffer[0] & 0x1F, &readback);
			usart_putchar(DATA_USART, (int)readback);
			while(!usart_tx_empty(DATA_USART));
			break;
		default:
			ads1299_send_byte(SPI_ADS1299_MAIN_CHIPNUM, data_usart_rx_dma_buffer[0]);
			break;
	}
	if (data_usart_rx_dma_buffer[0] == ADS1299_OPC_STOP)
	{
		usart_tx_buffer_idx = 0;
		pdca_disable(DATA_USART_TX_PDCA_CHANNEL);
	}
	pdca_load_channel(DATA_USART_RX_PDCA_CHANNEL, (void *)data_usart_rx_dma_buffer, 3);
	DATA_USART->cr = AVR32_USART_CR_RTSEN_MASK;
	
	Enable_global_interrupt();
}

void config_dma(void)
{
	/* DMA configuration options for USART3 (RN-42 Bluetooth) data transmission.
	 * RN-42 is connected to USART3 on the UC3. The USART operates in byte mode, so transfer size must be BYTE.
	 */
	const pdca_channel_options_t pdca_data_usart_tx_opt = 
	{
		.pid			= DATA_USART_TX_PDCA_PID,
		.transfer_size	= PDCA_TRANSFER_SIZE_BYTE,
		.r_addr			= NULL,
		.r_size			= 0
	};
	
	/* DMA configuration options for USART3 (RN-42 Bluetooth) data receipt.
	 * RN-42 is connected to USART3 on the UC3. The USART operates in byte mode, so transfer size must be BYTE.
	 */
	const pdca_channel_options_t pdca_data_usart_rx_opt =
	{
		.pid			= DATA_USART_RX_PDCA_PID,
		.transfer_size	= PDCA_TRANSFER_SIZE_BYTE,
		.addr			= (void *)data_usart_rx_dma_buffer,
		.size			= 3,
		.r_addr			= NULL,
		.r_size			= 0
	};
	
	bool global_irq_enabled = cpu_irq_is_enabled();
	if (global_irq_enabled) {
		/* Turn off interrupts while we set stuff up */
		Disable_global_interrupt();
	}
	
	/* Init PDCA and register corresponding interrupts */
	/* USART3 receipt */
	pdca_init_channel(DATA_USART_RX_PDCA_CHANNEL, &pdca_data_usart_rx_opt);
	INTC_register_interrupt((__int_handler)&pdca_data_usart_rx_handler, AVR32_PDCA_IRQ_0 + DATA_USART_RX_PDCA_CHANNEL, AVR32_INTC_INT3);
	pdca_enable_interrupt_transfer_complete(DATA_USART_RX_PDCA_CHANNEL);
	
	/* USART3 transmission */
	pdca_init_channel(DATA_USART_TX_PDCA_CHANNEL, &pdca_data_usart_tx_opt);
	INTC_register_interrupt((__int_handler)&pdca_data_usart_tx_handler, AVR32_PDCA_IRQ_0 + DATA_USART_TX_PDCA_CHANNEL, AVR32_INTC_INT2);
	pdca_load_channel(DATA_USART_TX_PDCA_CHANNEL, (void *)&data_usart_tx_packet, sizeof(data_usart_tx_packet));
	pdca_enable_interrupt_transfer_complete(DATA_USART_TX_PDCA_CHANNEL);	
	
	if (global_irq_enabled) {
		/* Enable all interrupts */
		Enable_global_interrupt();
	}
}

void config_eic(void)
{

	const eic_options_t eic_drdy_opt =
	{
		.eic_mode    = EIC_MODE_EDGE_TRIGGERED,
		.eic_level   = EIC_EDGE_FALLING_EDGE,
		.eic_filter  = EIC_FILTER_DISABLED,
		.eic_async   = EIC_ASYNCH_MODE,
		.eic_line    = ADS1299_DRDY_INT
	};
	
	bool global_irq_enabled = cpu_irq_is_enabled();
	if (global_irq_enabled) {
		Disable_global_interrupt();
	}
	
	/* Register external DRDY interrupt */
	INTC_register_interrupt((__int_handler)&eic_handler, ADS1299_DRDY_IRQ, AVR32_INTC_INT0);
	eic_init(&AVR32_EIC, &eic_drdy_opt, 1);
	eic_enable_line(&AVR32_EIC, eic_drdy_opt.eic_line);
	eic_enable_interrupt_line(&AVR32_EIC, eic_drdy_opt.eic_line);
	
	if (global_irq_enabled) {
		Enable_global_interrupt();
	}
}

void config_spi(void)
{
	const spi_options_t spi_opt =
	{
		.reg			= SPI_ADS1299_MAIN_CHIPNUM,		// "Master" ADS1299 when daisy-chaining: connected to CS0
		.baudrate		= SPI_BAUDRATE,					// 1 MHz preferred baudrate
		.bits			= 8,							// 8 bits per character (range is 8 to 16)
		.spck_delay		= 0,							// Delay before first clock pulse after selecting slave (in PBA clock periods)
		.trans_delay	= 0,							// Delay between each transfer/character (in PBA clock periods)
		.stay_act		= true,							// Sets this chip to stay active after last transfer to it
		.spi_mode		= SPI_MODE_1,					// Which SPI mode to use (ADS1299 is mode 1)
		.modfdis		= true							// Disable the mode fault detection
	};
	
	/* Init SPI module as master */
	spi_initMaster(SPI_ADDRESS, &spi_opt);
	
	/* Setup configuration for chip connected to CS0 */
	debug_spi = spi_setupChipReg(SPI_ADDRESS, &spi_opt, sysclk_get_pba_hz());
	
	/* Allow the module to transfer data */
	spi_enable(SPI_ADDRESS);
}

void config_gpios(void)
{						
	/* Debug/timing pin for oscilloscope */
	//gpio_enable_gpio_pin(AVR32_PIN_PA11);
	
	#if (BRAINBOARD_REV > 0)
	/* Config GPIOs for I2C/TWI comms with inertial measurement unit */
	/* First we apparently have to assign the default TWIM pins as something so that we can
	 * use different ones. Stupid. Tore my hair out before I got this one...
	 */
	gpio_enable_gpio_pin(AVR32_PIN_PB05);												// Default TWIMS0 TWCK
	gpio_enable_gpio_pin(AVR32_PIN_PA21);												// Default TWIMS0 TWD
	gpio_enable_module_pin(AVR32_TWIMS0_TWCK_0_0_PIN, AVR32_TWIMS0_TWCK_0_0_FUNCTION);	// PA04
	gpio_enable_module_pin(AVR32_TWIMS0_TWD_0_2_PIN, AVR32_TWIMS0_TWD_0_2_FUNCTION);	// PA05
	
	/* MPU6050 interrupt pin: polled at each ADS1299 DRDY */
	gpio_enable_gpio_pin(MPU6050_INT_PIN);												// PA10
	gpio_configure_pin(MPU6050_INT_PIN, GPIO_DIR_INPUT);
	#endif
	
	/* Config GPIOs for USART2 (wired data) */
	gpio_enable_module_pin(AVR32_USART2_TXD_0_1_PIN, AVR32_USART2_TXD_0_1_FUNCTION);	// PA06
	gpio_enable_pin_pull_up(AVR32_USART2_TXD_0_1_PIN);
	gpio_enable_module_pin(AVR32_USART2_RXD_0_1_PIN, AVR32_USART2_RXD_0_1_FUNCTION);	// PA07

	/* Config GPIOs for USART3 (RN-42 Bluetooth data) */
	gpio_enable_module_pin(AVR32_USART3_TXD_0_1_PIN, AVR32_USART3_TXD_0_1_FUNCTION);	// PB06
	gpio_enable_pin_pull_up(AVR32_USART3_TXD_0_1_PIN);
	gpio_enable_module_pin(AVR32_USART3_RXD_0_1_PIN, AVR32_USART3_RXD_0_1_FUNCTION);	// PB07
	gpio_enable_module_pin(AVR32_USART3_RTS_0_1_PIN, AVR32_USART3_RTS_0_1_FUNCTION);	// PB08
	gpio_enable_module_pin(AVR32_USART3_CTS_0_1_PIN, AVR32_USART3_CTS_0_1_FUNCTION);	// PB09
	
	/* Config GPIOs for SPI */
	gpio_enable_module_pin(AVR32_SPI_MISO_0_1_PIN, AVR32_SPI_MISO_0_1_FUNCTION);		// PB02
	gpio_enable_module_pin(AVR32_SPI_MOSI_0_1_PIN, AVR32_SPI_MOSI_0_1_FUNCTION);		// PB03
	gpio_enable_module_pin(AVR32_SPI_SCK_0_1_PIN, AVR32_SPI_SCK_0_1_FUNCTION);			// PB01
	gpio_enable_module_pin(AVR32_SPI_NPCS_0_1_PIN, AVR32_SPI_NPCS_0_1_FUNCTION);		// PB00
	
	/* RN-42 hardware reset pin */
	gpio_enable_gpio_pin(BT_RESET_PIN);													// PB11
	gpio_configure_pin(BT_RESET_PIN, GPIO_DIR_OUTPUT);
	
	/* ADS1299 control pins */
	#if (BRAINBOARD_REV == 0)
	/* BrainBoard R1 doesn't connect START and RESET pins, uses software commands instead */
	gpio_enable_gpio_pin(ADS1299_PIN_START);											// PA17	
	gpio_configure_pin(ADS1299_PIN_START, GPIO_DIR_OUTPUT);
	gpio_enable_gpio_pin(ADS1299_PIN_RESET);											// PB04
	gpio_configure_pin(ADS1299_PIN_RESET, GPIO_DIR_OUTPUT);
	#endif
	gpio_enable_gpio_pin(ADS1299_PIN_PWDN);												// PA09
	gpio_configure_pin(ADS1299_PIN_PWDN, GPIO_DIR_OUTPUT);
	/* ADS1299 DRDY interrupt pin */	
	gpio_enable_module_pin(ADS1299_PIN_DRDY, ADS1299_DRDY_INT_FN);						// PA18
}

status_code_t config_twim(void)
{	
	twim_options_t twim_opt =
	{
		.speed	= I2C_MASTER_SPEED_HZ,
		.chip	= MPU6050_DEFAULT_ADDRESS,
		.pba_hz = sysclk_get_pba_hz(),
		.smbus	= false 
	};
	
	sysclk_enable_peripheral_clock(I2C_MODULE);			
	return twim_master_init(I2C_MODULE, (const twim_options_t *)&twim_opt);
}

void config_usart(void)
{	

// 	const usart_options_t usartuc_opt =
// 	{
// 		.baudrate    = USARTUC_BAUDRATE,
// 		.channelmode = USART_NORMAL_CHMODE,
// 		.charlength  = 8,
// 		.paritytype  = USART_NO_PARITY,
// 		.stopbits    = USART_1_STOPBIT
// 	};
	
	usart_options_t usartbt_opt =
	{
		.baudrate    = USARTBT_DEFAULT_BAUDRATE,
		.channelmode = USART_NORMAL_CHMODE,
		.charlength  = 8,
		.paritytype  = USART_NO_PARITY,
		.stopbits    = USART_1_STOPBIT
		
	};
	
	/* Configure and enable USART module for inter-processor comms */
// 	sysclk_enable_peripheral_clock(USARTUC_MODULE);
// 	usart_init_rs232(USARTUC_MODULE, &usartuc_opt, sysclk_get_pba_hz());
	
	/* Enable USART for Bluetooth module comms */
	sysclk_enable_peripheral_clock(USARTBT_MODULE);
	debug_usart = usart_init_hw_handshaking(USARTBT_MODULE, &usartbt_opt, sysclk_get_pba_hz());
}

status_code_t config_rn42(volatile avr32_usart_t* usart)
{
	
	/* Reset-cycle RN-42 Bluetooth module */
	gpio_clr_gpio_pin(BT_RESET_PIN);
	delay_us(100);
	gpio_set_gpio_pin(BT_RESET_PIN);
	delay_ms(100);
	
	/* Configure RN-42 */
	/* Enter command mode */
	usart_write_line(usart, "$$$");
	while(!usart_tx_ready(usart));
	delay_ms(15);
	
	/* Reduce window for accepting device discovery requests (inquiry) to 80 ms. Default
	 * window is 160 ms (0x0100).
	 * The 16-bit hex value is multiplied by 0.625 ms to determine the interval. The minimum is
	 * 0x0012 (11.25 ms).
	 */
	usart_write_line(usart, "SI,0080\r\n");
	while(!usart_tx_ready(usart));
	delay_ms(50);

	/* Enable SNIFF mode with interval of 25 ms.
	 * In this mode, the RN-42 wakes up every [interval] to send/receive data. Data to transmit
	 * is buffered by the RN-42, so it shouldn't be lost (depending on buffer size). This saves
	 * a good amount of power.
	 * The 16-bit hex value is multiplied by 0.625 ms to determine the interval. Maximum is
	 * 0x7FFF, or about 20 s.
	 */
	usart_write_line(usart, "SW,0028\r\n");
	while(!usart_tx_ready(usart));
	delay_ms(50);
	
	/* Set all GPIO pins on RN-42 as inputs, reducing power.
	 *
	 * 16-bit hex value; first byte is mask to specify the GPIO number, second is command
	 * value, where 0 = input, 1 = output:
	 *
	 * PARAMETER[15:0] = MASK[7..0]<<8 | VALUE[7..0]
	 * e.g. to set GPIO5 (bit 5 of MASK) as an output, write 0b0010000000100000 = 0x2020
	 *      to set it as an input,                     write 0b0010000000000000 = 0x2000
	 *
	 * Sending 0x1000 sets ALL GPIOs to inputs (since GPIO4 is used for factory reset and
	 * isn't user-controllable).
	 * 
	 * Here the STATUS and CONN LEDs are connected to GPIO5 and GPIO2, respectively.
	 * We may want to disable them to conserve power.
	 *
	 * Quick reference: 0x2424 sets GPIO5 and GPIO2 to outputs. 0x2400 sets them to inputs.
	 *
	 * NOTE: the S% command is stored on the flash, and GPIOs retain these settings on
	 * power cycle or reset. To make the change temporary, use S@ with the same argument.
	 * This change will be lost on a reboot, so it will need to be moved to after the next
	 * next command.
	 */
	usart_write_line(usart, "S%,2424\r\n");
	while(!usart_tx_ready(usart));
	delay_ms(50);

 	/* Reboot RN-42 */
	usart_write_line(usart, "R,1\r\n");
	while(!usart_tx_ready(usart));
	delay_ms(15);
			
	return STATUS_OK;
}

status_code_t ads1299_check_device_id(uint8_t chipselect)
{
	uint8_t id;
		
	ads1299_rreg(chipselect, ADS1299_REGADDR_ID, &id);
	/* ADS1299 should return ID of 0bXXXX1110 */
	if ( (id & 0xF) != ADS1299_DEVICE_ID ) {
		return ERR_IO_ERROR;
	}
	else {
		return STATUS_OK;
	}
}

int main(void)
{	
	volatile uint32_t k, n, m;

	usart_tx_buffer_idx = 0;
	spi_buf_write_idx = 0;
	spi_buf_read_idx = 0;
	mu_band_avg = 0;
	
	frame_complete_flag = 0;
	
	/* Initialize in data streaming mode; host computer may change later */
	processing_type = DATA_STREAM;
	
	/* Set up basic functions: clocks, GPIOs, sleep settings */
	sysclk_init();
	sleepmgr_init();
	config_gpios();
	
	/* Initialize packet data arrays */
	for (m = 0; m < DATA_USART_TX_BUFFER_SIZE; m++) {
		for (n = 0; n < MAX_EEG_CHANNELS; n++) {
			data_usart_tx_packet.eegdata[n][m][0] = 0;
			data_usart_tx_packet.eegdata[n][m][1] = 0;
			data_usart_tx_packet.eegdata[n][m][2] = 0;
		}
		for (n = 0; n < MAX_IMU_DOF; n++) {
			data_usart_tx_packet.imudata[n][m] = 0;
		}
	}
	
	/* Initialize SPI input buffer */
	for (m = 0; m < DSP_PREBUFFER_NB_SAMPLES; m++) {
		for (n = 0; n < MAX_EEG_CHANNELS; n++) {
			spi_rx_data_buffer[m][n] = 0;
		}
	}
	
	/* Set up peripheral modules other than TWI, which needs INTC first */
	config_usart();
	config_rn42(USARTBT_MODULE);	
	config_spi();
	ads1299_device_init(SPI_ADS1299_MAIN_CHIPNUM, 0);	
		
	/* Verify ADS1299 device ID */
	//usart_write_line(AUX_USART, "Checking ADS1299 device ID...\r\n");
	if (ads1299_check_device_id(SPI_ADS1299_MAIN_CHIPNUM) == STATUS_OK) {
		//usart_write_line(AUX_USART, "Device ID verified.\r\n");
	}
	else {
		usart_write_line(AUX_USART, "Invalid ID. Possible SPI error.\r\n");
		/* Don't continue in error state; go to DEEPSTOP */
		AVR32_INTC.ipr[0];  // Dummy read in case any PB write operations are incomplete
		SLEEP(AVR32_PM_SMODE_DEEPSTOP);
	}
	
	
	/* Configure global interrupt controller */
	Disable_global_interrupt();
	INTC_init_interrupts();
	
	#if (BRAINBOARD_REV > 0)
	/* Init TWI */
	config_twim();
	
	/* Initialize the MPU6050 IMU */
	//usart_write_line(AUX_USART, "Initializing IMU...\r\n");
	if (mpu6050_initialize_normal(MPU6050_DEFAULT_ADDRESS) == STATUS_OK) {
		//usart_write_line(AUX_USART, "IMU successfully initialized.\r\n");
	}
	else {
		usart_write_line(AUX_USART, "IMU failed to initialize.\r\n");
		/* Don't continue in error state; go to DEEPSTOP */
		AVR32_INTC.ipr[0];  // Dummy read in case any PB write operations are incomplete
		SLEEP(AVR32_PM_SMODE_DEEPSTOP);
	}
	#endif
	
	/* Configure DRDY interrupt */
	config_eic();
	
	/* Set up DMA for SPI/USART communications */
	config_dma();
	
	Enable_global_interrupt();
	
	/* Enable receipt of USART commands */
	pdca_enable(DATA_USART_RX_PDCA_CHANNEL);
	
	/* Begin A/D conversion on the ADS1299 */
	/* Comment out to allow host computer to initiate data collection */
// 	ads1299_send_byte(SPI_ADS1299_MAIN_CHIPNUM, ADS1299_OPC_RDATAC);			
// 	ads1299_soft_start_conversion(SPI_ADS1299_MAIN_CHIPNUM);
	
	while(1)
	{
		if (frame_complete_flag) 
		{
			Disable_global_interrupt();
			/* DSP stuff on current frame */
			for (n = 0; n < MAX_EEG_CHANNELS; n++)
			{
				k = spi_buf_read_idx;
				for (m = 0; m < FRAME_SIZE; m++)
				{
					/* Casting to 16-bit signed data type (dsp16_t) will narrow the dynamic
					 * range to +/- (2^15-1)*LSB, or +/- 17.58 mV.
					 * It's possible we could increase the dynamic range by eliminating 
					 * some of the lower bits through right-shifting before the cast, but 
					 * this should be sufficient for well-conditioned EEG electrodes.
					 */
					
					dsp_filter_input_buffer[m] = (dsp16_t) (spi_rx_data_buffer[k][n]);
					k = (k+1) % DSP_PREBUFFER_NB_SAMPLES;
				}
				
				/* Execute filter and/or other operations */

			}
			spi_buf_read_idx = k;
			
		}
		else 
		{
			/* Go to IDLE and wait for DRDY interrupt */
			AVR32_INTC.ipr[0];  // Dummy read in case any PB write operations are incomplete
			SLEEP(AVR32_PM_SMODE_IDLE);
		}
	}
}
