/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
//#include <avr32/io.h>
#include <asf.h>
#include <stdio.h>
#include "eic.h"
#include "intc.h"
#include "ads1299.h"

/*
	// tPOR = power-on-reset time for ADS1299 = at least 65536/(ADS1299 FCLOCK)
	// 										  = 65536/2048000
	//										  = 32 ms
	// UC3L clock = 30 MHz
	// UC3L tcy   = 1/30000000 = 33 ns
	// so T_POR    = 32e-3/33e-9 = at least 960000
	// Datasheet recommends 1s

	#define ADS1299_T_POR			1000000UL

	// tRST = reset low width for ADS1299 = at least 2/(ADS1299 FCLOCK)
	//									  = 976.5625 ns
	// so T_RSTPULSE = 976.5625e-9/33e-9 = at least 30

	#define ADS1299_T_RSTPULSE		30

	// Need 18tclk after reset to initialize

	#define ADS1299_T_RST			267

// SPI must wait 4*t_clk of the ADS1299 after each command
// Since ADS1299 f_clk is 2.048 MHz, 4*t_clk is 1.96 us
// 1.96e-6/33e-9 = 60

#define ADS1299_T_DEL			60
*/

// Use if using SPI clock faster than 4 MHz (for ADS1299 clock 2.048 MHz)
//#define SPI_FAST

#define SPI_ADS1299_CHIPNUM		0
//#define USE_TERMINAL
#define BCI2VR

uint8_t ads1299_check_device_id(uint8_t);
void ads1299_host_init(void);
void configure_interrupts(void);
void spi_init_module(void);

volatile uint8_t read_request_flag, status_request_flag;
volatile uint16_t payload_size;

volatile uint8_t data_ready;
volatile uint8_t device_id;
//volatile int debug_val;

volatile uint32_t status;
volatile uint32_t sample_idx;

static const uint8_t dummy = 0;

volatile signal_t channel_array[MAX_CHANNELS][SIGNAL_BUFFER_SIZE];

#ifdef USE_TERMINAL
char output_string[10];
#endif

static const eic_options_t eic_opt =
{
	.eic_mode    = EIC_MODE_LEVEL_TRIGGERED,
	.eic_level   = EIC_LEVEL_LOW_LEVEL,
	.eic_filter  = EIC_FILTER_DISABLED,
	.eic_async   = EIC_ASYNCH_MODE,
	.eic_line    = ADS1299_DRDY_INT
};

static const pdca_channel_options_t pdca_usarttx_opt = 
{
	.pid			= AVR32_PDCA_PID_USART2_TX,
	.transfer_size	= PDCA_TRANSFER_SIZE_WORD,
	.addr			= (void *) tx_packet,
	.size			= sizeof  (tx_packet),
	.r_addr			= (void *) tx_packet,
	.r_size			= sizeof  (tx_packet)	
};


/**
 *  \brief DMA configuration options for SPI received data.
 *
 * Each transfer is one byte long, meaning that the pointer will increment by 1 on each transfer. The variables
 * status and rx_buffer are a uint32 and an 8-element int32 array, respectively. After the TCRR and MARR are
 * zeroed, the software should reload them identically to the initial values, but increment rx_buffer by 8, or
 * the applicable channel count.
 */
// static const pdca_channel_options_t pdca_spirx_opt =
// {
// 	.pid			= AVR32_PDCA_PID_SPI0_RX,
// 	.transfer_size	= PDCA_TRANSFER_SIZE_BYTE,
// 	.addr			= (void *)&status,
// 	.size			= 3,
// 	.r_addr			= (void *)rx_buffer,
// 	.r_size			= 24
// };
// 
// static const pdca_channel_options_t pdca_spitx_opt =
// {
// 	.pid			= AVR32_PDCA_PID_SPI0_TX,
// 	.transfer_size	= PDCA_TRANSFER_SIZE_BYTE,
// 	.addr			= (void *)&dummy,
// 	.size			= 1,
// 	.r_addr			= (void *)&dummy,
// 	.r_size			= 1
// };

static const spi_options_t spi_opt =
{
	SPI_ADS1299_CHIPNUM,							// The SPI channel to set up: Memory is connected to CS1.
	1000000,										// Preferred baudrate for the SPI.
	8,												// Number of bits in each character (8 to 16).
	0,												// Delay before first clock pulse after selecting slave (in PBA clock periods).
	0,												// Delay between each transfer/character (in PBA clock periods).
	1,												// Sets this chip to stay active after last transfer to it.
	SPI_MODE_1,										// Which SPI mode to use when transmitting.
	1												// Disable the mode fault detection.
};

