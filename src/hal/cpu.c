/*
 * cpu.c
 *
 *  Created on: 06.08.2015
 *      Author: alex
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
