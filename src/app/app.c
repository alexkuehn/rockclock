/*
 * app.c
 *
 *  Created on: 07.08.2015
 *      Author: alex
 */

#include "../hal/usart_if.h"
#include "../hal/ws2812_if.h"
#include "../com/bt_if.h"
#include "../hal/dcf_if.h"

#include "app_config.h"

#include "../app/clock_if.h"


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

