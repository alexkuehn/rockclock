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

/* component includes */
#include "remote_config.h"
#include "remote.h"

static remote_state_t remotestate;
static remote_slave_state_t remoteslavestate;
static uint8_t protocolbuffer;
static uint32_t timeouttime;
static uint8_t arm_timeout;
static uint8_t framebuffer[180];
static uint8_t frameptr;


static uint8_t dbgc = 't';
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
				received_num = usart_receive( &protocolbuffer, 1, 50 );

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

							if( protocolbuffer == REMOTE_PROTOCOL_SLAVE_STARTFRAME)
							{
								remoteslavestate = REMOTE_SLAVE_FRAME;
								frameptr = 0;
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
					case REMOTE_SLAVE_FRAME:
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


