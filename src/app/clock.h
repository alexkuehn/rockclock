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

#ifndef SRC_APP_CLOCK_H_
#define SRC_APP_CLOCK_H_

/* external standard includes */

/* external includes */

/* project includes */

/* component includes */


typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} pixel_t;

typedef struct {
	uint32_t r;
	uint32_t g;
	uint32_t b;
} hires_pixel_t;

typedef struct {
	int16_t rdiff;
	int16_t gdiff;
	int16_t bdiff;
} pixeldiff_t;

#define CLOCK_ELEMENTS 60     /**< number of clock pixel elements */

/** @brief update framebuffer transition
 */
extern void clock_fb_update( void );

/** @brief clear clock internal framebuffer
 */
extern void clock_clear_fb( void );

/** @brief set pixel in internal framebuffer
 */
extern void clock_set_pixel( uint8_t pos, uint8_t red, uint8_t green, uint8_t blue );

/** @brief increment internal clock by second
 */
extern void clock_tick( void );

/** @brief display the clock on WS2812 ring
 */
extern void clock_display( void);

#endif /* SRC_APP_CLOCK_H_ */
