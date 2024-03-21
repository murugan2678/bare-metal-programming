
/////////////////////////////////////////////////////
/*                                                 */
/* Author      : Murugan M                         */
/* Date        : 06-03-2024                        */
/* File name   : PLL configuration                 */
/* Describtion : the pll use for frequency         */
/*               calculate MHZ                     */
/*                                                 */
/////////////////////////////////////////////////////

// stm32f401ccu6 this model use for 25MHZ megahertz 
// this program frequency calculate MHZ

#include "header.h"
#include "fun.c"

void rcc_pllconf()
{

  // RCC_CR PLLCFGR first you clear
  *RCC_CR = *RCC_CR & (~0x01000000);
  
  // RCC_CR PLLCFGR select 24 bit set 1 pll on
  *RCC_CR = *RCC_CR | (1<<24);
  
  // RCC_CR PLLRDY pll ready
  while((*RCC_CR & (1<<25))==0);

  // RCC_PLLCFGR steps
  // step1 = /M division factor 25/5 = 5, 0 to 5 bits 
  // step2 = xN multiplication factor 5x8 = 40, 6 to 14 bits
  // step3 = /P division factor main system clock 40/2 = 20, finally come 20MHZ, 16 to 17 bits
 
  // step1 : PLLM set value for 5 --> 000101 ,range bit for 0 to 5th bit
  *RCC_PLLCFGR = *RCC_PLLCFGR | (1<<0);
  *RCC_PLLCFGR = *RCC_PLLCFGR | (1<<2);
  
  // step2 : PLLN set value for 8 --> 000001000 ,range bit for 6 to 14th bit 
  *RCC_PLLCFGR = *RCC_PLLCFGR | (1<<9);
  
  // step3 : PLLP set value for 2 --> 00 , range bit for 16 to 17th bit
  *RCC_PLLCFGR = *RCC_PLLCFGR | (0<<16);
  *RCC_PLLCFGR = *RCC_PLLCFGR | (0<<17);

  // RCC_AHB1ENR peripheral clock enable register, select a PORTA
  *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);

}

void gpio_moder()
{
  // GPIOA_MODER, port mode register select A 0th bit set for 1
  *GPIOA_MODER = *GPIOA_MODER | (1<<0);
}

void led_blink()
{
  // GPIOA_MODER, port output data register select A 0th bit set for 1
  *GPIOA_ODR = *GPIOA_ODR | (1<<0); // led on
}  

int main()
{
  //here a call the all functions
  rcc_pllconf();
  gpio_moder();
  while(1)
  {
    led_blink();
  }
}
