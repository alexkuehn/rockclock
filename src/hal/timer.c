/*
 * timer.c
 *
 *  Created on: 06.08.2015
 *      Author: alex
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
