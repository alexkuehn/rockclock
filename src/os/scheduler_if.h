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

#ifndef SRC_OS_SCHEDULER_IF_H_
#define SRC_OS_SCHEDULER_IF_H_

#include <stdint.h>

/* definition of schedule table */
typedef struct
{
   uint32_t period;
   void (* periodic_cb)( void );
} scheduletable_t;

#define scheduler_period_ms(time) (time)

/* definition of scheduler control states */
typedef enum
{
  SCHED_STOP = 0,
  SCHED_RUN
} schedule_ctrl_state_t;


/* interface: periodic process scheduler
 */
extern void scheduler_process();

/* interface: control process scheduling
 * processid: id of controlled process
 * controlcmd: cmd id to set process state
 */
extern void scheduler_control( uint8_t processid, schedule_ctrl_state_t controlcmd );


#endif /* SRC_OS_SCHEDULER_IF_H_ */
