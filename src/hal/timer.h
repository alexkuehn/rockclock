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

#ifndef SRC_HAL_TIMER_H_
#define SRC_HAL_TIMER_H_

/* external standard includes */

/* external includes */

/* project includes */

/* component includes */


#define HZ (1000)	/**< timer tick frequency in [Hz] */

/** @brief initialize the system timer component
 *
 */
extern void timer_init( void );


#endif /* SRC_HAL_TIMER_H_ */
