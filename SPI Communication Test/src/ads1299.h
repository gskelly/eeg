/**
 *  \file ads1299.h
 *  \brief Configuration settings, register definitions, and function prototypes for using the ADS1299 EEG analog front-end from Texas Instruments.
 *	\author Graham Kelly
 *	\version 0.1
 *	\date June 2013
 *	\copyright
 */

#ifndef ADS1299_H_
#define ADS1299_H_

#include "ads1299_comms_adapt.h"

#if UC3

#include "gpio.h"
#include "delay.h"

#else
/* #if other processors ... */
  
#endif /* #if UC3 */

/**************************************************************************************************************************************************
*               User-Editable Macros                                                                                                             *
**************************************************************************************************************************************************/

/* The following pin assignments may be changed according to the application. */
/**
 *  \brief Defines the host processor pin assigned to the RESET pin on the ADS1299.
 */
#define ADS1299_PIN_RESET		AVR32_PIN_PB04

/**
 *  \brief Defines the host processor pin assigned to the DRDY pin on the ADS1299.
 */
#define ADS1299_PIN_DRDY		AVR32_PIN_PB07
#define ADS1299_DRDY_INT		AVR32_EIC_INT1
#define ADS1299_DRDY_IRQ		AVR32_EIC_IRQ_1
#define ADS1299_DRDY_INT_FN		AVR32_EIC_EXTINT_1_1_FUNCTION	

/**
 *  \brief Defines the host processor pin assigned to the START pin on the ADS1299.
 */
#define ADS1299_PIN_START		AVR32_PIN_PA17

/**
 *  \brief Defines the host processor pin assigned to the PWDN pin on the ADS1299.
 */
//#define ADS1299_PIN_PWDN		

/**
 *  \brief Defines the size of the input signal buffer.
 *
 *  SIGNAL_BUFFER_SIZE should be a power of 2.
 */
#define SIGNAL_BUFFER_SIZE		256

#if (SIGNAL_BUFFER_SIZE == 0)
#error "SIGNAL_BUFFER_SIZE must be greater than 0."
#endif

#if ((SIGNAL_BUFFER_SIZE & (SIGNAL_BUFFER_SIZE - 1)) != 0)
#define BUFFER_SIZE_NOT_POWER_2
#pragma message "SIGNAL_BUFFER_SIZE is not a power of 2. Circular buffer implementation will be slower." 
#endif

/**
 *  \brief Defines the maximum number of input channels available.
 *
 *	The maximum number of channels for a single ADS1299 is 8. MAX_CHANNELS may be decreased if this many channels 
 *  are not desired. Increasing the number to support multiple ADCs is not currently supported.
 */
#define MAX_CHANNELS			6

#if ((MAX_CHANNELS > 8) || (MAX_CHANNELS < 1))
#error "Constant MAX_CHANNELS must be between 1 and 8."
#endif			

/**
 *  \brief Stores full 24-bit ADC data in 32-bit datatype.
 *
 *	Data from the ADS1299 is 24 bits long. These values may be stored in their entirety as 32-bit numbers for more
 *  precision, or they may be truncated to 16 bits for faster data processing. USE_32_BIT_DATA is referenced when
 *  reading data over SPI, and also determines the size of the signal_t typedef.
 */
//#define USE_32_BIT_DATA

/**
 *  \brief Maximum size of packet data payload in samples.
 */
#define MAX_PAYLOAD_SIZE			1

/**************************************************************************************************************************************************
*               Other Useful Definitions                                                                                                          *
**************************************************************************************************************************************************/

/* Bit masking macros */
#define SETMASK(A,B)						(A |= B)
#define CLRMASK(A,B)						(A &= ~(B))
#define TOGMASK(A,B)						(A ^= B)

/* Default register values */
#define ADS1299_REGDEFAULT_ID			ADS1299_DEVICE_ID
#define ADS1299_REGDEFAULT_CONFIG1		0x96			///< Multiple readback mode, OSC output disabled, DR = FMOD/4096
#define ADS1299_REGDEFAULT_CONFIG2		0xD0			///< Test signal sourced internally, low-amplitude test signal pulsed at FCLK/(2^21)
#define ADS1299_REGDEFAULT_CONFIG3		0x68			///< Ref buffer off, bias measurement off, internal bias ref, bias buffer off, bias sense disabled
#define ADS1299_REGDEFAULT_LOFF			0x00			///< 95%/5% LOFF comparator threshold, DC lead-off at 6 nA	
#define ADS1299_REGDEFAULT_CHNSET		0xE0			///< Channel off, gain 24, SRB2 disconnected, normal electrode input
#define ADS1299_REGDEFAULT_BIAS_SENSP	0x00			///< All BIAS channels disconnected from positive leads
#define ADS1299_REGDEFAULT_BIAS_SENSN	0x00			///< All BIAS channels disconnected from negative leads
#define ADS1299_REGDEFAULT_LOFF_SENSP	0x00			///< All LOFF channels disconnected from positive leads
#define ADS1299_REGDEFAULT_LOFF_SENSN	0x00			///< All LOFF channels disconnected from negative leads
#define ADS1299_REGDEFAULT_LOFF_FLIP	0x00			///< No flipping in this house; source/pull-up at INP, sink/pull-down at INN
#define ADS1299_REGDEFAULT_LOFF_STATP	0x00			///< This is a read-only register; reset value is 0x00
#define ADS1299_REGDEFAULT_LOFF_STATN	0x00			///< This is a read-only register; reset value is 0x00
#define ADS1299_REGDEFAULT_GPIO			0x0F			///< All GPIO set to inputs
#define ADS1299_REGDEFAULT_MISC1		0x00			///< SRB1 disconnected
#define ADS1299_REGDEFAULT_MISC2		0x00			///< Register not used in this silicon; should stay at 0x00
#define ADS1299_REGDEFAULT_CONFIG4		0x00			///< Continuous conversion, LOFF comparator powered down

