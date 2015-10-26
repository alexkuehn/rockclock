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

#ifndef SRC_HAL_WS2812_IF_H_
#define SRC_HAL_WS2812_IF_H_

/* external standard includes */
#include <stdint.h>

/* external includes */

/* project includes */

/* component includes */
#include "ws2812_config.h"
#include "ws2812.h"


/** @brief get LED update process running
 *
 * get the state of the LED updating process
 *
 * @return Boolean flag of running LED update process
 */
extern uint8_t ws2812_get_updating( void );

/** @brief mark the LED framebuffer for updating
 *
 * after the framebuffer is written, the new value needs to be transferred. This is trigerred by
 * emitting the update command.
 */
extern void ws2812_update( void );

/** @brief set the LED framebuffer pixel
 *
 * sets the pixel value in the framebuffer
 *
 * @param[in] row row of the LED framebuffer, row is an port channel
 * @param[in] col column of the LED framebuffer, columns are positions within a chain
 * @param[in] red red pixel value
 * @param[in] green green pixel value
 * @param[in] blue blue pixel value
 */
extern void ws2812_set_pixel( uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue );

/** @brief clear the LED framebuffer
 *
 * clears all pixel values within the LED framebuffer
 */
extern void ws2812_clear( void );

#endif /* SRC_HAL_WS2812_IF_H_ */
