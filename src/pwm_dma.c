#include"pwm_dma.h"
 // su dung TIM4
 
uint16_t u16Buffer[] = {1, 2, 3, 4, 5, 6};

void pwm_dma_Init(void)
{
  /* cau hinh TIM4  */
	RCC->APB1ENR |= ( 1 << 2 );
	TIM4->CR1 = (1 << 7) | (1 << 1); // TIMx_ARR register is buffered & UEV disabled
	TIM4->PSC = 72 - 1; // he so chia 72 --> f = 1 Mhz
	TIM4->ARR = 10-1;  // tgian nap lai 10 us -> f(PWM) = 100 kHz
	TIM4->CR1 &= ~(1 << 1); // UEV enabled
	TIM4->EGR = 1;
	TIM4->CR1 |= (1 << 0);
	
	/* cau hinh PWM */
	TIM4->CCMR1 &= ~(7 << 4);
	TIM4->CCMR1 |= (6 << 4);   // 110: PWM mode 1 , duty muc cao ( PWM mode 2 thi duty muc thap )
	TIM4->CCMR1 |= (1 << 3);   // Preload register on TIMx_CCR1 enabled
	TIM4->CCER |= (1 << 0);    // cho phep PWM chanel 1 hoat dong
	TIM4->CCR1 = 6;            // duty ( do rong xung)	
	
	/* datasheet: timer4 chanel 1 gan voi chan PB6  */
	RCC->APB2ENR |= (1 << 3);  // clock PB
	// PB6: AF-PP, Output speed 50MHz
	GPIOB->CRL &= ~(15 << 24);
	GPIOB->CRL |= (11 << 24);
	
	/* TIM4_CH1 of DMA1 Channel 1 */
  RCC->AHBENR |= (1 << 0);
	
	DMA1_Channel1->CCR &= ~(3 << 10);
	DMA1_Channel1->CCR |= (1 << 10); // Memory size 16 bit
	
	DMA1_Channel1->CCR &= ~(3 << 8);
	DMA1_Channel1->CCR |= (1 << 8); //  Peripheral size 16 bit
	
	DMA1_Channel1->CCR |= (3 << 12) | (1 << 7) | (1 << 4) | (1 << 5);
	
	DMA1_Channel1->CMAR = (uint32_t)u16Buffer;
	
	DMA1_Channel1->CPAR = (uint32_t)(&(TIM4->CCR1));
	
	DMA1_Channel1->CNDTR = sizeof(u16Buffer);
	
	DMA1_Channel1->CCR |= (1 << 0);
	
	TIM4->DIER |= (1 << 9);
}