/* Register value masks */
/*#define CONFIG1_DAISYCHAIN				(1<<6)	*/			
	

/**************************************************************************************************************************************************
*               Typedefs and Struct Declarations/Definitions                                                                                      *
**************************************************************************************************************************************************/

#ifdef USE_32_BIT_DATA
typedef int32_t signal_t;
#else
typedef int16_t signal_t;
#endif

typedef struct
{
	uint8_t sync0;
	uint8_t sync1;
	uint8_t version;
	uint8_t count;
	uint16_t* data;
	uint8_t switches;
} packet_t;

// typedef struct
// {
// 	uint32_t start;
// 	uint32_t packetnum;
// 	uint32_t status;
// 	signal_t data[MAX_PAYLOAD_SIZE];
// 	uint32_t checksum;
// } __attribute__((aligned(4))) packet_t;

/**
 *	\brief Error codes for interacting with the ADS1299.
 *
 */
typedef enum
{
	ADS1299_STATUS_OK				= 0,		///< No error.
	ADS1299_ERROR_SPI_TIMEOUT		= 1,		///< SPI timed out. Check SPI configuration and hardware connections.
} ads1299_error_t;

/**
 *	\brief Signal buffer object.
 *
 */
// typedef struct
// {
// 	signal_t* bufPtr;
// 	uint32_t  bufSize;
// 	 
// } ads1299_buffer;

/**
 *	\brief ADS1299 register addresses.
 *
 * Consult the ADS1299 datasheet and user's guide for more information.
 */
typedef enum 
{
	ADS1299_REGADDR_ID			= 0x00,			///< Chip ID register. Read-only. ID[4:0] should be 11110.
	ADS1299_REGADDR_CONFIG1		= 0x01,			///< Configuration register 1. Controls daisy-chain mode; clock output; and data rate.
	ADS1299_REGADDR_CONFIG2		= 0x02,			///< Configuration register 2. Controls calibration signal source, amplitude, and frequency.
	ADS1299_REGADDR_CONFIG3		= 0x03,			///< Configuration register 3. Controls reference buffer power and the bias functionality.
	ADS1299_REGADDR_LOFF		= 0x04,			///< Lead-off control register. Controls lead-off frequency, magnitude, and threshold.
	ADS1299_REGADDR_CH1SET		= 0x05,			///< Channel 1 settings register. Controls channel 1 input mux, SRB2 switch, gain, and power-down.
	ADS1299_REGADDR_CH2SET		= 0x06,			///< Channel 2 settings register. Controls channel 2 input mux, SRB2 switch, gain, and power-down.
	ADS1299_REGADDR_CH3SET		= 0x07,			///< Channel 3 settings register. Controls channel 3 input mux, SRB2 switch, gain, and power-down.
	ADS1299_REGADDR_CH4SET		= 0x08,			///< Channel 4 settings register. Controls channel 4 input mux, SRB2 switch, gain, and power-down.
	ADS1299_REGADDR_CH5SET		= 0x09,			///< Channel 5 settings register. Controls channel 5 input mux, SRB2 switch, gain, and power-down.
	ADS1299_REGADDR_CH6SET		= 0x0A,			///< Channel 6 settings register. Controls channel 6 input mux, SRB2 switch, gain, and power-down.
	ADS1299_REGADDR_CH7SET		= 0x0B,			///< Channel 7 settings register. Controls channel 7 input mux, SRB2 switch, gain, and power-down.
	ADS1299_REGADDR_CH8SET		= 0x0C,			///< Channel 8 settings register. Controls channel 8 input mux, SRB2 switch, gain, and power-down.
	ADS1299_REGADDR_BIAS_SENSP	= 0x0D,			///< Bias drive positive sense selection. Selects channels for bias drive derivation (positive side).
	ADS1299_REGADDR_BIAS_SENSN	= 0x0E,			///< Bias drive negative sense selection. Selects channels for bias drive derivation (negative side).
	ADS1299_REGADDR_LOFF_SENSP	= 0x0F,			///< Lead-off positive sense selection. Selects channels that will use lead-off detection (positive side).
	ADS1299_REGADDR_LOFF_SENSN	= 0x10,			///< Lead-off negative sense selection. Selects channels that will use lead-off detection (negative side).
	ADS1299_REGADDR_LOFF_FLIP	= 0x11,			///< 1: Swaps lead-off current source and sink on the corresponding channel. See datasheet.
	ADS1299_REGADDR_LOFF_STATP	= 0x12,			///< Lead-off positive side status register. Read-only. 0: lead on, 1: lead off.
	ADS1299_REGADDR_LOFF_STATN	= 0x13,			///< Lead-off negative side status register. Read-only. 0: lead on, 1: lead off.
	ADS1299_REGADDR_GPIO		= 0x14,			///< GPIO register. Controls state and direction of the ADS1299 GPIO pins.
	ADS1299_REGADDR_MISC1		= 0x15,			///< Miscellaneous 1. Connects/disconnects SRB1 to all channels' inverting inputs.
	ADS1299_REGADDR_MISC2		= 0x16,			///< Miscellaneous 2. No functionality in current revision of ADS1299.
	ADS1299_REGADDR_CONFIG4		= 0x17			///< Configuration register 4. Enables/disables single-shot mode and controls lead-off comparator power.
} ads1299_register_address_t;

/**
 *	\brief ADS1299 SPI communication opcodes.
 *	
 * Consult the ADS1299 datasheet and user's guide for more information.
 * For RREG and WREG opcodes, the first byte (opcode) must be ORed with the address of the register to be read/written. 
 * The command is completed with a second byte 000n nnnn, where n nnnn is (# registers to read) - 1.
 */
