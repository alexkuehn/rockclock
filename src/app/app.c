/*
 * app.c
 *
 *  Created on: 07.08.2015
 *      Author: alex
 */

#include "../hal/usart_if.h"
#include "../hal/io_if.h"

const uint8_t str[] ="Hallo Susanne, ich liebe Dich!";

void app_init(void)
{

}


void blink_process( void )
{
	io_toggle( LED_BLUE_PORT, LED_BLUE_PIN);
}

void hello_process( void )
{
	uint8_t j;

	j = 0;
	while( str[j] != '\0')
	{
		usart_transmit(str[j]);
		j++;
	}
	usart_transmit('\r');
	usart_transmit('\n');
}
