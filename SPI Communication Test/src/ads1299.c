/** \file ads1299.c
 *  \brief Some useful functions for using the ADS1299 EEG analog front-end from Texas Instruments.
 *	\details The ADS1299 is an SPI slave device. Communication with it requires at least one SPI master module on the host.
 *	\author Graham Kelly
 *	\version 0.1
 *	\date June 2013
 *	\copyright
 */

#include "ads1299.h"
//#include "app_defines.h"		// Application-specific defines: comms, etc.

/* Modifiable register values, initialized to defaults */
ads1299_registers_t ADS1299_REGISTERS = 
{
	.ID.VALUE			= ADS1299_REGDEFAULT_ID,
	.CONFIG1.VALUE		= ADS1299_REGDEFAULT_CONFIG1,
	.CONFIG2.VALUE		= ADS1299_REGDEFAULT_CONFIG2,
	.CONFIG3.VALUE		= ADS1299_REGDEFAULT_CONFIG3,
	.LOFF.VALUE			= ADS1299_REGDEFAULT_LOFF,
	.CH1SET.VALUE		= ADS1299_REGDEFAULT_CHNSET,
	.CH2SET.VALUE		= ADS1299_REGDEFAULT_CHNSET,
	.CH3SET.VALUE		= ADS1299_REGDEFAULT_CHNSET,
	.CH4SET.VALUE		= ADS1299_REGDEFAULT_CHNSET,
	.CH5SET.VALUE		= ADS1299_REGDEFAULT_CHNSET,
	.CH6SET.VALUE		= ADS1299_REGDEFAULT_CHNSET,
	.CH7SET.VALUE		= ADS1299_REGDEFAULT_CHNSET,
	.CH8SET.VALUE		= ADS1299_REGDEFAULT_CHNSET,
	.BIAS_SENSP.VALUE	= ADS1299_REGDEFAULT_BIAS_SENSP,
	.BIAS_SENSN.VALUE	= ADS1299_REGDEFAULT_BIAS_SENSN,
	.LOFF_SENSP.VALUE	= ADS1299_REGDEFAULT_LOFF_SENSP,
	.LOFF_SENSN.VALUE	= ADS1299_REGDEFAULT_LOFF_SENSN,
	.LOFF_FLIP.VALUE	= ADS1299_REGDEFAULT_LOFF_FLIP,
	.LOFF_STATP.VALUE	= ADS1299_REGDEFAULT_LOFF_STATP,
	.LOFF_STATN.VALUE	= ADS1299_REGDEFAULT_LOFF_STATN,
	.GPIO.VALUE			= ADS1299_REGDEFAULT_GPIO,
	.MISC1.VALUE		= ADS1299_REGDEFAULT_MISC1,
	.MISC2.VALUE		= ADS1299_REGDEFAULT_MISC2,
	.CONFIG4.VALUE		= ADS1299_REGDEFAULT_CONFIG4
};

/**************************************************************************************************************************************************
*               Function Definitions                                                                                                              *
**************************************************************************************************************************************************/

/* SYSTEM CONTROL FUNCTIONS **********************************************************************************************************************/

void ads1299_packet_init(packet_t* packet, uint16_t* tx_buffer)
{
	packet->sync0 = 165;
	packet->sync1 = 90;
	packet->version = 2;
	packet->count = 0;
	packet->data = tx_buffer;
	packet->switches = 0;
}

