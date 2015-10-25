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

#include <stdint.h>

#include "ws2812_config.h"
#include "ws2812.h"

extern uint8_t ws2812_get_updating( void );
extern void ws2812_update( void );

extern void ws2812_set_pixel( uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue );
extern void ws2812_clear( void );

#endif /* SRC_HAL_WS2812_IF_H_ */