static const usart_options_t usart_opt = 
{
	.baudrate    = USART_BAUDRATE,
	.channelmode = USART_NORMAL_CHMODE,
	.charlength  = 8,
	.paritytype  = USART_NO_PARITY,
	.stopbits    = USART_1_STOPBIT,
};

__attribute__((__interrupt__)) static void eic_irq_fn(void)
{
	/* Circularly increment sample index */
	/* Read back 24-bit status word and 8 channels of 24-bit data */
	ads1299_rdata32(SPI_ADS1299_CHIPNUM, &status, sample_idx, channel_array);
	#ifdef BUFFER_SIZE_NOT_POWER_2
	sample_idx = (sample_idx + 1) % SIGNAL_BUFFER_SIZE;
	#else
	sample_idx = (sample_idx + 1) & (SIGNAL_BUFFER_SIZE - 1);
	#endif
	
	read_request_flag = 2;															// DELETE THIS LATER MAYBE
	
	eic_clear_interrupt_line(&AVR32_EIC, ADS1299_DRDY_INT);
}

// __attribute__((__interrupt__)) static void pdca_irq_fn(void)
// {
// 	
// }

// __attribute__((__interrupt__)) static void usart_irq_fn(void)
// {
// 	int c;
// 	usart_read_char(USART_MODULE, &c);
// 	
// 	if ( (read_request_flag == 0) && (c == 0x72) )	/* 'r' */
// 	{
// 		read_request_flag = 1;
// 		//sleepmgr_enter_sleep();
// 		
// 	}
// 	else if (read_request_flag == 1)
// 	{
// 		if (c > SIGNAL_BUFFER_SIZE) {
// 			payload_size = SIGNAL_BUFFER_SIZE;
// 		}
// 		else {
// 			payload_size = c;
// 		}
// 		read_request_flag = 2;
// 	}
// 	
// }