typedef enum
{
	ADS1299_OPC_WAKEUP		= 0x02,			///< Wake up from standby.
	ADS1299_OPC_STANDBY		= 0x04,			///< Enter standby.
	ADS1299_OPC_RESET		= 0x06,			///< Reset all registers.	
	ADS1299_OPC_START		= 0x08,			///< Start data conversions.
	ADS1299_OPC_STOP		= 0x0A,			///< Stop data conversions.
	
	ADS1299_OPC_RDATAC		= 0x10,			///< Read data continuously (registers cannot be read or written in this mode).
	ADS1299_OPC_SDATAC		= 0x11,			///< Stop continuous data read.
	ADS1299_OPC_RDATA		= 0x12,			///< Read single data value.
	
	ADS1299_OPC_RREG		= 0x20,			///< Read register value.
	ADS1299_OPC_WREG		= 0x40			///< Write register value.
} ads1299_opcode_t;

/* ID REGISTER ***********************************************************************************************************************************/

/**
 *  \brief Factory-programmed device ID for ADS1299, stored in ID[3:0].
 */ 
// Factory-programmed device ID for ADS1299. Stored in ID[3:0].
#define ADS1299_DEVICE_ID		0b1110

/**
 *  \brief Bitfield struct for ID register.
 *
 * Consult the ADS1299 datasheet, page 40, for more information.
 */
typedef union 
{
	uint8_t VALUE;
	struct  
	{
		uint8_t RESERVED_1:	4;				///< Reserved; must be set to 0b0001
		uint8_t DEVID:		4;				///< Device ID; should be 0b1110 for ADS1299
	
	} BITS;
} ads1299_reg_id_t;

/* CONFIG1 REGISTER ******************************************************************************************************************************/

/**
 *  \brief Bit value enum for CONFIG1.DR (data rate).
 *
 * FMOD = FCLK/2, where FCLK is the clock frequency of the ADS1299. This is normally 2.048 MHz.
 */
typedef enum
{
	FMOD_DIV_BY_64			= 0b000,		///< Data is output at FMOD/64, or 16 kHz at 2.048 MHz.
	FMOD_DIV_BY_128			= 0b001,		///< Data is output at FMOD/128, or 8 kHz at 2.048 MHz.
	FMOD_DIV_BY_256			= 0b010,		///< Data is output at FMOD/256, or 4 kHz at 2.048 MHz.
	FMOD_DIV_BY_512			= 0b011,		///< Data is output at FMOD/512, or 2 kHz at 2.048 MHz.
	FMOD_DIV_BY_1024		= 0b100,		///< Data is output at FMOD/1024, or 1 kHz at 2.048 MHz.
	FMOD_DIV_BY_2048		= 0b101,		///< Data is output at FMOD/2048, or 500 Hz at 2.048 MHz.
	FMOD_DIV_BY_4096		= 0b110			///< Data is output at FMOD/4096, or 250 Hz at 2.048 MHz.
} ads1299_bitval_dr_t;

/**
 *  \brief Bitfield struct for CONFIG1 register.
 *
 * Consult the ADS1299 datasheet, page 40, for more information.
 */
typedef union 
{
	uint8_t VALUE;
	struct  
	{
		ads1299_bitval_dr_t		DR:				3;			///< DR[2:0]: Output data rate.
		uint8_t					RESERVED_10:	2;			///< Must be set to 0b10.
		uint8_t					CLK_EN:			1;			///< 1: clock output on CLK pin.
		uint8_t					DAISY_EN:		1;			///< 0: Daisy-chain mode, 1: Multiple readback mode.
		uint8_t					RESERVED_1:		1;			///< Must be set to 1.
	
	} BITS;
} ads1299_reg_config1_t;

/* CONFIG2 REGISTER ******************************************************************************************************************************/

/**
 *  \brief Bit value enum for CONFIG2.CAL_FREQ (calibration signal frequency).
 *
 * FCLK is the clock frequency of the ADS1299. This is normally 2.048 MHz.
 */
typedef enum
{
	CAL_PULSE_FCLK_DIV_2_21		= 0,		///< Calibration signal pulsed at FCLK/2^21, or approx. 1 Hz at 2.048 MHz.
	CAL_PULSE_FCLK_DIV_2_20		= 1,		///< Calibration signal pulsed at FCLK/2^20, or approx. 2 Hz at 2.048 MHz.
	CAL_DC						= 3			///< Calibration signal is not pulsed.
} ads1299_bitval_cal_freq_t;

/**
 *  \brief Bit value enum for CONFIG2.CAL_AMP0 (calibration signal amplitude).
 */
typedef enum
{
	CAL_AMP_VREF_DIV_2_4_MV		= 0,		///< Calibration signal amplitude is 1 x (VREFP - VREFN)/(2.4 mV).
	CAL_AMP_2VREF_DIV_2_4_MV	= 1			///< Calibration signal amplitude is 2 x (VREFP - VREFN)/(2.4 mV).
} ads1299_bitval_cal_amp_t;

/**
 *  \brief Bit value enum for CONFIG2.INT_CAL (calibration signal source).
 */
typedef enum
{
	CAL_EXT						= 0,		///< Calibration signal is driven externally.
	CAL_INT						= 1			///< Calibration signal is driven internally.
} ads1299_bitval_int_cal_t;

/**
 *  \brief Bitfield struct for CONFIG2 register.
 *
 * Consult the ADS1299 datasheet, page 41, for more information.
 */
typedef union
{
	uint8_t VALUE;
	struct 
	{
		ads1299_bitval_cal_freq_t CAL_FREQ:     2;	///< CAL_FREQ[1:0]: Test signal frequency.
		ads1299_bitval_cal_amp_t  CAL_AMP0:     1;	///< Test signal amplitude.
		uint8_t					  RESERVED_0:   1;	///< Must be set to 0.
		ads1299_bitval_int_cal_t  INT_CAL:      1;	///< Test signal source; 0: externally driven, 1: internally driven.
		uint8_t					  RESERVED_110:	3;	///< Must be set to 0b110.
	} BITS;
} ads1299_reg_config2_t;

