#include "header.h"
#include "fun.c"

void rcc_config()
{
  //RCC_CR select for 16th bit, first clear device
  *RCC_CR &= ~(0x0001000);

  //RCC_CR select for 16th bit HSE clock enable
  *RCC_CR |= (1<<16);

  //RCC_CR 17th bit HSE clock ready flag or not check
  while((*RCC_CR & (1<<17))==0);

  //RCC_CFGR system clock switch 0th bit and 1th bit, HSE oscillator external clock
  *RCC_CFGR &= (0x00000003);

  //RCC_CFGR system clock switch, 0th bit set 1 
  *RCC_CFGR |= (1<<0);

  //RCC_CFGR system clock switch status
  while(!(*RCC_CFGR & (1<<2))==4);

  //RCC_AHB1ENR peripheral clock enable register, 0th bit port A clock enable
  *RCC_AHB1ENR |= (1<<0);

  //RCC_APB2ENR peripheral clock enable register, 18th bit TIM11 clock enable
  *RCC_APB2ENR |= (1<<18);
}

void gpio_moder()
{
  //GPIOA_MODER 0th bit set for 1 general purpose output mode
  *GPIOA_MODER |= (1<<0); 
}

void timer11_config()
{
  //control register, 0th bit counter disable
  *TIM11_CR1 &= ~(1<<0); 

  //update disable
  *TIM11_CR1 |= (1<<1);

  //update request source disable
  *TIM11_CR1 &= (~1<<2);

  //auto-reload preload enable
  *TIM11_ARR |= (1<<7);

  //counter value give
  *TIM11_CNT = 0;

  //prescaler value give
  *TIM11_PSC = 1000;              //totally 100 milli secods value 2500000 this value sparate ARR = 2500 PSC = 1000

  //auto-reload register
  *TIM11_ARR = 12500;             //delay value, timer11 calculation for 100 milli seconds
                                  //timer11 100ms milli seconds value for 2500
                                  //timer11 120ms milli seconds value for 3000
                                  //timer11 500ms milli seconds value for 12500

  //counter enable
  *TIM11_CR1 |= (1<<0);

  //status register, update interrupt flag
  while(!(*TIM11_SR & (1<<0)));

  //status register clear
  *TIM11_SR &= ~(1<<0);
}

void led_blink()
{
  //port output data register, led on
  *GPIOA_ODR |= (1<<0);
  
  //delay for 100 milli seconds
  timer11_config();

  //port output data register, led off
  *GPIOA_ODR &= (0<<0);

  //delay for 100 milli seconds
  timer11_config();
}

int main()
{
  rcc_config();
  gpio_moder();
  while(1)
  {
    led_blink();
  }
}
