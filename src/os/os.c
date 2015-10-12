/*
 * os.c
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#include "../hal/cpu_if.h"
#include "../hal/timer_if.h"
void os_init( void )
{
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

