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
#include "../hal/usart_if.h"
#include "../hal/ws2812_if.h"
#include "../com/bt_if.h"
#include "../hal/dcf_if.h"
#include "../app/clock_if.h"

/* component includes */
#include "app_config.h"



void app_init(void)
{
	bt_init();
}

void clock_process( void )
{

	clock_update();

}

void loop_process(void)
{

}

