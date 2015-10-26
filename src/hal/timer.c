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
#include <stdint.h>

/* external includes */
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>

/* project includes */
#include "../hal/cpu_if.h"

/* component includes */
#include "timer.h"

static volatile uint32_t timer_tick;	/**< actual timer tick value */

void timer_init( void )
{
	/* configure system tick timer */
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

/** @brief SysTick callback
 *
 * callback for SysTick timer
 */
void sys_tick_handler( void )
{
    timer_tick++;
}
