/**
 *  \file mpu60x0.c
 *  \brief Configuration settings, register definitions, and function prototypes for using the MPU-60X0 inertial sensors from InvenSense.
 *	\author Graham Kelly
 *	\version 1.0
 *	\date August 2014
 *	
 *  Header file mpu60x0.h borrows heavily from the MPU-6050 I2Cdev library by
 *  Jeff Rowberg, released under the MIT license.
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

#include "mpu60x0.h"

uint8_t mpu6050_tx_buffer[1];
uint8_t mpu6050_rx_buffer[14];

status_code_t mpu6050_initialize_normal(uint32_t saddr)
{	
	status_code_t status;	
	twim_package_t init_package =
	{
		.chip			= MPU6050_DEFAULT_ADDRESS,
		.addr_length	= 1,
		.buffer			= (void *)mpu6050_tx_buffer,
		.length			= 1
	};
	
	status = STATUS_OK;
	
	/* Disable temperature sensor, source the MPU6050's clock from the X gyro oscillator, leave SLEEP */
	init_package.addr[0] = MPU6050_RA_PWR_MGMT_1;
	mpu6050_tx_buffer[0] = (	(1 << MPU6050_PWR1_TEMP_DIS_BIT)
							|	(0 << MPU6050_PWR1_SLEEP_BIT) 
							|         MPU6050_CLOCK_PLL_XGYRO    );
	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	/* Sanity check: read back that register */
// 	twim_read_packet(I2C_MODULE, (const twim_package_t*)&verify_package);
	
	/* Set desired sample rate */
	init_package.addr[0] = MPU6050_RA_SMPLRT_DIV;
	mpu6050_tx_buffer[0] = (uint8_t)((MPU6050_GYRO_OUTPUT_RATE_HZ/MPU6050_SAMPLING_RATE_HZ)-1);
	status = twim_write_packet(I2C_MODULE, (const twim_package_t*)&init_package);
	
	/* Configure LPF for 256 Hz bandwidth */
	init_package.addr[0] = MPU6050_RA_CONFIG;
	mpu6050_tx_buffer[0] = MPU6050_DLPF_BW;
	status = twim_write_packet(I2C_MODULE, (const twim_package_t*)&init_package);
	
	/* Gyro full-scale at +/-250 deg/s */
	init_package.addr[0] = MPU6050_RA_GYRO_CONFIG;
	mpu6050_tx_buffer[0] = (MPU6050_GYRO_FS_250 << MPU6050_GCONFIG_FS_SEL_BIT);
	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	/* Accelerometer full-scale at +/-2 g */
	init_package.addr[0] = MPU6050_RA_ACCEL_CONFIG;
	mpu6050_tx_buffer[0] = (MPU6050_ACCEL_FS_2 << MPU6050_ACONFIG_AFS_SEL_BIT);
	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	/* INT pin active high, push-pull, latched until any register read; FSYNC disabled */
	init_package.addr[0] = MPU6050_RA_INT_PIN_CFG;
	mpu6050_tx_buffer[0] = (  (MPU6050_INTMODE_ACTIVEHIGH << MPU6050_INTCFG_INT_LEVEL_BIT   )	
							| (MPU6050_INTDRV_PUSHPULL    << MPU6050_INTCFG_INT_OPEN_BIT    )
							| (MPU6050_INTLATCH_WAITCLEAR << MPU6050_INTCFG_LATCH_INT_EN_BIT)
							| (MPU6050_INTCLEAR_ANYREAD   << MPU6050_INTCFG_INT_RD_CLEAR_BIT)  );
	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	/* Enable FIFO for X,Y,Z gyro and accelerometers, disable for temperature */
	/* Data registers are arranged as ACCEL[X,Y,Z], TEMP, GYRO[X,Y,Z] with 2 bytes each (14 total).
	 * They are loaded into the FIFO in that order, from ACCEL_XOUT to GYRO_ZOUT.
	 */
