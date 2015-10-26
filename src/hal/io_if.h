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

#ifndef SRC_HAL_IO_IF_H_
#define SRC_HAL_IO_IF_H_

/* external standard includes */

/* external includes */

/* project includes */

/* component includes */
#include "io_config.h"


/** @brief toggle IO pin
 *
 * toggles the output of an IO pin
 *
 * @param[in] port GPIO port
 *
 * @param[in] pos GPIO pin position
 */
extern void io_toggle( uint32_t port, uint16_t pos);

/** @brief switch IO pin on
 *
 * switches the output of an IO pin to state ON
 *
 * @param[in] port GPIO port
 *
 * @param[in] pos GPIO pin position
 */
extern void io_on( uint32_t port, uint16_t pos);

/** @brief switch IO pin off
 *
 * switches the output of an IO pin to state OFF
 *
 * @param[in] port GPIO port
 *
 * @param[in] pos GPIO pin position
 */
extern void io_off( uint32_t port, uint16_t pos);

/** @brief get IO pin state
 *
 * gets the state of an IO pin
 *
 * @return Boolean state of an IO pin
 */
extern uint8_t io_get( uint32_t port, uint16_t pos);

#endif /* SRC_HAL_IO_IF_H_ */
