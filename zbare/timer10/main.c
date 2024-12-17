#include "header.h"
#include "fun.c"

void rcc_config()
{
  //first clear data
  *RCC_CR = *RCC_CR & (~0x00010000);

  //set HSE ON external clock for 16 bit 1 set
  *RCC_CR = *RCC_CR | (1<<16);

  //the HSE RDY external clock ready or not check
  while((*RCC_CR & (1<<17))==0);

  *RCC_CFGR = *RCC_CFGR & (~0x00000003);

  //system clock switch select the external clock
  *RCC_CFGR = *RCC_CFGR | (1<<0);

  //system clock switch status check
  while(!(*RCC_CFGR & (0x00000004))==4);

  // TIM10 clock enable
  *RCC_APB2ENR |= (1<<17);

  //RCC_AHB1ENR peripheral clock enable register
  *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);
}

//timer_conf the delay for 100 milli seconds

void timer_conf()
{
  //step1 : control register select, CEN counter  0th bit disable 0,
  *TIM10_CR1 = *TIM10_CR1 & ~(1<<0);

  //step2 :  UDIS update disable 1th bit set 1
  *TIM10_CR1 = *TIM10_CR1 | (1<<1);

  //step3 :  update request source  disable
  *TIM10_CR1 = *TIM10_CR1 & (~1<<2);

  //step4 : ARPE auto reload preload enable 7th bit set for 1
  *TIM10_CR1 = *TIM10_CR1 | (1<<7);
   
  //step5 : TIM10_CNT counter
  *TIM10_CNT = 0;

  //step6 : TIM10_PSC prescaler
  *TIM10_PSC = 1000;

  //step7 : TIM10_ARR auto reload register
  *TIM10_ARR = 12500;  //2seconds, for 2000 milli seconds value for 50000
                       //1seconds, for 1000 milli seconds value for 25000
                       //100 milli seconds value for 2500
                       //500 milli seconds value for 12500
                       //120 milli seconds value for 3000
                       //200 milli seconds value for 5000

  //step8 : TIM10_CR1 control register, counter  0th bit enable means 1
  *TIM10_CR1 = *TIM10_CR1 | (1<<0);

  //step9 : TIM10_SR update interrupt pending this bit is set by hardware, wait ontile 1
  while(!(*TIM10_SR & (1<<0))); 
  
  //step10 : TIM10_SR status register, clear
  *TIM10_SR = *TIM10_SR & (~1<<0); 
} 

void gpio_moder()
{
  //GPIO port mode register,general purpose output mode
  *GPIOA_MODER = *GPIOA_MODER | (1<<0);
}

void led_blink()
{

  //GPIOA port output data register, select porta 0th bit set 1 led on 
  *GPIOA_ODR = *GPIOA_ODR | (1<<0);

  //timer_config the delay for 100 milli seconds
  timer_conf();

  //GPIOA port output data register, select porta 0th bit set 0 led off
  *GPIOA_ODR = *GPIOA_ODR & (0<<0);

  //timer_config the delay for 100 milli seconds
  timer_conf();
}

int main()
{
  rcc_config();
  gpio_moder();
 // timer_conf();
  while(1)
  {
    led_blink();
  }
}