/* CONFIG3 REGISTER ******************************************************************************************************************************/

/**
 *  \brief 
 */
typedef enum
{
	BIAS_LOFF_SENSE_DISABLED			= 0,
	BIAS_LOFF_SENSE_ENABLED				= 1
} ads1299_bitval_bias_loff_sens_t;

/**
 *  \brief 
 */
typedef enum
{
	BIAS_BUFFER_DISABLED				= 0,
	BIAS_BUFFER_ENABLED					= 1
} ads1299_bitval_pd_bias_t;

/**
 *  \brief 
 */
typedef enum
{
	BIAS_REFERENCE_EXT					= 0,
	BIAS_REFERENCE_INT					= 1
} ads1299_bitval_biasref_int_t;

/**
 *  \brief 
 */
typedef enum
{
	BIAS_MEAS_DISABLED					= 0,
	BIAS_MEAS_ENABLED					= 1
} ads1299_bitval_bias_meas_t;

/**
 *  \brief 
 */
typedef enum
{
	REFERENCE_BUFFER_DISABLED			= 0,
	REFERENCE_BUFFER_ENABLED			= 1
} ads1299_bitval_pd_refbuf_t;

/**
 *  \brief Bitfield struct for CONFIG3 register.
 *
 * Consult the ADS1299 datasheet, page 42, for more information.
 */
typedef union  
{
	uint8_t VALUE;
	struct  
	{
		uint8_t BIAS_STAT:								1;	///< Read-only; 0: BIAS lead connected, 1: BIAS leadoff detected.
		ads1299_bitval_bias_loff_sens_t BIAS_LOFF_SENS:	1;	///< 0: BIAS leadoff sense disabled, 1: enabled.
		ads1299_bitval_pd_bias_t PD_BIAS:				1;	///< 0: BIAS buffer powered down, 1: enabled.
		ads1299_bitval_biasref_int_t BIASREF_INT:		1;	///< 0: BIASREF fed externally, 1: BIASREF = internal midsupply.
		ads1299_bitval_bias_meas_t BIAS_MEAS:			1;	///< 0: BIAS measurement disabled, 1: BIASIN routed to MUX 010.
		uint8_t RESERVED_11:							2;	///< Must be set to 0b11.
		ads1299_bitval_pd_refbuf_t PD_REFBUF:			1;	///< 0: reference buffer powered down, 1: enabled.
	} BITS;	
} ads1299_reg_config3_t;

/* CONFIG4 REGISTER ******************************************************************************************************************************/

#define CONFIG4_RESERVED_VALUE		0

/**
 *  \brief Bitfield struct for CONFIG4 register.
 *
 * Consult the ADS1299 datasheet, page 47, for more information.
 */
typedef union
{
	uint8_t VALUE;
	struct
	{
		uint8_t RESERVED_0A:			1;			///< Must be set to 0.
		uint8_t PD_LOFF_COMP:			1;			///< Lead-off comparator power-down.
		uint8_t RESERVED_0B:			1;			///< Must be set to 0.
		uint8_t SINGLE_SHOT:			1;			///< Single-shot conversion.
		uint8_t RESERVED_0000:			4;			///< Must be set to 0000.
	} BITS;	
} ads1299_reg_config4_t;

/* LOFF REGISTER *********************************************************************************************************************************/

#define	LOFF_RESERVED_VALUE			0

/**
 *  \brief Bit value enum for LOFF.FLEAD_OFF (lead-off current frequency).
 *
 * This should be as large as possible for continuous AC lead-off detection to ensure that it is out
 * of the EEG frequency band (approx. 0-100 Hz for most applications). The excitation signal can then 
 * be filtered out of the acquired overall signal, and its voltage amplitude measured in order to determine 
 * the electrode impedance.
 * FCLK is the clock frequency of the ADS1299. This is normally 2.048 MHz.
 * FDR is the output data rate. With the default clock, this must be at least 1 kHz in order to use
 * continuous AC impedance monitoring, since the excitation frequency of FDR/4 = 250 Hz is the lowest
 * possible frequency outside of the EEG band.
 */

typedef enum
{
	DC_LEAD_OFF					= 0b00,			///< Lead-off current is at DC.
	AC_LEAD_OFF_FCLK_DIV_2_18	= 0b01,			///< Lead-off current is at FCLK/2^18, or 7.8125 Hz at 2.048 MHz.
	AC_LEAD_OFF_FCLK_DIV_2_16	= 0b10,			///< Lead-off current is at FCLK/2^16, or 31.25 Hz at 2.048 MHz.
	AC_LEAD_OFF_FDR_DIV_4		= 0b11			///< Lead-off current is at FDR/4, or approx. 8 Hz at 2.048 MHz.			
} ads1299_bitval_fleadoff_t;

/**
 *  \brief Bit value enum for LOFF.ILEAD_OFF (lead-off current magnitude).
 *
 * This should be as small as possible for continuous lead-off detection, so as not to noticeably alter
 * the acquired signal. Default is _6_NA.
 */

typedef enum
{
	_6_NA						= 0b00,			///< 6 nA lead-off current.
	_24_NA						= 0b01,			///< 24 nA lead-off current.
	_6_UA						= 0b10,			///< 6 uA lead-off current.
	_24_UA						= 0b11			///< 24 uA lead-off current.
} ads1299_bitval_ileadoff_t;

