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

#include "timer.h"

#include "../hal/cpu_if.h"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>

static volatile uint32_t timer_tick;

void timer_init( void )
{
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	STK_CVR=0;
	systick_set_reload(rcc_ahb_frequency / HZ);
    systick_counter_enable();
	systick_interrupt_enable();
}

uint32_t timer_get( void )
{
  return timer_tick;
}

/* system timer signal */
void sys_tick_handler( void )
{
    timer_tick++;
}
