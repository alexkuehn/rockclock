/*
 * hal.c
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#include "cpu.h"
#include "io.h"
#include "timer.h"
#include "usart.h"
#include "ws2812.h"

void hal_init( void )
{
  cpu_init();
  io_init();
  usart_init();
  timer_init();
  ws2812_init();
}
