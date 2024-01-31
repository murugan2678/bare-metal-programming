/* switch button led blinking */

#include "header.h"
#include "fun.c"

void rcc_config()
{
  *RCC_CR = *RCC_CR & ~(0x00010000);
  *RCC_CR = *RCC_CR | (1<<16);
  while((*RCC_CR & (1<<17))==0);

  *RCC_CFGR = *RCC_CFGR & ~(0x00000003);
  *RCC_CFGR = *RCC_CFGR | (1<<0);
  while(!(*RCC_CFGR & (0x00000004))==4);

  *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0); //PORT A select the port
  *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<1);
}

void gpio_moder()
{
  *GPIOA_MODER = *GPIOA_MODER | (1<<0);
  *GPIOB_MODER = *GPIOB_MODER | (1<<0);
  *GPIOA_PUPDR = *GPIOA_PUPDR | (1<<1); //pull down using 
}

void pir_config()
{

  //*GPIOA_ODR = *GPIOA_ODR | (1<<0);
    if((*GPIOA_IDR & (1<<0))==1)
    {
      *GPIOB_ODR = *GPIOB_ODR | (1<<0); //led on
      delay();
    }
    else
    {
      //delay();
      *GPIOB_ODR = *GPIOB_ODR & (0<<0); //led off
    }
}

void delay()
{
  int x;
  for(x=0; x<2500000; x++);
}

int main()
{
  rcc_config();
  gpio_moder();
  while(1)
  {
    pir_config();
  }
}