// 	init_package.addr[0] = MPU6050_RA_FIFO_EN;
// 	mpu6050_tx_buffer[0] = (   (1 << MPU6050_XG_FIFO_EN_BIT)
// 							|  (1 << MPU6050_YG_FIFO_EN_BIT)
// 							|  (1 << MPU6050_ZG_FIFO_EN_BIT)
// 							|  (1 << MPU6050_ACCEL_FIFO_EN_BIT) 
// 							|  (0 << MPU6050_TEMP_FIFO_EN_BIT)  );
// 	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	/* Enable FIFO operations */
// 	init_package.addr[0] = MPU6050_RA_USER_CTRL;
// 	mpu6050_tx_buffer[0] = 0x02;
// 	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
// 	mpu6050_tx_buffer[0] = (1 << MPU6050_USERCTRL_FIFO_EN_BIT);
// 	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	/* Enable INT pin assertion upon entering DATA_RDY state */
	init_package.addr[0] = MPU6050_RA_INT_ENABLE;
	mpu6050_tx_buffer[0] = (1 << MPU6050_INTERRUPT_DATA_RDY_BIT);
	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	if (status != STATUS_OK) {
		return ERR_IO_ERROR;
	}
	return STATUS_OK;
}



status_code_t mpu6050_initialize_lowpower(uint32_t saddr)
{	
	status_code_t status;	
	twim_package_t init_package =
	{
		.chip			= MPU6050_DEFAULT_ADDRESS,
		.addr_length	= 1,
		.buffer			= (void *)mpu6050_tx_buffer,
		.length			= 1
	};
	
	status = STATUS_OK;
	
	/* Disable temperature sensor, enter cycle mode:
	 * wakeup at fixed interval to read accelerometer only
	 */
	/* CORRECTION: you may enable cycle mode, but it would be good to code a
	 * separate interrupt for the MPU-6050 in this case: tend to lose samples
	 * otherwise. No cycle mode for now.
	 */
	init_package.addr[0] = MPU6050_RA_PWR_MGMT_1;
	mpu6050_tx_buffer[0] = (	(1 << MPU6050_PWR1_TEMP_DIS_BIT)
							//|	(1 << MPU6050_PWR1_CYCLE_BIT)	
							|	(0 << MPU6050_PWR1_SLEEP_BIT));
	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	/* Put gyros in standby, set wakeup rate to 10 Hz */
	init_package.addr[0] = MPU6050_RA_PWR_MGMT_2;
	mpu6050_tx_buffer[0] = (	(MPU6050_WAKE_FREQ_10 <<  6)
							|   (1 << MPU6050_PWR2_STBY_XG_BIT)
							|	(1 << MPU6050_PWR2_STBY_YG_BIT)
							|   (1 << MPU6050_PWR2_STBY_ZG_BIT));
	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);							
	
	/* Sanity check: read back that register */
// 	twim_read_packet(I2C_MODULE, (const twim_package_t*)&verify_package);
	
	/* Set desired sample rate */
	init_package.addr[0] = MPU6050_RA_SMPLRT_DIV;
	mpu6050_tx_buffer[0] = (uint8_t)((MPU6050_GYRO_OUTPUT_RATE_HZ/MPU6050_SAMPLING_RATE_HZ)-1);
	status = twim_write_packet(I2C_MODULE, (const twim_package_t*)&init_package);
	
	/* Configure LPF for 256 Hz bandwidth */
	init_package.addr[0] = MPU6050_RA_CONFIG;
	mpu6050_tx_buffer[0] = MPU6050_DLPF_BW;
	status = twim_write_packet(I2C_MODULE, (const twim_package_t*)&init_package);
	
	/* Accelerometer full-scale at +/-2 g */
	init_package.addr[0] = MPU6050_RA_ACCEL_CONFIG;
	mpu6050_tx_buffer[0] = (MPU6050_ACCEL_FS_2 << MPU6050_ACONFIG_AFS_SEL_BIT);
	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	/* INT pin active high, push-pull, latched until any register read; FSYNC disabled */
	init_package.addr[0] = MPU6050_RA_INT_PIN_CFG;
	mpu6050_tx_buffer[0] = (  (MPU6050_INTMODE_ACTIVEHIGH << MPU6050_INTCFG_INT_LEVEL_BIT   )	
							| (MPU6050_INTDRV_PUSHPULL    << MPU6050_INTCFG_INT_OPEN_BIT    )
							| (MPU6050_INTLATCH_WAITCLEAR << MPU6050_INTCFG_LATCH_INT_EN_BIT)
							| (MPU6050_INTCLEAR_ANYREAD   << MPU6050_INTCFG_INT_RD_CLEAR_BIT)  );
	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	/* Enable FIFO for X,Y,Z gyro and accelerometers, disable for temperature */
	/* Data registers are arranged as ACCEL[X,Y,Z], TEMP, GYRO[X,Y,Z] with 2 bytes each (14 total).
	 * They are loaded into the FIFO in that order, from ACCEL_XOUT to GYRO_ZOUT.
	 */
