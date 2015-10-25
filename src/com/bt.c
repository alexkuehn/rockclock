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

#include <stdint.h>

#include "../os/os_if.h"
#include "../hal/io_if.h"
#include "../hal/usart_if.h"

#include "bt_if.h"
#include "bt.h"

static const uint32_t baudrates[] = {115200, 9600, 38400};

static const uint8_t BTCMD_AT[] = "AT\r\n";
#define  BTCMD_AT_LEN 4
static const uint8_t BTCMD_SET_NAME[] = "AT+NAME=ROCKCLOCK\r\n";
#define BTCMD_SET_NAME_LEN 19

static const uint8_t BTCMD_SET_PW[] = "AT+PSWD=4223\r\n";
#define BTCMD_SET_PW_LEN 14

static const uint8_t BTCMD_SET_BAUD[] = "AT+UART=115200,0,0\r\n";
#define BTCMD_SET_BAUD_LEN 20

static const uint8_t BTCMD_RESET[] = "AT+RESET\r\n";
#define BTCMD_RESET_LEN 10

#define RESPONSE_LENGTH 32
static uint8_t response_buffer[RESPONSE_LENGTH];

#define RESPONSE_TIMEOUT 200 /* ms */

static uint8_t bt_ready = 0;
void bt_init( void )
{
	uint8_t num_baudrates = sizeof(baudrates)/sizeof(uint32_t);
	uint8_t i = 0;
	int16_t received;

	/* first set BT module AT mode pin */
	io_on( HC05_CMD_PORT, HC05_CMD_PIN);
	/* now wait 100ms */
	delay(100);

	/* iterate through baudrates */
	do
	{
		usart_config_baudrate( baudrates[i]);
		usart_flush();
		/* send AT command */
		usart_transmit( (uint8_t *)BTCMD_AT, BTCMD_AT_LEN);
		/* wait for response */
		received = usart_receive( response_buffer, 4, RESPONSE_TIMEOUT);

		/* we found a valid answer */
		if( received > 0)
		{
			break;
		}
		i++;
	}while(i < num_baudrates);

	if( i > 0)
	{
		/* module seems to be configured for default -> reprogram */
		usart_transmit( (uint8_t *)BTCMD_SET_NAME, BTCMD_SET_NAME_LEN);
		received = usart_receive( response_buffer, RESPONSE_LENGTH, RESPONSE_TIMEOUT);
		/* module seems to be configured for default -> reprogram */
		usart_transmit( (uint8_t *)BTCMD_SET_PW, BTCMD_SET_PW_LEN);
		received = usart_receive( response_buffer, RESPONSE_LENGTH, RESPONSE_TIMEOUT);
		/* module seems to be configured for default -> reprogram */
		usart_transmit( (uint8_t *)BTCMD_SET_BAUD, BTCMD_SET_BAUD_LEN);
		received = usart_receive( response_buffer, RESPONSE_LENGTH, RESPONSE_TIMEOUT);
	}


	/* send reset */
	usart_transmit( (uint8_t *)BTCMD_RESET, BTCMD_RESET_LEN);
	received = usart_receive( response_buffer, RESPONSE_LENGTH, RESPONSE_TIMEOUT);
	/* leave AT mode */
	io_off( HC05_CMD_PORT, HC05_CMD_PIN);
	/* wait a bit */
	delay( 100);
	usart_config_baudrate( baudrates[0]);
	usart_flush();

	bt_ready = 1;

}

uint8_t bt_is_ready( void )
{
	return bt_ready;
}
