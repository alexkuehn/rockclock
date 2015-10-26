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

#ifndef SRC_HAL_WS2812_H_
#define SRC_HAL_WS2812_H_

/* external standard includes */

/* external includes */

/* project includes */

/* component includes */
#include "ws2812_config.h"


#define NR_OF_ROWS (8)		/**< number of LED row chains */

#define BITS_PER_LED 24		/**< number of bits coded per LED */
#define WS2812_BUFFERSIZE (NR_OF_LEDS_PER_CH*BITS_PER_LED)	/* size of LED unrolled buffer */


#define NR_OF_WAITCOUNTS (18)	/**< count for timer overflows to define WS2812 sync pulse length */

/** @brief initialize WS2812 subcomponent
 *
 */
extern void ws2812_init( void );

/** @brief start the WS2812 protocol transfer process
 *
 */
extern void ws2812_send( void );

#endif /* SRC_HAL_WS2812_H_ */
