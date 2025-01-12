
/******************************************************************************
 *                                                                            *
 *  Author      : Murugan M                                                   *
 *  Date        : 12-01-2024                                                  *
 *  File        : PORTC_13th_pin                                              *
 *  Description : Led blink for PORTC 13th pin.                               *
 *                using for one pheripheral bus AHB1ENR                       *
 *                HSI internal clock using. 16MHz                             *
 *****************************************************************************/

#include "arm.h"   /* this header file for all register inside there. using for structure is user-defined data type */

/* main function */
int main(void)
{
  /* function call */
  rcc_config();
  peripheral_bus_config();
  gpioc_moder();

  while(1)
  {
    led_blinking();  /* keep on run for led blinking */
  }
}

void rcc_config(void)
{
  /* RCC register. starting page number is 102 */
  /* (RCC) Reset clock control (RCC_CR) 	Clock Control Register  */

  RCC->CR = RCC->CR & (~0x00010000); /* HSEON : HSE clock disable, 16th bit  */

  RCC->CR |= (1 << 16);      /* HSEON : HSE clock enable, 16th bit  */

  while(!(RCC->CR & (1 << 17)));  /* HSERDY : HSE oscillator ready, external high speed clock ready flag */

  /* Clock Configuration Register (RCC_CFGR) */

  RCC->CFGR = RCC->CFGR & (~0x00000003);    /* clear system clock switch, 0th bit and 1st bit set value 11, after the clear */

  RCC->CFGR |= (1 << 0);  /* set HSE oscillator selected as system clock, 0th bit set 1 */

  while(!(RCC->CFGR & (0x00000004)));  /* HSE oscillator used as system clock, 2nd bit set 1, status checking */
}

void peripheral_bus_config(void)
{
  /* Advanced High-Performance Bus 1, peripheral clock enable register (RCC_AHB1ENR) */
  RCC->AHB1ENR |= (1 << 2);   /* IO port c clock enable, 2nd bit set 1 */
}

void gpioc_moder(void)
{
  /* gpio register starting page number is 158 */
  /* port mode register (GPIOC_MODER) */

  GPIOC->MODER |= (1 << 26);   /*  01 : general purpose output mode, port c 13th pin in moder 13th pin 26th bit set 1 */
}

void delay_ms(int ms)
{
  /* delay */
  for (int i = 0; i < ms * 4000; i++)
  {
    __asm("NOP");   /* asm means assembly */
  }
}

void led_blinking(void)
{
  /* port output data register (GPIOC_ODR) */
  /* select the ODR 13th pin, set 1 and clear */
  /* PORTC 13th pin led */

  if (GPIOC->ODR & 1 << 13)   /* condition checking */
  {
    GPIOC->ODR &= ~(1 << 13);  /* led on */
    delay_ms(1000);            /* delay for 1000ms milliseconds for stop */
  }
  else
  {
    GPIOC->ODR |= (1 << 13);  /* led off */
    delay_ms(100);            /* delay for 1000ms milliseconds for stop */
  }
}
