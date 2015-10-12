/*
 * scheduler.c
 *
 *  Created on: 06.08.2015
 *      Author: alex
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