ads1299_error_t	ads1299_device_init(uint8_t chip_select)
{
	#if UC3
	/* Enable all relevant pins */
	gpio_enable_gpio_pin(ADS1299_PIN_START);
	//gpio_enable_gpio_pin(ADS1299_PIN_DRDY);
	gpio_enable_gpio_pin(ADS1299_PIN_RESET);
	//gpio_enable_gpio_pin(ADS1299_PIN_PWDN);
	
	/* Configure I/O as appropriate */
	gpio_configure_pin(ADS1299_PIN_START, GPIO_DIR_OUTPUT);
	gpio_configure_pin(ADS1299_PIN_RESET, GPIO_DIR_OUTPUT);
	//gpio_configure_pin(ADS1299_PIN_PWDN, GPIO_DIR_OUTPUT);
	
	/* Pull START pin low to prevent spurious conversions while we initialize */
	gpio_clr_gpio_pin(ADS1299_PIN_START);
	
	/* Enable internal pull-ups for active-low output pins */
	gpio_enable_pin_pull_up(ADS1299_PIN_RESET);
	//gpio_enable_pin_pull_up(ADS1299_PIN_PWDN);
	
	/* Configure external interrupt pin */
	gpio_enable_module_pin(ADS1299_PIN_DRDY, ADS1299_DRDY_INT_FN);
	gpio_enable_pin_pull_up(ADS1299_PIN_DRDY);
	
	/* Hold RESET high for 200ms to allow oscillator and reference warm-up */
	gpio_set_gpio_pin(ADS1299_PIN_RESET);
	delay_ms(200);
	/* Issue RESET pulse to initialize ADS1299 registers */
	gpio_clr_gpio_pin(ADS1299_PIN_RESET);
	delay_us(1);
	gpio_set_gpio_pin(ADS1299_PIN_RESET);
	delay_us(10);
	
	/* Begin SPI comms to tell chip to exit continuous data mode */
	ads1299_send_byte(chip_select, ADS1299_OPC_SDATAC);
	
	/* Activate conversion (if desired; otherwise comment out) */
	//ads1299_hard_start_conversion();
		
	return ADS1299_STATUS_OK;
	#else
	#endif /* #if UC3 */
}

/*
inline ads1299_error_t ads1299_powerdn (void)
{
	#if UC3
	gpio_clr_gpio_pin(ADS1299_PIN_PWDN);
	return ADS1299_STATUS_OK;
	#else
	#endif
}

inline ads1299_error_t ads1299_powerup (void)
{
	#if UC3
	gpio_set_gpio_pin(ADS1299_PIN_PWDN);
	return ADS1299_STATUS_OK;
	#else
	#endif
}
*/

/* REGISTER READ/WRITE FUNCTIONS *****************************************************************************************************************/

ads1299_error_t ads1299_rreg(uint8_t chip_select, ads1299_register_address_t reg_addr, uint8_t* read_reg_val_ptr)
{
	#if UC3
	uint16_t read_data;
		
	spi_selectChip(SPI_ADDRESS, chip_select);
	
	/* First byte: read command for specified register */
	ads1299_send_byte_no_cs(ADS1299_OPC_RREG | reg_addr);
	
	/* Second byte: Read only 1 register (send n-1, where n is number registers to read) */
	ads1299_send_byte_no_cs(0x00);
	
	/* Dummy byte to clock in data */
	ads1299_send_byte_no_cs(DUMMY_BYTE);
	
	/* Read SPI RX register */
	read_data = spi_get(SPI_ADDRESS);
	*read_reg_val_ptr = (uint8_t) read_data;	
	
	spi_unselectChip(SPI_ADDRESS, chip_select);
		
	return ADS1299_STATUS_OK;
	#else
	#endif /* #if UC3 */
}

ads1299_error_t ads1299_wreg(uint8_t chip_select, ads1299_register_address_t reg_addr, uint8_t reg_val_to_write)
{
	#if UC3
	spi_selectChip(SPI_ADDRESS, chip_select);
	
	/* First byte: write command for specified register */
	ads1299_send_byte_no_cs(ADS1299_OPC_WREG | reg_addr);
	
	/* Second byte: number of registers to write (1) */
	ads1299_send_byte_no_cs(0x00);
	
	/* Third byte: write register value */
	ads1299_send_byte_no_cs(reg_val_to_write);
		
	spi_unselectChip(SPI_ADDRESS, chip_select);
		
	return ADS1299_STATUS_OK;
	#else
	#endif /* #if UC3 */
}

/* DATA RETRIEVAL FUNCTIONS **********************************************************************************************************************/

