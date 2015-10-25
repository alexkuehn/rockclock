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

#include "scheduler_if.h"
#include "scheduler.h"

#include "../schedule_table.h"

#include "../hal/timer_if.h"


#define SCHEDULETABLE_SIZE (sizeof(scheduletable)/sizeof(scheduletable[0]))
static const scheduletable_t scheduletable[] = schedule_cb_table;
static const uint8_t scheduletable_size = SCHEDULETABLE_SIZE;

static uint32_t tickstart[SCHEDULETABLE_SIZE];
static schedule_ctrl_state_t scheduler_state[SCHEDULETABLE_SIZE];
static uint8_t schedule_runflag[SCHEDULETABLE_SIZE];


void scheduler_init()
{
   uint8_t i;

   for( i = 0; i < scheduletable_size; i++ )
   {
      tickstart[i] = timer_get();
      schedule_runflag[i] = 0;
      scheduler_state[i] = SCHED_RUN;
   }
}


void scheduler_process()
{
   uint8_t i;
   uint32_t tickend, tickdiff;
   uint32_t period;


   tickend = timer_get();
   /* evaluation and execution of scheduled tasks is splitted to
    * obtain consistent timeline
    */

   /* evaluate which tasks should be executed */
   for( i = 0; i < scheduletable_size; i++ )
   {


      period = scheduletable[i].period;

      tickdiff = tickend - tickstart[i];
      if( tickdiff > period )
      {
         tickstart[i] = tickend;
         schedule_runflag[i] = 1;
      }
   }

   /* execute the tasks */
   for( i = 0; i < scheduletable_size; i++ )
   {
      if( (schedule_runflag[i] == 1) &&
          (scheduletable[i].periodic_cb != 0)
        )
      {
          if( scheduler_state[i] == SCHED_RUN )
            {
              scheduletable[i].periodic_cb();
            }
            schedule_runflag[i] = 0;
      }
   }
}

void scheduler_control( uint8_t processid, schedule_ctrl_state_t controlcmd )
{
  if( processid < SCHEDULETABLE_SIZE )
    {
      scheduler_state[processid] = controlcmd;
    }
}
