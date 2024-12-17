
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

  // GPIOBEN : IO port B clock enable
  *RCC_AHB1ENR |= (1<<1);

  // TIM10EN : TIM10 clock enable
  *RCC_APB2ENR |= (1<<17);

  // TIM11EN : TIM11 clock enable
  *RCC_APB2ENR |= (1<<18);
}

void gpio_portb(void)
{
  // first MODER register clear a two bits 16th and 17th bit
  *GPIOB_MODER &= ~(3<<16); //

  // 10 : Alternate function mode, PORTB 8th pin select for MODER8 in 16th and 17th bit
  *GPIOB_MODER |= (1<<17);

  // 11 : high speed, PORTB 8th pin select for OSPEEDR8 in 16th and 17th bit
  *GPIOB_OSPEEDR |= (3<<16);

  // 10 : pull-down, PORTB 8th pin select for PUPDR8 in 16th and 17th bit
  *GPIOB_PUPDR |= (1<<17);
  
  // 0011 : AF3, PORTB 8th pin select for AFRH8
  *GPIOB_AFRH & ~0x0f;
  *GPIOB_AFRH |= 0x3;
}

// PWM mode config

void timer10_pwm(void)
{
  // CC1E : capture/compare 1 output enable, 0 : off - OC1 is not active
  *TIM10_CCER &= ~(1<<0);

  // CC1S  : capture/compare 1 selection, OC1FE : output compare 1 fast enable, 
  // OC1PE : ouput compare 1 preload enable, disable 
  *TIM10_CCMR1 &= ~(1<<3);

  // CC1S : 00 : CC1 channel is configured as output
  *TIM10_CCMR1 &= (~3<<0); 

  // OC1M : output compare 1 mode, value 110 : PWM mode 1 select
  *TIM10_CCMR1 |= (6<<4);

  // OC1FE : output compare 1 fast enable
  *TIM10_CCMR1 |= (1<<2);

  // CCR1 [15:0] : capture/compare 1 value, first clear 16bit
  *TIM10_CCR1 &= ~0xffff;

  // CCR1 [15:0] : capture/compare 1 value, 16bit value give
  *TIM10_CCR1 |= 1;

  // ARPE : auto-reload preload enable
  *TIM10_CR1 |= (1<<7);

  // auto-reload register clear
  *TIM10_ARR &= (~0xffff);

  // auto-reload register value set
  *TIM10_ARR |= 320;

  // prescaler clear
  *TIM10_PSC &= (~0xffff);

  // prescaler value set
  *TIM10_PSC |= 1000;

  // counter value set
  *TIM10_CNT = 0;

  // CC1P : capture/compare 1 output polarity, 0 : OC1 active high
  *TIM10_CCER &= (~1<<1); 

  // CC1NP : capture/compare 1 complementry output polarity, CC1 as output : CCC1NP must be kept clear
  *TIM10_CCER &= ~(1<<3); 
  
  // CC1E : capture/compare 1 output enable, 1 : on - OC1 signal output pin
  *TIM10_CCER |= (1<<0);

  // OC1PE : output compare 1 preload enable, set 1
  *TIM10_CCMR1 |= (1<<3);

  // UG : update generation 0th bit set 1, CC1G : capture/compare 1 generation 1th bit set 1
  *TIM10_EGR |= (3<<0);

  // CEN : counter enable
  *TIM10_CR1 |= (1<<0);
}

// timer11 delay 

void timer11_delay(unsigned int x, unsigned y)
{
  // counter 0th bit disable clear
  *TIM11_CR1 &= (~1<<0);

  // UDIS : update disable set 1
  *TIM11_CR1 |= (1<<1);

  // URS : update request source disable
  *TIM11_CR1 &= (~1<<2);

  // ARPE : auto-reload preload enable
  *TIM11_CR1 |= (1<<7);

  // counter value set
  *TIM11_CNT = 0;

  // prescaler value set
  *TIM11_PSC = x;

  // auto-reload register value set
  *TIM11_ARR = y;

  // CEN : counter enable
  *TIM11_CR1 |= (1<<0);

  // UIF : update interrupt flag, this bit is set by hardware when the register are update
  while(!(*TIM11_SR & (1<<0)));
 
  // status register clear
  *TIM11_SR &= (~1<<0);
}

int main()
{ 
  rcc_config();
  gpio_portb();
  timer10_pwm();
  while(1)
  { 
    // 18 ms on, 2ms off
    for(int i=320; i>=0; i--)  // 20ms milli seconds value
    {
      while(!(*TIM10_SR & (1<<0))); // wait update interrupt flag
      *TIM10_SR &= (~1<<0); // clear the update
      *TIM10_CCR1 = i;     // load the for CCR1
      timer11_delay(1600,100); // delay given 1600 100
      *TIM10_PSC = 100; // update prescaler value
    } 
    for(int j=0; j<320; j++)
    { 
      while(!(*TIM10_SR & (1<<0)));
      *TIM10_SR &= (~1<<0);
      *TIM10_CCR1 = j;
      timer11_delay(1600,100);
      *TIM10_PSC = 100;
    } 
  }
}
