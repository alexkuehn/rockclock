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
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>

/* project includes */
#include "io_if.h"

/* component includes */
#include "ws2812.h"


static uint8_t low_pattern = 0x00;		/**< bitpattern for low signal */
static uint8_t high_pattern = 0xFF;		/**< bitpattern for high signal */

static uint8_t frame_pattern[3*NR_OF_LEDS_PER_CH*NR_OF_ROWS];	/**< framebuffer */

static uint8_t bitframe_pattern[WS2812_BUFFERSIZE];			/**< bit unrolled framebuffer */
static uint8_t bitframe_pattern_draw[WS2812_BUFFERSIZE];	/**< bit unrolled backbuffer */

static uint8_t waitcnt = 0;	/**< timer waiting counter */

static uint8_t *actual_bitframe = bitframe_pattern;			/**< pointer to actual framebuffer */
static uint8_t *drawing_bitframe = bitframe_pattern_draw;	/**< pointer to backbuffer */

static volatile uint8_t update_flag = 0;		/**< buffer updating progress flag */

void ws2812_clear( void )
{
	uint32_t i;

	for( i = 0; i < (3*NR_OF_LEDS_PER_CH*NR_OF_ROWS); i++ )
	{
		frame_pattern[i] = 0;
	}
}

void ws2812_set_pixel( uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue )
{
	uint32_t index;


	index = 3*NR_OF_LEDS_PER_CH*row;
	index += 3*col;

	frame_pattern[index] = green;
	frame_pattern[index+1] = red;
	frame_pattern[index+2] = blue;
}

void ws2812_update( void )
{
	uint32_t i, bitpos,frameindex,rgbpos, runbit;
	uint32_t colpos, incol;
	uint8_t frameval;

	/* spinlock until buffer manipulation is ready */

	colpos = 0;
	incol = 0;
	rgbpos = 0;

	/* unroll the bits of a framebuffer */
	/* every bit of an WS2812 datastream is machester encoded
	 * translate every bit in a load value for timer
	 */
	for(i=0; i < WS2812_BUFFERSIZE; i++)
	{
		runbit = i & 7;
		drawing_bitframe[i] = 0;

		for( bitpos = 0; bitpos < 8; bitpos++)
		{
			frameindex = (3*NR_OF_LEDS_PER_CH*bitpos)+(3*colpos)+rgbpos;
			frameval = frame_pattern[frameindex];
			drawing_bitframe[i] |=  (((frameval << runbit) & 0x80 ) >> 7) << bitpos;
		}


		if( incol == (BITS_PER_LED-1) )
		{
			colpos++;
			incol = 0;
		}
		else
		{
			incol++;
		}

		if( runbit == 7)
		{
			rgbpos++;
			if( rgbpos == 3)
			{
				rgbpos = 0;
			}
		}
	}

	update_flag = 1;
}

