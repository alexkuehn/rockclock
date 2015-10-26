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

#ifndef SRC_HAL_IO_CONFIG_H_
#define SRC_HAL_IO_CONFIG_H_

/* external standard includes */

/* external includes */
#include <libopencm3/stm32/gpio.h>

/* project includes */

/* component includes */

/* --> port and pin definitions */
#define LED_BLUE_PORT 	GPIOC
#define LED_BLUE_PIN  	GPIO8

#define HC05_CMD_PORT	GPIOA
#define HC05_CMD_PIN	GPIO1

#define DCF_IN_PORT		GPIOC
#define DCF_IN_PIN		GPIO0
/* <-- port and pin definitions */

#endif /* SRC_HAL_IO_CONFIG_H_ */