ads1299_error_t ads1299_rdata32(uint8_t chip_select, volatile uint32_t* status_ptr, 
								volatile uint32_t sample_idx, volatile signal_t channel_array[][SIGNAL_BUFFER_SIZE])
{
	#if UC3
	uint8_t channel_idx;
	volatile uint32_t statustemp = 0;
	volatile signal_t sigtemp = 0;
	
	/* Begin SPI comms and send read data command */
	spi_selectChip(SPI_ADDRESS, chip_select);
	
	ads1299_send_byte_no_cs(ADS1299_OPC_RDATA);
		
	/* Read in status word first (24 bits) */
	ads1299_send_byte_no_cs(DUMMY_BYTE);
	statustemp = (uint32_t) spi_get(SPI_ADDRESS);
	statustemp *= 65536;
	(*status_ptr) = statustemp;
		
	ads1299_send_byte_no_cs(DUMMY_BYTE);
	statustemp = (uint32_t) spi_get(SPI_ADDRESS);
	statustemp *= 256;
	(*status_ptr) += statustemp;
		
	ads1299_send_byte_no_cs(DUMMY_BYTE);
	(*status_ptr) += (uint32_t) spi_get(SPI_ADDRESS);
	
	/* Begin reading in data */	
	for (channel_idx = 0; channel_idx < MAX_CHANNELS; channel_idx++)
	{
		ads1299_send_byte_no_cs(DUMMY_BYTE);
		
		sigtemp = (signal_t) spi_get(SPI_ADDRESS);
		#ifdef USE_32_BIT_DATA
		sigtemp *= 65536;
		#else
		sigtemp *= 256;
		#endif
		channel_array[channel_idx][sample_idx] = sigtemp;
			
		ads1299_send_byte_no_cs(DUMMY_BYTE);
		
		sigtemp = (signal_t) spi_get(SPI_ADDRESS);
		#ifdef USE_32_BIT_DATA
		sigtemp *= 256;
		#endif
		channel_array[channel_idx][sample_idx] += sigtemp;
			
		ads1299_send_byte_no_cs(DUMMY_BYTE);
		
		sigtemp = (signal_t) spi_get(SPI_ADDRESS);
		#ifdef USE_32_BIT_DATA
		channel_array[channel_idx][sample_idx] += sigtemp;
		#endif
	}
		
	spi_unselectChip(SPI_ADDRESS, chip_select);
		
	return ADS1299_STATUS_OK;
	#else
	#endif	/* #if UC3 */
}

// ads1299_error_t ads1299_rdata8(uint8_t chip_select, volatile uint8_t status_array[],
// 							   volatile uint32_t sample_idx, volatile int8_t channel_array[][4*SIGNAL_BUFFER_SIZE])
// {	
// 	uint8_t channel_idx, i;
// 	
// 	/* So that we can use the same sample_idx from ads1299_rdata32, multiply idx by 4 */
// 	sample_idx = ((sample_idx == ((4*SIGNAL_BUFFER_SIZE)-1))? 0: (4*sample_idx));
// 	
// 	spi_selectChip(SPI_ADDRESS, chip_select);
// 	
// 	ads1299_send_byte_no_cs(ADS1299_OPC_RDATA);
// 	
// 	/* Read in status word first (24 bits) */
// 	ads1299_send_byte_no_cs(DUMMY_BYTE);
// 	status_array[0] = (uint8_t) spi_get(SPI_ADDRESS);
// 	
// 	ads1299_send_byte_no_cs(DUMMY_BYTE);
// 	status_array[1] = (uint8_t) spi_get(SPI_ADDRESS);
// 	
// 	ads1299_send_byte_no_cs(DUMMY_BYTE);
// 	status_array[2] = (uint8_t) spi_get(SPI_ADDRESS);
// 	
// 	/* Begin reading in data */
// 	for (channel_idx = 0; channel_idx < MAX_CHANNELS; channel_idx++)
// 	{
// 		for (i = 0; i < 3; i++)
// 		{
// 			ads1299_send_byte_no_cs(DUMMY_BYTE);
// 			channel_array[channel_idx][sample_idx+i] = (uint8_t) spi_get(SPI_ADDRESS);
// 		}
// 		channel_array[channel_idx][sample_idx+4] = 0xAA;		
// 	}
// 	
// 	spi_unselectChip(SPI_ADDRESS, chip_select);
// 	
// 	return ADS1299_STATUS_OK;								   
// }

/*
ads1299_error_t ads1299_24bit_read(uint8_t chip_select, *int32_t output_data_ptr)
{
	int32_t temp;

	spi_put(SPI_ADDRESS, 0x00);
	while(!spi_is_tx_empty(SPI_ADDRESS));
	temp = (int32_t) spi_get(SPI_ADDRESS);
	temp *= 65536;
	*output_data_ptr = temp;
	
	spi_put(SPI_ADDRESS, 0x00);
	while(!spi_is_tx_empty(SPI_ADDRESS));
	temp = (int32_t) spi_get(SPI_ADDRESS);
	temp *= 256;
	*output_data_ptr += temp;
	
	spi_put(SPI_ADDRESS, 0x00);
	while(!spi_is_tx_empty(SPI_ADDRESS));
	*output_data_ptr += (int32_t) spi_get(SPI_ADDRESS);
	
	return ADS1299_STATUS_OK;
}
*/