int main(void)
{
	uint8_t read_idx;
	int n, m;
	uint8_t packet_num, data_out;
	volatile signal_t checksum, sample;
	
	read_request_flag = 0;
	status_request_flag = 0;
	
// 	#ifdef USE_32_BIT_DATA
// 	uint8_t use_32_bit_data = 1;
// 	#else
// 	uint8_t use_32_bit_data = 0;
// 	#endif
	
// 	const uint32_t channel_mask = SIGNAL_BUFFER_SIZE - 1;
// 	const uint32_t buffer_mask  = (MAX_CHANNELS * SIGNAL_BUFFER_SIZE) - 1;
	
	board_init();
	sysclk_init();
	sleepmgr_init();
	
	/* Initialize arrays */
	for (m = 0; m < SIGNAL_BUFFER_SIZE; m++)
	{
		for (n = 0; n < MAX_CHANNELS; n++)
		{
			channel_array[n][m] = 0;
		}
	}
	
	/* Configure and enable USART module */
	sysclk_enable_peripheral_clock(USART_MODULE);
	usart_init_rs232(USART_MODULE, &usart_opt, sysclk_get_peripheral_bus_hz(USART_MODULE));
	#ifdef USE_TERMINAL
	usart_write_line(USART_MODULE, "USART initialized.\r\n");
	#endif
	
	/* Configure and enable SPI master module */
	spi_init_module();
	#ifdef USE_TERMINAL
	usart_write_line(USART_MODULE, "SPI initialized. Checking ADS1299 device ID...\r\n");
	#endif
	
	/* Debugging pin for checking timing
	 * This is the pin marked 'ADC6' on UC3-L0 Xplained
	 */
	gpio_enable_gpio_pin(AVR32_PIN_PB06);
	gpio_configure_pin(AVR32_PIN_PB06, GPIO_DIR_OUTPUT);
	gpio_clr_gpio_pin(AVR32_PIN_PB06);

	/* Initialize the ADS1299 */
	//ads1299_host_init();
	ads1299_device_init(SPI_ADS1299_CHIPNUM);
	
	/* Allow MCU to sleep no more deeply than IDLE mode */
	sleepmgr_lock_mode(SLEEPMGR_IDLE);
	
	/* Verify ADS1299 device ID */
	#ifdef USE_TERMINAL
	device_id = ads1299_check_device_id(SPI_ADS1299_CHIPNUM);
	if (device_id == ADS1299_DEVICE_ID)
	{
		usart_write_line(USART_MODULE, "Device ID verified. Beginning main loop...\r\n");
		usart_write_line(USART_MODULE, "Press 'r' at any time to display data.\r\n");
	}
	else
	{
		usart_write_line(USART_MODULE, "Invalid device ID. Check SPI connections.\r\n");
		while(1);
	}
	#endif

	/* Write to CH8 settings register, set as normal input, gain 24 */
	ads1299_wreg(SPI_ADS1299_CHIPNUM, ADS1299_REGADDR_CH5SET, 0b01100000);
	/* Write to CONFIG1, set data rate to 250 Hz */
	ads1299_wreg(SPI_ADS1299_CHIPNUM, ADS1299_REGADDR_CONFIG1, 0b10010110);
// 	/* Write to MISC1 register, SRB1 on (ref electrode) */
// 	ads1299_wreg(SPI_ADS1299_CHIPNUM, ADS1299_REGADDR_MISC1, 0b00100000);
	
	//pdca_init_channel(1, &pdca_usarttx_opt);
	
	/* Initialize interrupt controller and register USART RX and EIC interrupts */
	configure_interrupts();
	
	packet_num = 0;
	checksum = 0;
	
	/* Begin A/D conversion on the ADS1299, then go to IDLE */			
	ads1299_hard_start_conversion();
	//sleepmgr_enter_sleep();
	
	while(1)
	{				
		if (read_request_flag == 2)
		{
			/* Somebody's asking for [payload_size] samples, so spit 'em out!
			 * Samples will be sent in the format [CH1@t1][CH2@t1][...][CH8@t1]...
			 *                                    [CH1@t2][CH2@t2][...][CH8@t2]...
			 *                                        .       .     .      .
			 *                                    [CH1@tn][CH2@tn][...][CH8@tn]
			 * Where n is payload_size.
			 * 
			 * When calculating the checksum, we extract the lower byte of each data point and cumulatively XOR
			 * them over each channel.
			 *
			 * Default payload size is 32 samples/channel; default MAX_CHANNELS is 6; default precision is 16-bit
			 *
			 * For binary-format packets (not using terminal):
			 * Total header size is 'H'+packet_num+(3*status_word)               = 5 bytes
			 * Total data size for 16-bit precision is (2*payload_size*MAX_CHANNELS) bytes
			 *				   for 24-bit precision is (3*payload_size*MAX_CHANNELS) bytes
			 * Total trailer size is 'FF'+checksum								 = 3 bytes
			 * ---------------------------------------------------------------------------
			 *																
			 */
			Disable_global_interrupt();
			read_idx = sample_idx;
			
			#ifdef BCI2VR
			
			/* Send sync bytes per modEEG protocol v2 */
			usart_write_char(USART_MODULE, 0xA5);
			while(!usart_tx_ready(USART_MODULE));
			usart_write_char(USART_MODULE, 0x5A);
			while(!usart_tx_ready(USART_MODULE));
			
			/* Send version number */
			usart_write_char(USART_MODULE, 2);
			while(!usart_tx_ready(USART_MODULE));
			
			/* Send packet number (0-255) */
			usart_write_char(USART_MODULE, packet_num);
			while(!usart_tx_ready(USART_MODULE));
			packet_num = (packet_num == 255)? 0: (packet_num+1);
			
			#else
						
			/* Send 24-bit status word */
			#ifdef USE_TERMINAL
			sprintf(output_string, "%6X", (unsigned int)status);
			usart_write_line(USART_MODULE, output_string);
			while(!usart_tx_ready(USART_MODULE));
			#else
			usart_write_char(USART_MODULE, (int)((status>>16) & 0xFF));
			while(!usart_tx_ready(USART_MODULE));
			usart_write_char(USART_MODULE, (int)((status>>8) & 0xFF));
			while(!usart_tx_ready(USART_MODULE));
			usart_write_char(USART_MODULE, (int)(status & 0xFF));
			while(!usart_tx_ready(USART_MODULE));
			#endif /* #ifdef USE_TERMINAL */
			
			#endif /* #ifdef BCI2VR */
			
			for (m = 0; m < MAX_PAYLOAD_SIZE; m++)
			{				
				for (n = 0; n < MAX_CHANNELS; n++)
				{
					sample = channel_array[n][read_idx];
					/* Checksum(n) = Checksum(n-1) XOR lower byte of sample */
					/* Checksum initialized to 0 */
					checksum ^= (sample & 0xFF);
					
					#ifdef USE_TERMINAL				
					sprintf(output_string, "%7i", sample);
					usart_write_line(USART_MODULE, output_string);
					while(!usart_tx_ready(USART_MODULE));
					usart_write_char(USART_MODULE, 0x20);
					while(!usart_tx_ready(USART_MODULE));
					#else
										
					#ifdef USE_32_BIT_DATA
					data_out = (int)((sample>>16) & 0xFF);
					usart_write_char(USART_MODULE, data_out);
					while(!usart_tx_ready(USART_MODULE));
					#endif /* #ifdef USE_32_BIT_DATA */
					
					data_out = (int)((sample>>8) & 0xFF);
					usart_write_char(USART_MODULE, data_out);
					while(!usart_tx_ready(USART_MODULE));
					
					data_out = (int)(sample & 0xFF);
					usart_write_char(USART_MODULE, data_out);
					while(!usart_tx_ready(USART_MODULE));
					
					#endif /* #ifdef USE_TERMINAL */
				}
								
				#ifdef USE_TERMINAL
				usart_write_line(USART_MODULE, "\r\n");
				while(!usart_tx_ready(USART_MODULE));
				#endif
				
				#ifdef BUFFER_SIZE_NOT_POWER_2
				read_idx = (read_idx + 1) % SIGNAL_BUFFER_SIZE;
				#else
				read_idx = (read_idx + 1) & (SIGNAL_BUFFER_SIZE - 1);
				#endif
				
			}
			
			#ifdef BCI2VR
			
			/* Send dummy byte ("switches" from modEEG protocol) */
			usart_write_char(USART_MODULE, 0);
			while(!usart_tx_ready(USART_MODULE));
			
			#else
			
			/* Send checksum */
			#ifdef USE_TERMINAL
			sprintf(output_string, "\r\n%6i\r\n", checksum);
			usart_write_line(USART_MODULE, output_string);
			while(!usart_tx_ready(USART_MODULE));
			#else
			usart_write_char(USART_MODULE, checksum);
			while(!usart_tx_ready(USART_MODULE));
			#endif /* #ifdef USE_TERMINAL */
			
			/* Send trailer bytes ('FF') */
			usart_write_char(USART_MODULE, 'F');
			while(!usart_tx_ready(USART_MODULE));
			usart_write_char(USART_MODULE, 'F');
			while(!usart_tx_ready(USART_MODULE));
			
			#endif /* #ifdef BCI2VR */
			
			/* Reset checksum and flags, re-enable interrupts, and IDLE */
			checksum = 0;		
			read_request_flag = 0;
			Enable_global_interrupt();
			//sleepmgr_enter_sleep();
			
		}	
	}
}

