
///////////////////////////////////////////////
/*                                           */
/*  Author      : Murugan M                  */
/*  Date        : 23-03-2024                 */
/*  File name   : pwm                        */
/*  Describtion : Pulse Width Modulation     */
/*                18ms on and 2ms off        */
/*                                           */
///////////////////////////////////////////////


#include "header.h"
#include "fun.c"


void rcc_config(void)
{
  // HSIRDY : internal high-speed clock ready flag
  while(!(*RCC_CR & (1<<1)));

  // TIM10EN : TIM10 clock enable
  *RCC_APB2ENR |= (1<<17);

  // GPIOBEN : IO port B clock enable
  *RCC_AHB1ENR |= (1<<1);
}

void gpio_portb(void)
{
  // first MODER register clear a two bits 16th and 17th bit
  *GPIOB_MODER &= ~(3<<16);

  // 10 : Alternate function mode, PORTB 8th pin select for MODER8 in 16th and 17th bit
  *GPIOB_MODER |= (1<<17); 

  // 11 : high speed, PORTB 8th pin select for OSPEEDR8 in 16th and 17th bit
  *GPIOB_OSPEEDR |= (3<<16);

  // 10 : pull-down, PORTB 8th pin select for PUPDR8 in 16th and 17th bit
  *GPIOB_PUPDR |= (1<<17);
  
  // 0011 : AF3, PORTB 8th pin select for AFRH8
  *GPIOB_AFRH |= (3<<0); 
}

// PWM mode config

void timer10_pwm(void)
{
  // CC1E : capture/compare 1 output enable, 0 : off - OC1 is not active
  *TIM10_CCER &= (~1<<0); // (~1<<0) //~(1<<0)

  // CC1S  : capture/compare 1 selection, OC1FE : output compare 1 fast enable, 
  // OC1PE : ouput compare 1 preload enable, this 0,1,2,3 four bit clear
  *TIM10_CCMR1 &= (~1<<3); // (~1<<3) //~(1<<3)

  // CC1S : 00 : CC1 channel is configured as output
  *TIM10_CCMR1 &= (~3<<0); //~0x3; // (~3<<0)

  // CC1M : output compare 1 mode, value 110 : PWM mode 1 select
  *TIM10_CCMR1 |= (6<<4); //0x60

  // OC1FE : output compare 1 fast enable
  *TIM10_CCMR1 |= (1<<2);

  // CCR1 [15:0] : capture/compare 1 value, 16bit value give
  *TIM10_CCR1 = 1; //75

  // ARPE : auto-reload preload enable
  *TIM10_CR1 |= (1<<7);

  // auto-reload register value set
  *TIM10_ARR = 320;  // 1.5ms value for 375 

  // prescaler value set
  *TIM10_PSC = 999;//1000;

  // counter value set
  *TIM10_CNT = 0;

  // CC1P : capture/compare 1 output polarity, 0 : OC1 active high
  *TIM10_CCER &= (~1<<1); 

  // CC1NP : capture/compare 1 complementry output polarity, CC1 as ouput : CCC1NP must be kept clear
  *TIM10_CCER &= (~1<<3); // ~(1<<3)
  
  // CC1E : capture/compare 1 output enable, 1 : on - OC1 signal output pin
  *TIM10_CCER |= (1<<0);

  // OC1PE : output compare 1 preload enable, set 1
  *TIM10_CCMR1 |= (1<<3);

  // UG : update generation 0th bit set 1, CC1G : capture/compare 1 generation 1th bit set 1
  *TIM10_EGR |= (3<<0);

  // CEN : counter enable
  *TIM10_CR1 |= (1<<0);
}

int main()
{ 
  rcc_config();
  gpio_portb();
  timer10_pwm();
  while(1)
  { 
   for(int i=320; i>0; i--)
   {
      while(!(*TIM10_SR & (1<<0)));
      *TIM10_SR &= (~3<<0);
      *TIM10_CCR1 = i;
   }   
   for(int j=0; j<320; j++) 
   { 
      while(!(*TIM10_SR & (1<<0)));
      *TIM10_SR &= (~3<<0);
      *TIM10_CCR1 = j;
   }
  }
} 
