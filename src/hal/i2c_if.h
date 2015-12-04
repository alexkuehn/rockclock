/*
 * This file is part of ROCKCLOCK firmware.
 *
 * Copyright (c) 2015 Alexander Kühn <prj@alexkuehn.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRC_HAL_I2C_IF_H_
#define SRC_HAL_I2C_IF_H_

/* external standard includes */

/* external includes */

/* project includes */

/* component includes */
#include "i2c_config.h"

/** @brief blocking receive of I2C bus data
 *
 * receive data from I2C device
 *
 * @param[in] busaddr device adress on i2c bus with 7bit
 * @param[in] reg register adress of device
 * @param[out] data pointer to memory, where read values should be written
 * @param[in] length length of data block to be read
 */
extern void i2c_receive_blocking( uint8_t busaddr, uint8_t reg, uint8_t *data, uint8_t length);

/** @brief blocking transmit of I2C bus data
 *
 * transmit data to I2C device
 *
 * @param[in] busaddr device adress on i2c bus with 7bit
 * @param[in] reg register adress of device
 * @param[in] data pointer to memory, where values should be transferred
 * @param[in] length length of data block to be written
 */
extern void i2c_transmit_blocking( uint8_t busaddr, uint8_t reg, uint8_t *data, uint8_t length);

#endif /* SRC_HAL_I2C_IF_H_ */
