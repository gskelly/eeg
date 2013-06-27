/** \file ads1299_spi_adapt.h
 *  \brief Compatibility interface for communications drivers between different host hardware implementations.
 *	\author Graham Kelly
 *	\version 0.1
 *	\date June 2013
 *	\copyright
 */

#ifndef ADS1299_COMMS_ADAPT_H_
#define ADS1299_COMMS_ADAPT_H_

#include "ads1299_host_processor.h"

#if UC3

#include <spi.h>
#include <usart.h>

#define SPI_ADDRESS			(&AVR32_SPI)
#define DUMMY_BYTE			0x00

#define USART_MODULE		(&AVR32_USART2)
#define USART_BAUDRATE		115200

#else
/* #ifdef WHATEVER ... */

#endif /* #if UC3 */

/* Convenience macros for extracting bytes from larger types for sending data over SPI/USART */
/* Careful with these: remember byte 0 is leftmost byte, and don't use GET_BYTE_2 or GET_BYTE_3 on 16-bit ints */
#define GET_BYTE_0(a)		* ((uint8_t *)&a)
#define GET_BYTE_1(a)		*(((uint8_t *)&a) + 1);
#define GET_BYTE_2(a)		*(((uint8_t *)&a) + 2);
#define GET_BYTE_3(a)		*(((uint8_t *)&a) + 3);

#endif /* #ifndef ADS1299_COMMS_ADAPT_H_ */