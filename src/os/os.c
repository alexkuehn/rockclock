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
#include "../hal/cpu_if.h"
#include "../hal/timer_if.h"
#include "../os/scheduler.h"

/* component includes */

void os_init( void )
{
	/* initialize the os subcomponents */
	scheduler_init();

	/* start the interrupt system */
	cpu_enable_int();
}

void delay( uint32_t millis)
{
	uint32_t ringtime;
	ringtime = timer_get() + millis;
	while( ringtime > timer_get());
}