/**
 *  \brief Bit value enum for LOFF.COMP_TH (lead-off comparator threshold).
 *
 * Definition names are for the positive side (LOFFP). The corresponding LOFFN thresholds
 * are the difference between these thresholds and 100%. Default value is _95_PERCENT.
 */
typedef enum
{
	_95_PERCENT					= 0b000,
	_92_5_PERCENT				= 0b001,
	_90_PERCENT					= 0b010,
	_87_5_PERCENT				= 0b011,
	_85_PERCENT					= 0b100,
	_80_PERCENT					= 0b101,
	_75_PERCENT					= 0b110,
	_70_PERCENT					= 0b111						
} ads1299_bitval_comp_th_t;

/**
 *  \brief Bitfield struct for LOFF register.
 *
 * Consult the ADS1299 datasheet, page 43, for more information.
 */
typedef union 
{
	uint8_t VALUE;
	struct 
	{
		ads1299_bitval_fleadoff_t	FLEAD_OFF:	2;			///< Lead-off frequency.
		ads1299_bitval_ileadoff_t	ILEAD_OFF:	2;			///< Lead-off current magnitude.
		uint8_t						RESERVED_0:	1;			///< Must be set to 0.
		ads1299_bitval_comp_th_t	COMP_TH:	3;			///< Lead-off comparator threshold.
	} BITS;
} ads1299_reg_loff_t;

/* CHnSET REGISTERS ******************************************************************************************************************************/

/**
 *  \brief Bit value enum for CHnSET.MUX (channel mux setting).
 *
 * Controls the channel multiplexing on the ADS1299.
 * Consult the ADS1299 datasheet, pages 16-17, for more information.
 */
typedef enum
{
	NORMAL_ELECTRODE		= 0b000,			///< Channel is connected to the corresponding positive and negative input pins.
	INPUT_SHORTED			= 0b001,			///< Channel inputs are shorted together. Used for offset and noise measurements.
	BIAS_MEASUREMENT		= 0b010,			///< Used with CONFIG3.BIAS_MEAS for bias measurement. See ADS1299 datasheet, pp. 53-54.
	MVDD_SUPPLY				= 0b011,			///< Used for measuring analog and digital supplies. See ADS1299 datasheet, p. 17.
	TEMPERATURE_SENSOR		= 0b100,			///< Measures device temperature. See ADS1299 datasheet, p. 17.
	TEST_SIGNAL				= 0b101,			///< Measures calibration signal. See ADS1299 datasheet, pp. 17 and 41.
	BIAS_DRIVE_P			= 0b110,			///< Connects positive side of channel to bias drive output.
	BIAS_DRIVE_N			= 0b111 			///< Connects negative side of channel to bias drive output.
} ads1299_bitval_mux_t;

/**
 *  \brief 
 */
typedef enum
{
	SRB2_DISCONNECTED		= 0,
	SRB2_CONNECTED			= 1
} ads1299_bitval_srb2_t;

/**
 *  \brief Bit value enum for CHnSET.GAIN (channel PGA gain).
 *
 * Take care to ensure that the gain is appropriate for the common-mode level of the device inputs.
 * Higher gain settings have lower input-referred noise.
 * Consult the ADS1299 datasheet, pages 6-7 and 19-20, for more information.
 */
typedef enum
{
	GAIN_1					= 0b000,			///< PGA gain = 1.
	GAIN_2					= 0b001,			///< PGA gain = 2.
	GAIN_4					= 0b010,			///< PGA gain = 4.
	GAIN_6					= 0b011,			///< PGA gain = 6.
	GAIN_8					= 0b100,			///< PGA gain = 8.
	GAIN_12					= 0b101,			///< PGA gain = 12.
	GAIN_24					= 0b110				///< PGA gain = 24.
} ads1299_bitval_gain_t;

/**
 *  \brief 
 */
typedef enum
{
	CHANNEL_ON				= 0,
	CHANNEL_OFF				= 1
} ads1299_bitval_chan_pd_t;

/**
 *  \brief Bitfield struct for the CHnSET registers.
 *
 * This struct provides the generic bitfield for all 8 channel registers on the ADS1299.
 * Consult the ADS1299 datasheet, page 44, for more information.
 */
typedef union  
{
	uint8_t VALUE;
	struct 
	{
		ads1299_bitval_mux_t		MUX:	3;		///< Channel input type.
		ads1299_bitval_srb2_t		SRB2:	1;		///< Stim/ref/bias channel 2; 0: off, 1: on.
		ads1299_bitval_gain_t		GAIN:	3;		///< PGA gain setting.
		ads1299_bitval_chan_pd_t	PD:		1;		///< Power-down; 0: normal operation, 1: powered down.
	} BITS;	
} ads1299_reg_chnset_t;

/* BIAS_SENSP REGISTER ****************************************************************************************************************************/

typedef union
{
	uint8_t VALUE;
	struct  
	{
		uint8_t	BIASP8:	1;
		uint8_t	BIASP7:	1;
		uint8_t	BIASP6:	1;
		uint8_t	BIASP5:	1;
		uint8_t	BIASP4:	1;
		uint8_t	BIASP3:	1;
		uint8_t	BIASP2:	1;
		uint8_t	BIASP1:	1;
	} BITS;
} ads1299_reg_bias_sensp_t;

/* BIAS_SENSN REGISTER ****************************************************************************************************************************/

typedef union
{
	uint8_t VALUE;
	struct
	{
		uint8_t BIASN8:	1;
		uint8_t	BIASN7:	1;
		uint8_t	BIASN6:	1;
		uint8_t	BIASN5:	1;
		uint8_t	BIASN4:	1;
		uint8_t	BIASN3:	1;
		uint8_t	BIASN2:	1;
		uint8_t	BIASN1:	1;
	} BITS;
} ads1299_reg_bias_sensn_t;

/* LOFF_SENSP REGISTER ****************************************************************************************************************************/

