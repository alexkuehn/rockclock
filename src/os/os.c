/*
 * os.c
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#include "../hal/cpu_if.h"

void os_init( void )
{
  scheduler_init();

  /* start the interrupt system */
  cpu_enable_int();
}

