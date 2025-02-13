/****************************************************************************
 *                                                                          *
 *  Author      : Murugan M                                                 *
 *  Date        : 12-02-2025                                                *
 *  File        : stepper_motor_28BYJ-48                                    *
 *  Description : using for one pheripheral bus AHB1ENR                     *
 *                90 degree value for 510  and 180 degree value for 1019    *
 *                360 degree value for 2038                                 *
 *                using for HSI : internal clock for 16 MHz                 *                  
 ***************************************************************************/

#include "arm.h"   /* this header file for all register inside there. using for structure is user-defined data type */

/* stepper motor this value given means rotate */
char arr[] = {0x09, 0x0A, 0x06, 0x05};
   /* (or) */
//char arr[] = {0x0C, 0x06, 0x03, 0x09};

/* main function */
int main(void)
{
  /* function call */
  rcc_config();
  peripheral_bus_config();
  gpioa_moder_config();
  while(1)
  {
    stepper_motor_90degree();
    delay_ms(1000);
    stepper_motor_180degree();
    delay_ms(1000);
    stepper_motor_360degree();
    delay_ms(1000);
  }
}

/* function definition */
void rcc_config(void)
{
  /* select HSI internal clock for 16 MHz ---> Mega hertz. Mega means 10^6 */

  /* Reset clock control register (RCC_CR) */

  /* set and clear for HSION bit */
  RCC->CR &= ~(1 << 0);

  /* HSION : Internal high-speed clock enable. 0th bit set 1. 1 : HSI oscillator ON */
  RCC->CR |= (1 << 0);

  /* HSION : Internal high-speed clock enable. 0th bit set 1. 1 : HSI oscillator ON */
  while(!(RCC->CR & (1 << 1)));

  /* Reset clock control configuration register (RCC_CFGR) */

  /* SW : System clock switch. 00 : HSI oscillator selected as system clock. 0th bit and 1st bit. set 00 */
  RCC->CFGR &= ~(3 << 0);

  /* SWS : System clock switch status. 00 : HSI oscillator used as the system clock. 2nd bit and 3rd bit. set 00  */
  while((RCC->CFGR & (1 << 3)) != (0 << 2));
}

void peripheral_bus_config(void)
{
  /* Advanced High Bus 1. peripheral clock enable register (RCC_AHB1ENR) */

  /* GPIOAEN : port A clock enable */
  RCC->AHB1ENR |= (1 << 0);  // PORTA GPIOAEN: IO port A clock enable for 0th bit set 1.
}

void gpioa_moder_config(void)
{
  /* General purpose input and output (GPIOB_MODER) */

  /* PORT A select MODER0 to MODER3 */
  GPIOA->MODER |= (0x00000055);  //  01: General purpose output mode
}

void stepper_motor_90degree(void)
{
  for(int i = 0; i < 510; i++)   /* 90 degree rotate value : 510 steps */
  {
    for(int j = 0; j < 4; j++)
    {
      GPIOA->ODR = arr[j];
      delay_ms(2);   /* 2ms millisecond delay */
    }
  }
}

void stepper_motor_180degree(void)
{
  for(int m = 0; m < 1019; m++)    /* 180 degree rotate value : 1019 steps */
  {
    for(int p = 0; p < 4 ; p++)
    {
      GPIOA->ODR = arr[p];
      delay_ms(2);
    }
  }
}

void stepper_motor_360degree(void)
{
  for(int q = 0; q < 2038; q++)    /* 360 degree rotate value : 2038 steps */
  {
    for(int s = 0; s < 4; s++)
    {
      GPIOA->ODR = arr[s];
      delay_ms(2);
    }
  }
}

/* delay */
void delay_ms(int ms)
{
  for (int k = 0; k < ms * 4000; k++)
  {
    __asm("NOP");   /* asm menas assembly. NOP means no operation */
  }
}