void ws2812_init( void )
{
	uint32_t i,j;

	for( i = 0; i < WS2812_BUFFERSIZE; i++ )
	{
		bitframe_pattern[i] = 0;
		bitframe_pattern_draw[i] = 0;
	}

	for( i=0; i < (3*NR_OF_LEDS_PER_CH*NR_OF_ROWS); i++)
	{
		frame_pattern[i] = 0;

	}

	/* configure the GPIOs */
	/* Enable GPIOC clock  */
	rcc_periph_clock_enable(RCC_GPIOB);

	/* Setup GPIO pin GPIO8/9 on GPIO port C for LEDs. */
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, GPIO0);
	gpio_set_output_options( GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, GPIO0);

	/* configure the DMA */
	rcc_periph_clock_enable(RCC_DMA);

	/* configure DMA for sending high constant pattern to port on begin of cycle
	 * send unrolled bitframe on Timer OC1 event
	 * send low on Timer OC3 event
	 */
	dma_channel_reset( DMA1, DMA_CHANNEL3);
	dma_set_peripheral_address( DMA1, DMA_CHANNEL3, (uint32_t)&GPIOB_ODR);
    dma_set_memory_address( DMA1, DMA_CHANNEL3, (uint32_t)&high_pattern);
    dma_set_peripheral_size( DMA1, DMA_CHANNEL3, DMA_CCR_PSIZE_8BIT);
    dma_set_memory_size( DMA1, DMA_CHANNEL3, DMA_CCR_MSIZE_8BIT);
    dma_disable_peripheral_increment_mode( DMA1, DMA_CHANNEL3 );
    dma_disable_memory_increment_mode( DMA1, DMA_CHANNEL3 );
    dma_set_priority( DMA1, DMA_CHANNEL3, DMA_CCR_PL_HIGH);
    dma_set_read_from_memory(DMA1, DMA_CHANNEL3);

	dma_channel_reset( DMA1, DMA_CHANNEL4);
	dma_set_peripheral_address( DMA1, DMA_CHANNEL4, (uint32_t)&GPIOB_ODR);
    dma_set_memory_address( DMA1, DMA_CHANNEL4, (uint32_t)actual_bitframe);
    dma_set_peripheral_size( DMA1, DMA_CHANNEL4, DMA_CCR_PSIZE_8BIT);
    dma_set_memory_size( DMA1, DMA_CHANNEL4, DMA_CCR_MSIZE_8BIT);
    dma_disable_peripheral_increment_mode( DMA1, DMA_CHANNEL4 );
    dma_enable_memory_increment_mode( DMA1, DMA_CHANNEL4 );
    dma_set_priority( DMA1, DMA_CHANNEL4, DMA_CCR_PL_HIGH);
    dma_set_read_from_memory(DMA1, DMA_CHANNEL4);

	dma_channel_reset( DMA1, DMA_CHANNEL2);
	dma_set_peripheral_address( DMA1, DMA_CHANNEL2, (uint32_t)&GPIOB_ODR);
    dma_set_memory_address( DMA1, DMA_CHANNEL2, (uint32_t)&low_pattern);
    dma_set_peripheral_size( DMA1, DMA_CHANNEL2, DMA_CCR_PSIZE_8BIT);
    dma_set_memory_size( DMA1, DMA_CHANNEL2, DMA_CCR_MSIZE_8BIT);
    dma_disable_peripheral_increment_mode( DMA1, DMA_CHANNEL2 );
    dma_disable_memory_increment_mode( DMA1, DMA_CHANNEL2 );
    dma_set_priority( DMA1, DMA_CHANNEL2, DMA_CCR_PL_HIGH);
    dma_set_read_from_memory(DMA1, DMA_CHANNEL2);


    nvic_set_priority(NVIC_DMA1_CHANNEL2_3_IRQ, 0);
    nvic_enable_irq(NVIC_DMA1_CHANNEL2_3_IRQ);
    dma_enable_transfer_complete_interrupt( DMA1, DMA_CHANNEL2);


    /* configure timer */
    rcc_periph_clock_enable(RCC_TIM3);


    /* configure the timer for 800kHz overall frequency */
    timer_reset( TIM3 );
    timer_set_mode( TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_period( TIM3, 60);
    timer_set_repetition_counter( TIM3, 0);
    timer_disable_preload( TIM3 );

    /* timer compare section 1 for low coding */
    timer_set_oc_mode( TIM3, TIM_OC1,TIM_OCM_FROZEN);
    timer_set_oc_value( TIM3, TIM_OC1, 19);
    timer_disable_oc_preload( TIM3, TIM_OC1);

    /* timer compare section 3 for high coding */
    timer_set_oc_mode( TIM3, TIM_OC3,TIM_OCM_FROZEN);
    timer_set_oc_value( TIM3, TIM_OC3, 41);
    timer_disable_oc_preload( TIM3, TIM_OC3);

    nvic_set_priority( NVIC_TIM3_IRQ,1);
    nvic_set_priority( NVIC_PENDSV_IRQ, 2);
    nvic_enable_irq(NVIC_TIM3_IRQ);
    nvic_enable_irq(NVIC_PENDSV_IRQ);

    /* start the sending process */
    ws2812_send();
}