typedef union
{
	uint8_t VALUE;
	struct
	{
		uint8_t LOFFP8: 1;
		uint8_t	LOFFP7:	1;
		uint8_t	LOFFP6:	1;
		uint8_t	LOFFP5:	1;
		uint8_t	LOFFP4:	1;
		uint8_t	LOFFP3:	1;
		uint8_t	LOFFP2:	1;
		uint8_t	LOFFP1:	1;
	} BITS;
} ads1299_reg_loff_sensp_t;

/* LOFF_SENSN REGISTER ****************************************************************************************************************************/

typedef union
{
	uint8_t VALUE;
	struct
	{
		uint8_t LOFFN8: 1;
		uint8_t	LOFFN7:	1;
		uint8_t	LOFFN6:	1;
		uint8_t	LOFFN5:	1;
		uint8_t	LOFFN4:	1;
		uint8_t	LOFFN3:	1;
		uint8_t	LOFFN2:	1;
		uint8_t	LOFFN1:	1;
	} BITS;
} ads1299_reg_loff_sensn_t;

/* LOFF_FLIP REGISTER ****************************************************************************************************************************/

typedef union
{
	uint8_t VALUE;
	struct
	{
		uint8_t	LOFF_FLIP8:	1;
		uint8_t	LOFF_FLIP7:	1;
		uint8_t	LOFF_FLIP6:	1;
		uint8_t	LOFF_FLIP5:	1;
		uint8_t	LOFF_FLIP4:	1;
		uint8_t	LOFF_FLIP3:	1;
		uint8_t	LOFF_FLIP2:	1;
		uint8_t	LOFF_FLIP1:	1;
	} BITS;
} ads1299_reg_loff_flip_t;

/* LOFF_STATP REGISTER ***************************************************************************************************************************/

typedef union
{
	uint8_t VALUE;
	struct
	{
		uint8_t	IN8P_OFF:	1;
		uint8_t	IN7P_OFF:	1;
		uint8_t	IN6P_OFF:	1;
		uint8_t	IN5P_OFF:	1;
		uint8_t	IN4P_OFF:	1;
		uint8_t	IN3P_OFF:	1;
		uint8_t	IN2P_OFF:	1;
		uint8_t	IN1P_OFF:	1;
		
	} BITS;
} ads1299_reg_loff_statp_t;

/* LOFF_STATN REGISTER ***************************************************************************************************************************/

typedef union
{
	uint8_t VALUE;
	struct
	{
		uint8_t	IN8N_OFF:	1;
		uint8_t	IN7N_OFF:	1;
		uint8_t	IN6N_OFF:	1;
		uint8_t	IN5N_OFF:	1;
		uint8_t	IN4N_OFF:	1;
		uint8_t	IN3N_OFF:	1;
		uint8_t	IN2N_OFF:	1;
		uint8_t	IN1N_OFF:	1;
		
	} BITS;
} ads1299_reg_loff_statn_t;

/* GPIO REGISTER *********************************************************************************************************************************/

/**
 *	\brief Bitfield struct for the GPIO register.
 *
 * The ADS1299 has 4 GPIO pins that can be manipulated via the SPI bus if there are not enough
 * GPIO pins available on the host.
 * GPIOC[4:1] controls the pin direction on GPIO pins 4:1.
 * GPIOD[4:1] controls the logic levels on GPIO pins 4:1.
 */
typedef union  
{
	uint8_t VALUE;
	struct  
	{
		uint8_t GPIO4C:				1;				///< GPIO control 4; 0: output, 1: input.
		uint8_t GPIO3C:				1;				///< GPIO control 3; 0: output, 1: input.
		uint8_t GPIO2C:				1;				///< GPIO control 2; 0: output, 1: input.
		uint8_t GPIO1C:				1;				///< GPIO control 1; 0: output, 1: input.
		uint8_t GPIO4D:				1;				///< GPIO data 4.
		uint8_t GPIO3D:				1;				///< GPIO data 3.
		uint8_t GPIO2D:				1;				///< GPIO data 2.
		uint8_t GPIO1D:				1;				///< GPIO data 1.
	} BITS;	
} ads1299_reg_gpio_t;

/* MISC1 REGISTER ********************************************************************************************************************************/

typedef enum
{
	SRB1_OFF					= 0,			///< Stim/ref/bias 1 turned off.
	SRB1_ON						= 1				///< Stim/ref/bias 1 connected to all channel inverting inputs.
} ads1299_bitval_srb1_t;

/**
 *  \brief Bitfield struct for the MISC1 register.
 *
 *	MISC1 controls only one function, the SRB1 channel.
 *	Consult the ADS1299 user's guide for more information.
 *
 *	\todo (GSK TODO: add page numbers)
 */
typedef union
{
	uint8_t VALUE;
	struct  
	{
		uint8_t					RESERVED_00000:	5;	///< Must be set to 00000.
		ads1299_bitval_srb1_t	SRB1:			1;	///< Stim/ref/bias channel 1; 0: off, 1: on.
		uint8_t					RESERVED_00:	2;	///< Must be set to 00.
	} BITS;
} ads1299_reg_misc1_t;

/* MISC2 REGISTER ********************************************************************************************************************************/

/**
 *  \brief Bitfield struct for the MISC2 register.
 *
 *	MISC2 don't do nothin' right now!
 *	Consult the ADS1299 user's guide for more information.
 *
 *	\todo (GSK TODO: add page numbers)
 */
typedef union
{
	uint8_t VALUE;
	struct
	{
		uint8_t RESERVED:			8;			///< Must be set to 00000000.
	} BITS;
} ads1299_reg_misc2_t;