// 	init_package.addr[0] = MPU6050_RA_FIFO_EN;
// 	mpu6050_tx_buffer[0] = (   (1 << MPU6050_XG_FIFO_EN_BIT)
// 							|  (1 << MPU6050_YG_FIFO_EN_BIT)
// 							|  (1 << MPU6050_ZG_FIFO_EN_BIT)
// 							|  (1 << MPU6050_ACCEL_FIFO_EN_BIT) 
// 							|  (0 << MPU6050_TEMP_FIFO_EN_BIT)  );
// 	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	/* Enable FIFO operations */
// 	init_package.addr[0] = MPU6050_RA_USER_CTRL;
// 	mpu6050_tx_buffer[0] = 0x02;
// 	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
// 	mpu6050_tx_buffer[0] = (1 << MPU6050_USERCTRL_FIFO_EN_BIT);
// 	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	/* Enable INT pin assertion upon entering DATA_RDY state */
	init_package.addr[0] = MPU6050_RA_INT_ENABLE;
	mpu6050_tx_buffer[0] = (1 << MPU6050_INTERRUPT_DATA_RDY_BIT);
	status = twim_write_packet(I2C_MODULE, (const twim_package_t *)&init_package);
	
	if (status != STATUS_OK) {
		return ERR_IO_ERROR;
	}
	return STATUS_OK;
}

status_code_t mpu6050_rdata16_packet(uint32_t saddr, volatile uint32_t sample_idx, data_packet_t* packet)
{
	status_code_t status;
	int16_t data[7];
	const twim_package_t twim_package =
	{
		.chip			= saddr,
		.addr[0]		= MPU6050_RA_ACCEL_XOUT_H,
		.addr_length	= 1,
		.buffer			= (void *)data,
		.length			= 14
	};
	
	status = twim_read_packet(I2C_MODULE, &twim_package);
	/* Accelerometer data */
	packet->imudata[sample_idx][0] = data[0];
	packet->imudata[sample_idx][1] = data[1];
	packet->imudata[sample_idx][2] = data[2];
	/* Skip temperature data */
	/* Gyro data */
	packet->imudata[sample_idx][3] = data[4];
	packet->imudata[sample_idx][4] = data[5];
	packet->imudata[sample_idx][5] = data[6];
	
	return status;
}

status_code_t mpu6050_rdata16_fifo(uint32_t saddr, volatile uint32_t sample_idx, data_packet_t* packet)
{
	status_code_t status;
	uint8_t fifo_count_lo, fifo_count_hi;
	uint16_t fifo_count;
	twim_package_t twim_package =
	{
		.chip			= saddr,
		.addr[0]		= MPU6050_RA_FIFO_COUNTH,
		.addr_length	= 1,
		.buffer			= (void *)&fifo_count_hi,
		.length			= 1
	};

	fifo_count_hi = 0;
	fifo_count_lo = 0;
	
	/* First check how many samples are in the FIFO */
	/* TWIM packet was initialized to read the FIFO_COUNT high byte first, so read now */
	status = twim_read_packet(I2C_MODULE, (const twim_package_t *)&twim_package);
	/* Now read the low byte */
	twim_package.buffer = (void *)&fifo_count_lo;
	status = twim_read_packet(I2C_MODULE, (const twim_package_t *)&twim_package);
	/* Concatenate */
	fifo_count = fifo_count_lo | ((uint16_t)fifo_count_hi << 8);
	/* Read the FIFO if there's enough data */
	if (fifo_count >= 2*MAX_IMU_DOF) {
		twim_package.addr[0] = MPU6050_RA_FIFO_R_W;
		twim_package.buffer = (void *)packet->imudata[sample_idx];
		twim_package.length = 2*MAX_IMU_DOF;
		status = twim_read_packet(I2C_MODULE, (const twim_package_t *)&twim_package);
	}
	else return ERR_BUSY;
	return status;
}