void ads1299_host_init(void)
{
	/* Nothing here right now */
}

void spi_init_module(void)
{
	/* Init SPI pins */
	gpio_enable_module_pin(AVR32_SPI_MISO_0_1_PIN, AVR32_SPI_MISO_0_1_FUNCTION);	// PB02
	gpio_enable_module_pin(AVR32_SPI_MOSI_0_1_PIN, AVR32_SPI_MOSI_0_1_FUNCTION);	// PB03
	gpio_enable_module_pin(AVR32_SPI_SCK_0_1_PIN, AVR32_SPI_SCK_0_1_FUNCTION);		// PB01
	gpio_enable_module_pin(AVR32_SPI_NPCS_0_1_PIN, AVR32_SPI_NPCS_0_1_FUNCTION);	// PB00
	/* Init SPI module as master */
	spi_initMaster(SPI_ADDRESS, &spi_opt);
	/* Setup configuration for chip connected to CS1 */
	spi_setupChipReg(SPI_ADDRESS, &spi_opt, sysclk_get_pba_hz());
	/* Allow the module to transfer data */
	spi_enable(SPI_ADDRESS);
}

void configure_interrupts(void)
{
	Disable_global_interrupt();
	
	/* Configure global interrupt controller */
	INTC_init_interrupts();
	
	/* Register interrupts */
	
// 	INTC_register_interrupt(&usart_irq_fn, AVR32_USART2_IRQ, AVR32_INTC_INT2);
// 	USART_MODULE->ier = AVR32_USART_IER_RXRDY_MASK;
	
	INTC_register_interrupt(&eic_irq_fn, ADS1299_DRDY_IRQ, AVR32_INTC_INT3);
	eic_init(&AVR32_EIC, &eic_opt, 1);
	eic_enable_line(&AVR32_EIC, ADS1299_DRDY_INT);
	eic_enable_interrupt_line(&AVR32_EIC, ADS1299_DRDY_INT);
	
	/* Enable all interrupts */
	Enable_global_interrupt();
}

uint8_t ads1299_check_device_id(uint8_t chipselect)
{
	uint8_t device_id;
	
	ads1299_rreg(chipselect, ADS1299_REGADDR_ID, &device_id);
	return (device_id & 0xF);
}