typedef struct
{
	ads1299_reg_id_t			ID;
	ads1299_reg_config1_t		CONFIG1;
	ads1299_reg_config2_t		CONFIG2;
	ads1299_reg_config3_t		CONFIG3;
	ads1299_reg_loff_t			LOFF;
	ads1299_reg_chnset_t		CH1SET;
	ads1299_reg_chnset_t		CH2SET;
	ads1299_reg_chnset_t		CH3SET;
	ads1299_reg_chnset_t		CH4SET;
	ads1299_reg_chnset_t		CH5SET;
	ads1299_reg_chnset_t		CH6SET;
	ads1299_reg_chnset_t		CH7SET;
	ads1299_reg_chnset_t		CH8SET;
	ads1299_reg_bias_sensp_t	BIAS_SENSP;
	ads1299_reg_bias_sensn_t	BIAS_SENSN;
	ads1299_reg_loff_sensp_t	LOFF_SENSP;
	ads1299_reg_loff_sensn_t	LOFF_SENSN;
	ads1299_reg_loff_flip_t		LOFF_FLIP;
	ads1299_reg_loff_statp_t	LOFF_STATP;
	ads1299_reg_loff_statn_t	LOFF_STATN;
	ads1299_reg_gpio_t			GPIO;
	ads1299_reg_misc1_t			MISC1;
	ads1299_reg_misc2_t			MISC2;
	ads1299_reg_config4_t		CONFIG4;
} ads1299_registers_t;

/**************************************************************************************************************************************************
*               Prototypes                                                                                                                        *
**************************************************************************************************************************************************/

/**
 *	\brief Initialize packet structure for EEG data communication.
 */
void ads1299_packet_init(packet_t*, uint16_t*);

/**
 *	\brief Initialize the ADS1299.
 *
 * This function performs the power-on reset and initialization procedure documented on page 58 of the
 * ADS1299 datasheet, up to "Send SDATAC Command."
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \return Zero if successful, or an error code if unsuccessful.
 */
ads1299_error_t ads1299_device_init(uint8_t);

/**
 *	\brief Read a single register from the ADS1299.
 *
 * This function sends the RREG opcode, logical OR'd with the specified register address, and
 * writes the obtained register value to a variable. This command will have no effect if the 
 * device is in continuous read mode.
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \param reg_addr The register address of the register to be read.
 * \param read_reg_val_ptr Pointer to the variable to store the read register value.
 * \return Zero if successful, or an error code if unsuccessful.
 */
ads1299_error_t ads1299_rreg(uint8_t, ads1299_register_address_t, uint8_t*);

/**
 *	\brief Write a single register on the ADS1299.
 *
 * This function sends the WREG opcode, logical OR'd with the specified register address, and
 * then writes the specified value to that register. This command will have no effect if the 
 * device is in continuous read mode.
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \param reg_addr The register address of the register to be written.
 * \param reg_val_to_write The value to be written to the specified register.
 * \return Zero if successful, or an error code if unsuccessful.
 */
ads1299_error_t ads1299_wreg(uint8_t, ads1299_register_address_t, uint8_t);

/**
 *	\brief Read a single data sample for each enabled channel.
 *
 * This function sends the RDATA opcode, then reads back the 24-bit status word and 8 channels of 24-bit data.
 * These 24-bit values are stored right-aligned in 32-bit integers.
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \param status_ptr A pointer to the location of the variable to store the status word.
 * \param sample_idx The index within each channel array of the sample to be acquired.
 * \param channel_array An array of channels, each of which is an array of samples.
 * \return Zero if successful, or an error code if unsuccessful.
 */
ads1299_error_t ads1299_rdata32(uint8_t, volatile uint32_t*, volatile uint32_t, volatile signal_t[][SIGNAL_BUFFER_SIZE]);

/**
 *	\brief Read a single data sample for each enabled channel.
 *
 * This function sends the SDATAC opcode, which exits the continuous data mode.
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \param status_ptr A pointer to the location of the variable to store the status word.
 * \param sample_idx The index within each channel array of the sample to be acquired.
 * \param channel_array An array of channels, each of which is an array of samples.
 * \return Zero if successful, or an error code if unsuccessful.
 */
//ads1299_error_t ads1299_rdata8(uint8_t, volatile uint8_t[], volatile uint32_t, volatile int8_t[][4*SIGNAL_BUFFER_SIZE]);


/* INLINE FUNCTIONS ******************************************************************************************************************************/

/**
 *	\brief Send a single byte to the ADS1299 without manipulating chip select.
 *
 *	Use this function when multiple bytes need to be sent and you want the chip to remain selected
 *	throughout the process.
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param opcode The opcode to send.
 * \return Zero if successful, or an error code if unsuccessful.
 */
static inline ads1299_error_t ads1299_send_byte_no_cs(ads1299_opcode_t opcode)
{
	while(!spi_is_tx_ready(SPI_ADDRESS));
	spi_put(SPI_ADDRESS, opcode);
	while(!spi_is_tx_empty(SPI_ADDRESS));
	
	return ADS1299_STATUS_OK;
}

/**
 *	\brief Send a single opcode to the ADS1299.
 *
 * This function sends the specified byte to the ADS1299. Chip select is cleared (activated) and set
 * (inactivated) within the function.
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \param opcode The opcode to send.
 * \return Zero if successful, or an error code if unsuccessful.
 */
static inline ads1299_error_t ads1299_send_byte(uint8_t chip_select, ads1299_opcode_t opcode)
{
	//#if UC3
	spi_selectChip(SPI_ADDRESS, chip_select);
	
	ads1299_send_byte_no_cs(opcode);
	
	spi_unselectChip(SPI_ADDRESS, chip_select);
	
	return ADS1299_STATUS_OK;
	//#else
	//#endif
}

/**
 *	\brief Put the ADS1299 in standby mode.
 *
 * This function sends the STANDBY opcode to the ADS1299. This places the device in a low-power mode by
 * shutting down all parts of the circuit except for the reference section. Return from standby using 
 * ads1299_wake(). Do not send any other commands during standby mode.
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \return Zero if successful, or an error code if unsuccessful.
 */
