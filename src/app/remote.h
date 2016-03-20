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

#ifndef SRC_APP_REMOTE_H_
#define SRC_APP_REMOTE_H_

/* external standard includes */

/* external includes */

/* project includes */

/* component includes */


#define REMOTE_TIMEOUT  (1000) /**<  com connection timeout [ms] */

/* protocol symbols */
#define REMOTE_PROTOCOL_START_SLAVE 's'
#define REMOTE_PROTOCOL_LEAVE_SLAVE 'x'
#define REMOTE_PROTOCOL_SLAVE_STARTFRAME 'f'
#define REMOTE_PROTOCOL_HEARTBEAT 'h'

/* communication states */
typedef enum
{
   REMOTE_STATE_NC = 0,
   REMOTE_STATE_SLAVE
} remote_state_t;

/* slavemode states */
typedef enum
{
   REMOTE_SLAVE_WAIT = 0,
   REMOTE_SLAVE_FRAME
} remote_slave_state_t;

#endif /* SRC_APP_REMOTE_H_ */
