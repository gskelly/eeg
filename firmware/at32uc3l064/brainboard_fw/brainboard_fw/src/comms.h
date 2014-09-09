/** 
 *  \file comms.h
 *  \brief Defines channel counts, communication packets, and data rates for Brainboard.
 *	\author Graham Kelly
 *	\version 1.0
 *	\date August 2014
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

#ifndef _COMMS_H_
#define _COMMS_H_

#include <compiler.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_ADDRESS						(&AVR32_SPI)
#define DUMMY_BYTE						0x00

#define SPI_ADS1299_MAIN_CHIPNUM		0
#define SPI_ADS1299_DAISY_CHIPNUM		2

/* For primary USART communicating with RN-42 Bluetooth module */
#define USARTBT_MODULE					(&AVR32_USART3)
#define USARTBT_DEFAULT_BAUDRATE		115200
/* For secondary USART communicating with Beaglebone (if applicable; R0 and R1 only) */
#define USARTUC_MODULE					(&AVR32_USART2)
#define USARTUC_BAUDRATE				921600
#define I2C_MODULE						(&AVR32_TWIM0)

/* Define which USART is used for data streaming and which is used for auxiliary communications
 * (R0 and R1 only) 
 */
#define DATA_USART						USARTBT_MODULE
//#define DATA_USART					USARTUC_MODULE
#define AUX_USART						USARTBT_MODULE
//#define AUX_USART						USARTUC_MODULE

/* DMA/PDCA definitions */
#define DATA_USART_TX_PDCA_CHANNEL		0
#define DATA_USART_TX_PDCA_PID			AVR32_PDCA_PID_USART3_TX
#define DATA_USART_RX_PDCA_CHANNEL		1
#define DATA_USART_RX_PDCA_PID			AVR32_PDCA_PID_USART3_RX

/**
 *  \brief Defines the size of the signal portion of transmitted data packets.
 */
#define DATA_USART_TX_BUFFER_SIZE			1

/**
 *  \brief Defines the maximum number of EEG input channels available.
 *
 *	The maximum number of channels for a single ADS1299 is 8. MAX_CHANNELS may be decreased if this many channels 
 *  are not desired. Increasing the number to support multiple ADCs is not currently supported.
 */
#define MAX_EEG_CHANNELS			8

/**
 *  \brief Defines the maximum number of degrees of freedom from the inertial measurement chip.
 *
 *	The maximum number of channels supported for a single chip is 6 (3 acceleration and 3 rotation).
 *  Increasing the number to support multiple IMUs is not currently supported.
 */
#define MAX_IMU_DOF					6


/**
 *	\brief Mouse HID report structure for the RN-42.
 *
 *  Allows the Brainboard to be used as a USB mouse.
 */
 typedef struct
 {
	uint8_t header;
	uint8_t length;
	uint8_t descriptor;
	uint8_t buttons;			// Bitfield of some kind--need to test
	int8_t dx;
	int8_t dy;
	int8_t wheel;
} __attribute__((packed)) mouse_hid_report_t;

/**
 *	\brief PADDED Brainboard packet structure for communicating 24-bit EEG and inertial data.
 *
 *  This packet type pre-pads 24-bit data to 32 bits. Easy to work with but least efficient for transmission.
 */
typedef struct  
{
	uint16_t start;
	uint8_t packetnum;
	volatile uint32_t eegstatus;
	volatile int32_t eegdata[DATA_USART_TX_BUFFER_SIZE][MAX_EEG_CHANNELS];
	volatile int16_t imudata[DATA_USART_TX_BUFFER_SIZE][MAX_IMU_DOF];
} __attribute__((packed)) bboard_data32bit_packet_t;

/**
 *	\brief PACKED Brainboard packet structure for communicating 24-bit EEG and inertial data.
 *
 *  This packet type stores EEG data as raw bytes. Most compact.
 */
typedef struct  
{
	uint16_t start;
	uint8_t packetnum;
	volatile uint8_t eegstatus[3];
	volatile uint8_t eegdata[DATA_USART_TX_BUFFER_SIZE][MAX_EEG_CHANNELS][3];
	volatile int16_t imudata[DATA_USART_TX_BUFFER_SIZE][MAX_IMU_DOF];
} __attribute__((packed)) bboard_data24bit_packet_t;

/**
 *	\brief Brainboard packet structure for communicating 16-bit EEG and inertial data.
 *
 *  This packet type truncates the EEG signal data, which trades packet size for precision. May lose information this way.
 */
typedef struct  
{
	uint16_t start;
	uint8_t packetnum;
	volatile uint32_t eegstatus;
	volatile int16_t eegdata[DATA_USART_TX_BUFFER_SIZE][MAX_EEG_CHANNELS];
	volatile int16_t imudata[DATA_USART_TX_BUFFER_SIZE][MAX_IMU_DOF];
} __attribute__((packed)) bboard_data16bit_packet_t;

// #define data_packet_t					bboard_data32bit_packet_t
// #define TX_PACKET_SIZE	(7+DATA_USART_TX_BUFFER_SIZE*(4*MAX_EEG_CHANNELS+2*MAX_IMU_DOF))

#define data_packet_t				bboard_data24bit_packet_t
#define TX_PACKET_SIZE	(6+DATA_USART_TX_BUFFER_SIZE*(3*MAX_EEG_CHANNELS+2*MAX_IMU_DOF))

// #define data_packet_t				bboard_data16bit_packet_t
// #define TX_PACKET_SIZE	(7+DATA_USART_TX_BUFFER_SIZE*(2*MAX_EEG_CHANNELS+2*MAX_IMU_DOF))


/**
 * \brief Compile-time assertion macros for checking packet struct sizes.
 *	
 *        ct_assert will give a negative array size error if it evaluates false.
 */
#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
#define ct_assert(e) extern char (*ct_assert(void)) [sizeof(char[1 - 2*!(e)])]

/* If this line gives a compile-time error, byte alignment/packing differs from the intended
 * implementation on the selected platform and may cause runtime errors.
 */
ct_assert(sizeof(data_packet_t) == TX_PACKET_SIZE);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _COMMS_H_ */