static inline ads1299_error_t ads1299_standby(uint8_t chip_select)
{
	//#if UC3
	ads1299_send_byte(chip_select, ADS1299_OPC_STANDBY);
	
	return ADS1299_STATUS_OK;
	//#else
	//#endif
}

/**
 *	\brief Wake the ADS1299 from standby mode.
 *
 * This function sends the WAKEUP opcode to the ADS1299. This returns the device to normal operation 
 * after entering standby mode using ads1299_standby(). The host must wait 4 ADS1299 clock cycles
 * (approximately 2 us at 2.048 MHz) after sending this opcode to allow the device to wake up. 
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \return Zero if successful, or an error code if unsuccessful.
 */
static inline ads1299_error_t ads1299_wake(uint8_t chip_select)
{
	//#if UC3
	ads1299_send_byte(chip_select, ADS1299_OPC_WAKEUP);
	
	return ADS1299_STATUS_OK;
	//#else
	//#endif
}

/**
 *	\brief Start analog-to-digital conversion on the ADS1299 by setting the START pin.
 *
 * This function pulls the START pin high, which begins analog-to-digital conversion on the ADS1299.
 * If conversions are already in progress, this has no effect. Pulling the START pin low 
 * ads1299_hard_stop_conversion() must follow this command by at least 4 ADS1299 clock cycles 
 * (approximately 2 us at 2.048 MHz). This command should not be used if ads1299_soft_start_conversion() 
 * has been used but has not yet been followed by ads1299_soft_stop_conversion().
 * The START pin is defined at the top of ads1299.h with the macro ADS1299_PIN_START.
 */
static inline void ads1299_hard_start_conversion(void)
{
	//#if UC3
	gpio_set_gpio_pin(ADS1299_PIN_START);
	//#else
	//#endif
}

/**
 *	\brief Stop analog-to-digital conversion on the ADS1299 by clearing the START pin.
 *
 * This function pulls the START pin low, which halts analog-to-digital conversion on the ADS1299.
 * This command must follow pulling the START pin high ads1299_hard_start_conversion() by at least 
 * 4 ADS1299 clock cycles (approximately 2 us at 2.048 MHz).
 * The START pin is defined at the top of ads1299.h with the macro ADS1299_PIN_START.
 */
static inline void ads1299_hard_stop_conversion(void)
{
	//#if UC3
	gpio_clr_gpio_pin(ADS1299_PIN_START);
	//#else
	//#endif
}

/**
 *	\brief Start analog-to-digital conversion on the ADS1299 using the START opcode.
 *
 * This function sends the START opcode, which begins analog-to-digital conversion on the ADS1299.
 * It is provided in case the START pin is not available for use in the user application.
 * If conversions are already in progress, this has no effect. The STOP command ads1299_soft_stop_conversion()
 * must follow this command by at least 4 ADS1299 clock cycles (approximately 2 us at 2.048 MHz). 
 * This command should not be used if ads1299_hard_start_conversion() has not yet been followed by 
 * ads1299_hard_stop_conversion().
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \return Zero if successful, or an error code if unsuccessful.
 */
static inline ads1299_error_t ads1299_soft_start_conversion(uint8_t chip_select)
{
	//#if UC3
	ads1299_send_byte(chip_select, ADS1299_OPC_START);
	
	return ADS1299_STATUS_OK;
	//#else
	//#endif
}

/**
 *	\brief Stop analog-to-digital conversion on the ADS1299 using the STOP opcode.
 *
 * This function sends the STOP opcode, which halts analog-to-digital conversion on the ADS1299.
 * It is provided in case the START pin is not available for use in the user application.
 * This command must follow a START opcode ads1299_soft_start_conversion() by at least 4 ADS1299
 * clock cycles (approximately 2 us at 2.048 MHz). This command should not be used if 
 * ads1299_hard_start_conversion() has not yet been followed by ads1299_hard_stop_conversion().
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \return Zero if successful, or an error code if unsuccessful.
 */
static inline ads1299_error_t ads1299_soft_stop_conversion(uint8_t chip_select)
{
	//#if UC3
	ads1299_send_byte(chip_select, ADS1299_OPC_STOP);
	
	return ADS1299_STATUS_OK;
	//#else
	//#endif
}

/**
 *	\brief Enable continuous data output.
 *
 * This function sends the RDATAC opcode, which makes conversion data immediately available
 * to the host as soon as the DRDY pin goes low. The host need only send SCLKs to retrieve
 * the data, rather than starting with a RDATA command. Registers cannot be read or written
 * (RREG or WREG) in this mode. Cancel continuous read mode using ads1299_stop_rdatac().
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \return Zero if successful, or an error code if unsuccessful.
 */
static inline ads1299_error_t ads1299_start_rdatac(uint8_t chip_select)
{
	//#if UC3
	ads1299_send_byte(chip_select, ADS1299_OPC_RDATAC);
	
	return ADS1299_STATUS_OK;
	//#else
	//#endif
}

/**
 *	\brief Disable continuous data output.
 *
 * This function sends the SDATAC opcode, which exits the continuous data mode.
 *
 * \pre Requires spi.h from the Atmel Software Framework and ads1299_spi_adapt.h.
 * \param chip_select The chip select number of the ADS1299 to be initialized.
 * \return Zero if successful, or an error code if unsuccessful.
 */
static inline ads1299_error_t ads1299_stop_rdatac(uint8_t chip_select)
{
	//#if UC3
	ads1299_send_byte(chip_select, ADS1299_OPC_SDATAC);
	
	return ADS1299_STATUS_OK;
	//#else
	//#endif
}

#endif // #ifndef _ADS1299_H