/** @brief Pending Service callback
 *
 * callback function for handling a pending service
 */
void pend_sv_handler( void )
{

}

/** @brief DMA2,3 callback
 *
 * callback function for DMA transfer
 */
void dma1_channel2_3_isr( void )
{
	/* after DMA transfer completed enable timer 3 overflow */
	dma_clear_interrupt_flags(DMA1, DMA_CHANNEL2, DMA_TCIF);
	timer_enable_irq(TIM3, TIM_DIER_UIE);

	dma_disable_channel( DMA1, DMA_CHANNEL2);
	dma_disable_channel( DMA1, DMA_CHANNEL3);
	dma_disable_channel( DMA1, DMA_CHANNEL4);

	timer_disable_irq( TIM3, TIM_DIER_CC1DE);
	timer_disable_irq( TIM3, TIM_DIER_CC3DE);
	timer_disable_irq( TIM3, TIM_DIER_UDE);


}

void ws2812_send( void )
{
	/* init the DMA data transfer */

	dma_clear_interrupt_flags( DMA1, DMA_CHANNEL2, DMA_TEIF | DMA_HTIF | DMA_TCIF | DMA_GIF);
	dma_clear_interrupt_flags( DMA1, DMA_CHANNEL3, DMA_TEIF | DMA_HTIF | DMA_TCIF | DMA_GIF);
	dma_clear_interrupt_flags( DMA1, DMA_CHANNEL4, DMA_TEIF | DMA_HTIF | DMA_TCIF | DMA_GIF);

	dma_set_memory_address( DMA1, DMA_CHANNEL4, (uint32_t)actual_bitframe);

	dma_set_number_of_data( DMA1, DMA_CHANNEL2, WS2812_BUFFERSIZE);
	dma_set_number_of_data( DMA1, DMA_CHANNEL3, WS2812_BUFFERSIZE);
	dma_set_number_of_data( DMA1, DMA_CHANNEL4, WS2812_BUFFERSIZE);

	TIM3_SR = 0;

	dma_enable_channel( DMA1, DMA_CHANNEL2);
	dma_enable_channel( DMA1, DMA_CHANNEL3);
	dma_enable_channel( DMA1, DMA_CHANNEL4);

	//timer_enable_irq( TIM3, TIM_DIER_TDE);
	timer_enable_irq( TIM3, TIM_DIER_CC1DE);
	timer_enable_irq( TIM3, TIM_DIER_CC3DE);
	timer_enable_irq( TIM3, TIM_DIER_UDE);

	timer_set_counter(TIM3, 60);


	timer_enable_counter(TIM3);


	SCB_ICSR |= SCB_ICSR_PENDSVSET;



}

/** @brief Timer3 callback
 *
 * callback function for Timer3
 */
void tim3_isr(void)
{
	uint8_t *bftemp;
	timer_clear_flag(TIM3, TIM_SR_UIF);


	/* wait an amount of time to trigger the WS2812 sync pulse */
	if( waitcnt < NR_OF_WAITCOUNTS)
	{

		waitcnt++;
	}
	else
	{

		waitcnt = 0;
		timer_disable_counter(TIM3);
		timer_disable_irq(TIM3, TIM_DIER_UIE);


		/* if new framebuffer data is available
		 * switch the unrolled buffer with the backbuffer
		 */
		if(update_flag == 1)
		{

			bftemp = actual_bitframe;
			actual_bitframe = drawing_bitframe;
			drawing_bitframe = bftemp;

			update_flag = 0;
		}

		/* restart the sending process */
		ws2812_send();
	}
}

uint8_t ws2812_get_updating( void )
{
	return update_flag;
}
