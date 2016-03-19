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

#ifndef SRC_APP_CLOCK_IF_H_
#define SRC_APP_CLOCK_IF_H_

/* external standard includes */
#include <stdint.h>

/* external includes */

/* project includes */

/* component includes */
#include "clock_config.h"

/** @brief clock time structure
 *
 */
typedef struct
{
	uint8_t h;		/**< hours */
	uint8_t m;		/**< minutes */
	uint8_t s;		/**< seconds */
} clock_t;


/** @brief update the clock value
 *
 * needs to be called within a exact seond
 */
extern void clock_update( void );

/** @brief set the clock time manually
 *
 * @param[in] h Hours
 * @param[in] m Minutes
 * @param[in] s Seconds
 */
extern void clock_set( uint8_t h, uint8_t m, uint8_t s);

/** @brief rotate the clock display
 *
 * @param[in] pixel Pixelvalue without offset
 * @param[out] rotpixel Rotated Pixel value
 */
#define DISP_ROT(x) ( ((x)+CFG_CLOCK_DISP_ROTATE) % 60 )

#endif /* SRC_APP_CLOCK_IF_H_ */
