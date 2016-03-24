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
#include <stdbool.h>

/* external includes */

/* project includes */
#include "../hal/usart_if.h"
#include "../hal/ws2812_if.h"
#include "../com/bt_if.h"
#include "../app/clock_if.h"
#include "../hal/timer_if.h"
#include "../hal/i2c_if.h"

/* component includes */
#include "remote_config.h"
#include "remote.h"

static remote_state_t remotestate;
static remote_slave_state_t remoteslavestate;
static uint8_t protocolbuffer;
static uint32_t timeouttime;
static uint8_t arm_timeout;
static uint8_t framebuffer[REMOTE_BUFFER_SIZE];
static uint8_t clockbuffer[3];
static uint8_t frameptr;
static int16_t receive_remain;

void remote_init(void)
{
	remotestate = REMOTE_STATE_NC;
	arm_timeout = false;
}

void remote_process( void )
{
	int16_t received_num;
	/* only process when the connection is ready */
	if( bt_is_ready())
	{
		switch( remotestate )
		{
			case REMOTE_STATE_NC:
				received_num = usart_receive( &protocolbuffer, 1, 0 );

				if( received_num > 0)
				{
					if( protocolbuffer == REMOTE_PROTOCOL_START_SLAVE)
					{
						remotestate = REMOTE_STATE_SLAVE;
						remoteslavestate = REMOTE_SLAVE_WAIT;
						arm_timeout = true;
						timeouttime = timer_get() + REMOTE_TIMEOUT;
						clock_mute(true);
					}
				}
				break;
			case REMOTE_STATE_SLAVE:
				switch( remoteslavestate )
				{
					case REMOTE_SLAVE_WAIT:
						received_num = usart_receive( &protocolbuffer, 1, 50 );
						if( received_num > 0)
						{

							if( protocolbuffer == REMOTE_PROTOCOL_SLAVE_STARTFRAME)
							{
								remoteslavestate = REMOTE_SLAVE_FRAME;
								frameptr = 0;
								timeouttime = timer_get() + REMOTE_TIMEOUT;
							}

							if( protocolbuffer == REMOTE_PROTOCOL_SLAVE_SETCLOCK)
							{
								remoteslavestate = REMOTE_SLAVE_SETCLOCK;
								frameptr = 0;
								timeouttime = timer_get() + REMOTE_TIMEOUT;
							}

							if( protocolbuffer == REMOTE_PROTOCOL_HEARTBEAT)
							{
								timeouttime = timer_get() + REMOTE_TIMEOUT;
							}

							if( protocolbuffer == REMOTE_PROTOCOL_LEAVE_SLAVE)
							{
								remotestate = REMOTE_STATE_NC;
								arm_timeout = false;
								clock_mute(false);
							}
						}
						else
						{
							if( timer_get() > timeouttime)
							{
								remotestate = REMOTE_STATE_NC;
								arm_timeout = false;
								clock_mute(false);
							}
						}
						break;
					case REMOTE_SLAVE_FRAME:
						receive_remain = REMOTE_BUFFER_SIZE - frameptr;
						if( receive_remain > REMOTE_BUFFER_BLOCK)
						{
							receive_remain = REMOTE_BUFFER_BLOCK;
						}

						received_num = usart_receive( &framebuffer[0+frameptr], receive_remain, 50 );

						if( received_num > 0)
						{
							frameptr += received_num;

							if( frameptr >= REMOTE_BUFFER_SIZE)
							{
								remote_display();
								remoteslavestate = REMOTE_SLAVE_WAIT;
								timeouttime = timer_get() + REMOTE_TIMEOUT;
							}
						}
						else
						{
							if( timer_get() > timeouttime)
							{
								remotestate = REMOTE_STATE_NC;
								arm_timeout = false;
								clock_mute(false);
							}
						}

						break;
					case REMOTE_SLAVE_SETCLOCK:
						received_num = usart_receive( clockbuffer, 3, 200 );
						if( received_num > 0 )
						{
							if(received_num >= 3)
							{
								uint8_t rtcbuffer[3];
								rtcbuffer[0] = dec2bcd( clockbuffer[2] );
								rtcbuffer[1] = dec2bcd( clockbuffer[1] );
								rtcbuffer[2] = dec2bcd( clockbuffer[0] );
								clock_set( clockbuffer[0], clockbuffer[1], clockbuffer[2]);
								i2c_transmit_blocking( 0x68, 0x00, &rtcbuffer[0], 3);
							}
						}
						else
						{
							if( timer_get() > timeouttime)
							{
								remotestate = REMOTE_STATE_NC;
								arm_timeout = false;
								clock_mute(false);
							}
						}
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
}

void remote_display(void)
{
	uint8_t i;

	for( i=0; i < 60; i++)
	{

		ws2812_set_pixel(0, i, framebuffer[3*i],
				framebuffer[3*i+1], framebuffer[3*i+2]);
	}
	ws2812_update();
}

