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

/* external standard includes */

/* external includes */

/* project includes */
#include "cpu.h"
#include "io.h"
#include "timer.h"
#include "usart.h"
#include "ws2812.h"
#include "dcf_if.h"
#include "i2c.h"

/* component includes */


void hal_init( void )
{
	/* initialize the HAL subcomponents */
	cpu_init();
	io_init();
	usart_init();
	timer_init();
	dcf_init();
	ws2812_init();
	i2c_init();
}
