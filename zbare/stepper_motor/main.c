
/////////////////////////////////////////////////////
/* Author      : Murugan M                         */
/* Date        : 06-03-2024                        */
/* File name   : stepper motor                     */
/* Describtion : stepper motor is a burshless      */
/*               DC-Direct current electric motor  */
/////////////////////////////////////////////////////

#include "header.h"
#include "fun.c"

void rcc_config()
{
  // HSE clock 16th bit clear --> CR
  *RCC_CR &= (~1<<16);

  // HSE clock enable 16th set 1 --> CR
  *RCC_CR |= (1<<16);

  // HSERDY clock ready flag check for 17th bit set 1 --> CR
  while(!(*RCC_CR & (1<<17))==0);

  // System clock switch 0th bit and 1th bit clear --> CFGR
  *RCC_CFGR &= ~(3<<0);

  // System clock switch enable for HSE 0th bit set 1 --> CFGR
  *RCC_CFGR |= (1<<0);

  // System clock switch status check for HSE 2nd bit set 1 --> CFGR
  while(!(*RCC_CFGR & (1<<2))==4);

  // IO port A clock enable 0th bit set 1 --> AHB1ENR
  *RCC_AHB1ENR |= (1<<0);
}

void gpio_moder()
{
  for(int i=0; i<8; i++)
  {
    // General purpose output mode range for 0th bit to 7th bit enable --> MODER
    // main 4 pin enable 
    *GPIOA_MODER |= (1<<i);
    i++; 
  }
}

void step_motor()
{
  // stepper motor hexa decimal value for 8bit value 
  char arr[] = {0x07,0x03,0x0B,0x09,0x0D,0x0C,0x0E,0x06};
  int j,k,s;
  
  // keep on rotate a stepper motor
  while(1)
  {
    // permanent magnet rotor 96 pole, 96/2 = 48, upper 48 pole, down 48 pole
    // totally 96 pole
    for(k=0; k<48; k++)
    {
      // 8 pole stator, coil 
      for(j=0; j<8; j++)
      {	  
	// 8 bit data send for port A, 4 led on
	*GPIOA_ODR |= arr[j];

	// 5 milli seconds
	systick_delay();
          
	// 4 led led off
	*GPIOA_ODR &= (0x00000000);

	// 5 milli seconds
	systick_delay();
      }
    }
  }
}

// here use for systick timer
void systick_delay()
{
  // systick reload value register, reload 0th bit to 23th bit
  // *STK_LOAD = 2499999; 100ms
  *STK_LOAD =   124999; //5ms // 199999; 8ms // 499999; 20ms  //249999; 10 ms

  // Counter enable 0th bit set 1, systick control and status register  
  *STK_CTRL = *STK_CTRL | (1<<0);

  // clock source selection, 2nd bit set 1, processor clock (AHB) 
  *STK_CTRL = *STK_CTRL | (1<<2);
  
  // return 1 if timer count to 0 last time this was read 
  while(!(*STK_CTRL & (1<<16)));
}

int main()
{
    // here a all function call
    rcc_config();
    gpio_moder();
    step_motor();
}
