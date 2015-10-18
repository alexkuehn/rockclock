/*
 * dcf.c
 *
 *  Created on: 16.10.2015
 *      Author: alex
 */

#include "dcf_if.h"
#include "dcf_config.h"
#include "../hal/io_if.h"
#include "../hal/timer_if.h"

#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>


#define DCF_DEBOUNCE 3

#define DCF_STATE_SYNC 1400 /* ms */

#if(CFG_DEBUG_DCF_SAMPLING)
#include "../hal/usart_if.h"
#include "../com/bt_if.h"
#endif

extern void dcf_sample_process( void );
extern void dcf_signal_process( uint8_t dcfsignal );

void dcf_init( void )
{
    /* configure timer */
    rcc_periph_clock_enable(RCC_TIM14);

    timer_reset( TIM14 );
    timer_set_mode( TIM14, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_prescaler(TIM14, 100);
    timer_set_period( TIM14, 4750);
    timer_set_repetition_counter( TIM14, 0);
    timer_disable_preload( TIM14 );



    nvic_enable_irq(NVIC_TIM14_IRQ);
    TIM14_SR = 0;

	timer_enable_irq( TIM14, TIM_DIER_UIE);

	timer_set_counter(TIM14, 60);
	timer_enable_counter(TIM14);
}

void tim14_isr(void)
{
	timer_clear_flag(TIM14, TIM_SR_UIF);

	dcf_sample_process();
}

void dcf_sample_process( void )
{
	static int8_t on_cnt = 0;
	static uint8_t filtered_sample = 0;
#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_PHY))
	const uint8_t dbg_dcf_str[] = "D0";
	#define DBG_DCF_STR_LEN 2
#endif
#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_SIG))
	const uint8_t dbg_dcf_filter_str[] = "DF";
	#define DBG_DCF_FILTER_STR_LEN 2
#endif
	uint8_t sample;

	sample = io_get(DCF_IN_PORT,  DCF_IN_PIN );

#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_PHY))
	if( bt_is_ready() )
	{
		usart_transmit( (uint8_t *)dbg_dcf_str, DBG_DCF_STR_LEN);
		usart_transmit( &sample, 1);
	}
#endif

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
	if( bt_is_ready() )
	{
		usart_transmit( (uint8_t *)dbg_dcf_filter_str, DBG_DCF_FILTER_STR_LEN);
		usart_transmit( &filtered_sample, 1);
	}
#endif
#if((CFG_DEBUG_DCF_SAMPLING & CFG_DCF_DEBUG_SIG_LED))
	if( filtered_sample >0)
	{
		io_on(LED_BLUE_PORT, LED_BLUE_PIN);
	}
	else
	{
		io_off(LED_BLUE_PORT, LED_BLUE_PIN);
	}

	dcf_signal_process( filtered_sample);
#endif

}

void dcf_signal_process( uint8_t dcfsignal )
{
	typedef enum { DCF_SIG_INIT, DCF_SIG_SYNCED } dcf_sig_state_t;
	static dcf_sig_state_t sig_state = DCF_SIG_INIT;
	static uint32_t last_timestamp = 0;
	static uint8_t last_sig_val = 0;
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

					}
				}
				break;
			default:
				break;
		}
	}
	last_sig_val = dcfsignal;
}
