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
#include <stdint.h>

/* external includes */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

/* project includes */
#include "../hal/io_if.h"
#include "../hal/timer_if.h"
#if(CFG_DEBUG_DCF_SAMPLING)
#include "../hal/usart_if.h"
#include "../com/bt_if.h"
#endif

/* component includes */
#include "dcf_if.h"
#include "dcf.h"
#include "dcf_config.h"


static volatile uint8_t dcf_second_flag = 0;	/**< second marker mailbox value */
static volatile uint64_t dcf_data_buf = 0;		/**< dcf frame mailbox value */
static volatile uint8_t dcf_data_new = 0;		/**< dcf frame mailbox flag */

#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_PHY))
	const uint8_t dbg_dcf_str[] = "D0";			/**< DCF physical layer debug mark */
	#define DBG_DCF_STR_LEN 2					/**< DCF physical layer debug mark length*/
#endif
#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_SIG))
	const uint8_t dbg_dcf_filter_str[] = "DF";	/**< DCF signal debug mark */
	#define DBG_DCF_FILTER_STR_LEN 2			/**< DCF signal debug mark length*/
#endif
#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_SYNC_MACH))
	const uint8_t dbg_dcf_sync_str[] = "DS";	/**< DCF protocol layer debug mark */
	#define DBG_DCF_SYNC_STR_LEN 2				/**< DCF protocol layer debug mark length */
#endif



uint8_t dcf_mailbox_data_get( uint64_t *rawdcf)
{
	uint8_t retval;

	/* copy DCF data frame and reset data availability flag */
	*rawdcf = dcf_data_buf;
	retval = dcf_data_new;

	dcf_data_new = 0;
	return retval;
}

uint8_t dcf_mailbox_second_flag_get( void )
{
	uint8_t retval = dcf_second_flag;
	/* copy second marker and clear it afterwards */
	dcf_second_flag = 0;
	return retval;
}

void dcf_init( void )
{
    /* enable timer peripheral clock */
    rcc_periph_clock_enable(RCC_TIM14);

    /* configure timer for 1ms period */
    timer_reset( TIM14 );
    timer_set_mode( TIM14, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM14, 100);
    timer_set_period( TIM14, 4750);
    timer_set_repetition_counter( TIM14, 0);
    timer_disable_preload( TIM14 );


    /* enable timer interrupt source */
    nvic_enable_irq(NVIC_TIM14_IRQ);

    /* reset timer value */
    TIM14_SR = 0;

    /* configure timer interrupt for compare interrupt */
	timer_enable_irq( TIM14, TIM_DIER_UIE);

	timer_set_counter(TIM14, 60);

	/* finally enable the timer */
	timer_enable_counter(TIM14);
}

/** @brief Timer14 callback
 *
 * callback function for Timer14 interrupt.
 * Usage as DCF decoder timer: expected to be run every 1ms
 */
void tim14_isr(void)
{
	timer_clear_flag(TIM14, TIM_SR_UIF);

	/* process the DCF signal */
	dcf_sample_process();
}

void dcf_sample_process( void )
{
	static int8_t on_cnt = 0;				/**< counter for consecutive samples */
	static uint8_t filtered_sample = 0;		/**< filtered sample signal */
	uint8_t sample;

	/* get the actual sample from IO */
	sample = io_get(DCF_IN_PORT,  DCF_IN_PIN );

#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_PHY))
	/* debug: send actual sample value */
	if( bt_is_ready() )
	{
		usart_transmit( (uint8_t *)dbg_dcf_str, DBG_DCF_STR_LEN);
		usart_transmit( &sample, 1);
	}
#endif

	/* filter the sample with a debounce time
	 * the sample gets is final value after parametrized amount of consecutive samples
	 */
	if( sample > 0)
	{
		if( on_cnt < DCF_DEBOUNCE)
		{
			on_cnt++;
		}
	}
	else
	{
		if( on_cnt > 0)
		{
			on_cnt--;
		}
	}

	if( on_cnt >= DCF_DEBOUNCE)
	{
		filtered_sample = 1;
	}
	if( on_cnt == 0 )
	{
		filtered_sample = 0;
	}

#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_SIG))
	/* debug: send filtered sample signal */
	if( bt_is_ready() )
	{
		usart_transmit( (uint8_t *)dbg_dcf_filter_str, DBG_DCF_FILTER_STR_LEN);
		usart_transmit( &filtered_sample, 1);
	}
#endif
#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_SIG_LED))
	/* debug: output filtered signal over LED */
	if( filtered_sample >0)
	{
		io_on(LED_BLUE_PORT, LED_BLUE_PIN);
	}
	else
	{
		io_off(LED_BLUE_PORT, LED_BLUE_PIN);
	}
#endif
	/* now process the prefiltered signal */
	dcf_signal_process( filtered_sample);

}

void dcf_signal_process( uint8_t dcfsignal )
{

	static dcf_sig_state_t sig_state = DCF_SIG_INIT;

	static uint32_t last_timestamp = 0;
	static uint8_t last_sig_val = 0;
	static uint64_t bitbuffer = 0;
	static uint8_t bitpos = 0;
	uint32_t timediff = 0;
	uint32_t timeval;

	/* evaluate if a signal transition took place */
	if( dcfsignal != last_sig_val )
	{

		timeval = timer_get();
		timediff = timeval - last_timestamp;
		last_timestamp = timeval;
		switch( sig_state )
		{
			case DCF_SIG_INIT:
				if( dcfsignal == 1 )
				{
					/* found first positive edge */
					/* check if we found the 59. second with long pause */
					if( timediff > DCF_STATE_SYNC)
					{
						/* we found the sync pause
						 * now the next minute begins
						 */
						sig_state = DCF_SIG_SYNCED;
						bitpos = 0;
						bitbuffer = 0;
					}
				}
				break;

			case DCF_SIG_SYNCED:
				if( dcfsignal == 1)
				{
					/* positive edge
					 * begin of second marker
					 * but check plausibility: if last transition time was max. DCF_STATE_MIN milliseconds before, signal is invalid
					 *
					 */
					if( timediff < DCF_STATE_MIN)
					{
						/* signal not plausible: reinit */
						sig_state = DCF_SIG_INIT;
					}
					else
					{
						/* begin of second mark
						 * load the mailbox */
						dcf_second_flag = 1;
#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_SYNC_LED))
						io_on(LED_BLUE_PORT, LED_BLUE_PIN);
#endif

						/* data packed fully received
						 * load the data mailbox
						 */
						if( bitpos == 59)
						{
							bitpos = 0;
							dcf_data_buf = bitbuffer;
							dcf_data_new = 1;
						}
					}
				}
				else
				{
#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_SYNC_LED))
					io_off(LED_BLUE_PORT, LED_BLUE_PIN);
#endif
					/* negative edge
					 * find bitlength and plausibilize
					 */
					if( (timediff > DCF_STATE_LO_MIN) && (timediff < DCF_STATE_HI_MAX))
					{
						/* plausible length */
						if( timediff > DCF_STATE_HI_MIN)
						{
							bitbuffer |= ((uint64_t)1ULL << bitpos);
							bitpos++;
						}
						else
						{
							if( timediff < DCF_STATE_LO_MAX)
							{
								bitpos++;
							}
							else
							{
								/* signal not plausible: reinit */
								sig_state = DCF_SIG_INIT;

							}
						}
					}
					else
					{
						/* signal not plausible: reinit */
						sig_state = DCF_SIG_INIT;
					}
				}
				break;
			default:
				break;
		}
	}
	last_sig_val = dcfsignal;
}
