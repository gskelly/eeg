/** 
 *  \file brainboard.h
 *  \brief Board-specific definitions for current Brainboard revision.
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

#ifndef _BRAINBOARD_H_
#define _BRAINBOARD_H_

#include <compiler.h>

#define BRAINBOARD_REV					2	

/* For the below values, clocks are derived from the uC's digital frequency-locked loop (DFLL),
 * divided by various clock dividers */
#define FCPU_HZ							(DFLL_FREQ_HZ >> CONFIG_SYSCLK_CPU_DIV) // CPU frequency in Hz
#define FPBA_HZ							(DFLL_FREQ_HZ >> CONFIG_SYSCLK_PBA_DIV) // Peripheral Bus A freq in Hz
#define FPBB_HZ							(DFLL_FREQ_HZ >> CONFIG_SYSCLK_PBB_DIV) // Peripheral Bus B freq in Hz

/*************************************************************************************************************************************************
 *               Host Pin Config Definitions                                                                                                     *
 *************************************************************************************************************************************************/

#if (BRAINBOARD_REV == 0)

/**
 *  \brief Defines the host processor pin assigned to the RESET pin on the ADS1299.
 */
#define ADS1299_PIN_RESET		AVR32_PIN_PB04

/**
 *  \brief Defines the host processor pin assigned to the START pin on the ADS1299.
 */
#define ADS1299_PIN_START		AVR32_PIN_PA17

#endif

/**
 *  \brief Defines the host processor pin assigned to the DRDY pin on the ADS1299.
 */
#define ADS1299_PIN_DRDY		AVR32_PIN_PA18
#define ADS1299_DRDY_INT		5
#define ADS1299_DRDY_PRIORITY	AVR32_INTC_INT3
#define ADS1299_DRDY_IRQ		AVR32_EIC_IRQ_5
#define ADS1299_DRDY_INT_FN		AVR32_EIC_EXTINT_5_0_FUNCTION	

/**
 *  \brief Defines the host processor pin assigned to the PWDN pin on the ADS1299.
 */
#define ADS1299_PIN_PWDN		AVR32_PIN_PA09	

/**
 *  \brief Defines the host processor pin assigned to the RESET pin on the RN-42.
 */
#define BT_RESET_PIN			AVR32_PIN_PB11

/**
 *  \brief Defines the host processor pin assigned to the INT pin on the MPU-6050.
 */
#if (BRAINBOARD_REV == 2)
#define MPU6050_INT_PIN			AVR32_PIN_PA07
#elif (BRAINBOARD_REV < 2)
#define MPU6050_INT_PIN			AVR32_PIN_PA10
#endif

#endif /* #ifndef _BRAINBOARD_H_ */