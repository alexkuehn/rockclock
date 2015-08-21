/*
 * ws2812.c
 *
 *  Created on: 09.08.2015
 *      Author: alex
 */

#include "ws2812.h"

#include <stdint.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>


#include "io_if.h"

static uint8_t low_pattern = 0x00;
static uint8_t high_pattern = 0xFF;

static uint8_t frame_pattern[3*NR_OF_LEDS_PER_CH*NR_OF_ROWS];
static uint8_t bitframe_pattern[WS2812_BUFFERSIZE];
static uint8_t bitframe_pattern_back[WS2812_BUFFERSIZE];

static uint8_t waitcnt = 0;

static uint8_t *actual_bitframe = bitframe_pattern;
static uint8_t *backed_bitframe = bitframe_pattern_back;

static volatile uint8_t transferring = 0;

void ws2812_clear( void )
{
}

void ws2812_set( uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue )
{
	uint8_t i;

	for( i = 0; i < 8; i++)
	{
		/* clear data */
		bitframe_pattern[((col*24)+i)] &= ~(0x01<<row);
		bitframe_pattern[((col*24)+8+i)] &= ~(0x01<<row);
		bitframe_pattern[((col*24)+16+i)] &= ~(0x01<<row);
		/* write new value */
		bitframe_pattern[((col*24)+i)] |= ((((green<<i) & 0x80)>>7)<<row);
		bitframe_pattern[((col*24)+8+i)] |= ((((red<<i) & 0x80)>>7)<<row);
		bitframe_pattern[((col*24)+16+i)] |= ((((blue<<i) & 0x80)>>7)<<row);
	}
}

void ws2812_init( void )
{
	uint32_t i,j;

	for( i = 0; i < WS2812_BUFFERSIZE; i++ )
	{
		bitframe_pattern[i] = 0;

		bitframe_pattern_back[i] = 0;

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


    timer_reset( TIM3 );
    timer_set_mode( TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    timer_set_period( TIM3, 60);
    timer_set_repetition_counter( TIM3, 0);
    timer_disable_preload( TIM3 );

    timer_set_oc_mode( TIM3, TIM_OC1,TIM_OCM_FROZEN);
    timer_set_oc_value( TIM3, TIM_OC1, 12);
    timer_disable_oc_preload( TIM3, TIM_OC1);

    timer_set_oc_mode( TIM3, TIM_OC3,TIM_OCM_FROZEN);
    timer_set_oc_value( TIM3, TIM_OC3, 24);
    timer_disable_oc_preload( TIM3, TIM_OC3);

    nvic_set_priority( NVIC_TIM3_IRQ,1);
    nvic_set_priority( NVIC_PENDSV_IRQ, 2);
    nvic_enable_irq(NVIC_TIM3_IRQ);
    nvic_enable_irq(NVIC_PENDSV_IRQ);

}

void pend_sv_handler( void )
{
	uint32_t i,j;
	uint32_t bitindex, bufferindex;
	uint32_t mask;
	io_on( GPIOC, GPIO9);



	for( i = 0; i < NR_OF_LEDS_PER_CH; i++ )
	{
		bitindex = i*24;
		bufferindex = i*3*NR_OF_ROWS;

		mask = frame_pattern[bufferindex];
		backed_bitframe[bitindex] = (mask & 0x80) >> 7;
		backed_bitframe[bitindex+1] = (mask & 0x40) >> 6;
		backed_bitframe[bitindex+2] = (mask & 0x20) >> 5;
		backed_bitframe[bitindex+3] = (mask & 0x10) >> 4;
		backed_bitframe[bitindex+4] = (mask & 0x08) >> 3;
		backed_bitframe[bitindex+5] = (mask & 0x04) >> 2;
		backed_bitframe[bitindex+6] = (mask & 0x02) >> 1;
		backed_bitframe[bitindex+7] = (mask & 0x01);

		bufferindex++;

		mask = frame_pattern[bufferindex];
		bitindex += 8;

		backed_bitframe[bitindex] = (mask & 0x80) >> 7;
		backed_bitframe[bitindex+1] = (mask & 0x40) >> 6;
		backed_bitframe[bitindex+2] = (mask & 0x20) >> 5;
		backed_bitframe[bitindex+3] = (mask & 0x10) >> 4;
		backed_bitframe[bitindex+4] = (mask & 0x08) >> 3;
		backed_bitframe[bitindex+5] = (mask & 0x04) >> 2;
		backed_bitframe[bitindex+6] = (mask & 0x02) >> 1;
		backed_bitframe[bitindex+7] = (mask & 0x01);

		bufferindex++;

		mask = frame_pattern[bufferindex];
		bitindex += 8;

		backed_bitframe[bitindex] = (mask & 0x80) >> 7;
		backed_bitframe[bitindex+1] = (mask & 0x40) >> 6;
		backed_bitframe[bitindex+2] = (mask & 0x20) >> 5;
		backed_bitframe[bitindex+3] = (mask & 0x10) >> 4;
		backed_bitframe[bitindex+4] = (mask & 0x08) >> 3;
		backed_bitframe[bitindex+5] = (mask & 0x04) >> 2;
		backed_bitframe[bitindex+6] = (mask & 0x02) >> 1;
		backed_bitframe[bitindex+7] = (mask & 0x01);


#if(0)
		for( i = 0; i < 8; i++)
		{
			/* clear data */
			backed_bitframe[((col*24)+i)] &= ~(0x01<<row);
			backed_bitframe[((col*24)+8+i)] &= ~(0x01<<row);
			backed_bitframe[((col*24)+16+i)] &= ~(0x01<<row);
			/* write new value */
			backed_bitframe[((col*24)+i)] |= ((((green<<i) & 0x80)>>7)<<row);
			backed_bitframe[((col*24)+8+i)] |= ((((red<<i) & 0x80)>>7)<<row);
			backed_bitframe[((col*24)+16+i)] |= ((((blue<<i) & 0x80)>>7)<<row);
		}
#endif

	}

	io_off( GPIOC, GPIO9);

}

void dma1_channel2_3_isr( void )
{
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
	transferring = 1;

	io_on( GPIOC, GPIO10);

	dma_clear_interrupt_flags( DMA1, DMA_CHANNEL2, DMA_TEIF | DMA_HTIF | DMA_TCIF | DMA_GIF);
	dma_clear_interrupt_flags( DMA1, DMA_CHANNEL3, DMA_TEIF | DMA_HTIF | DMA_TCIF | DMA_GIF);
	dma_clear_interrupt_flags( DMA1, DMA_CHANNEL4, DMA_TEIF | DMA_HTIF | DMA_TCIF | DMA_GIF);

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

void tim3_isr(void)
{
	static uint8_t *bftemp;
	timer_clear_flag(TIM3, TIM_SR_UIF);

#if(0)
	bftemp = actual_bitframe;
	actual_bitframe = backed_bitframe;
	backed_bitframe = bftemp;
#endif

	if( waitcnt < NR_OF_WAITCOUNTS)
	{

		waitcnt++;
	}
	else
	{
		io_off( GPIOC, GPIO10);
		waitcnt = 0;
		timer_disable_counter(TIM3);
		timer_disable_irq(TIM3, TIM_DIER_UIE);

		transferring = 0;
	}
}

uint8_t ws2812_get_transferring( void )
{
	return transferring;
}
