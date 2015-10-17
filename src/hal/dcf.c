/*
 * dcf.c
 *
 *  Created on: 16.10.2015
 *      Author: alex
 */

#include "dcf_if.h"
#include "../hal/io_if.h"

#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>


#define DCF_SAMPLES

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

	if( io_get(DCF_IN_PORT,  DCF_IN_PIN ) != 0)
	{
		io_on(LED_BLUE_PORT, LED_BLUE_PIN);
	}
	else
	{
		io_off(LED_BLUE_PORT, LED_BLUE_PIN);
	}
}
