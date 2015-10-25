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

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/cortex.h>

void cpu_init( void )
{
	/* initialize the clock */
	rcc_clock_setup_in_hsi_out_48mhz();
}

void cpu_disable_int( void )
{
	cm_disable_interrupts();
}

void cpu_enable_int( void )
{
	cm_enable_interrupts();
}
