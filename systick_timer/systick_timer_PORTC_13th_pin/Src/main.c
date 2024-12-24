
/*******************************************************************************
 *                                                                             *
 * Author      : Murugan M                                                     *
 * Date        : 23-12-2024                                                    *
 * File        : systick_timer_PORTC_13th_pin                                  *
 * Description : SysTick timer 100ms milliseconds                              *
 *               The processor has a 24-bit system timer. RELOAD               *
 *               value can be any value in the range 0x00000001-0x00FFFFFF     *
 *               the SysTick timer datasheet for STM32 programming manual      *
 *               using for one peripheral bus AHB1ENR                          *
 *                                                                             *
 *******************************************************************************/


#include "arm.h" /* header file arm .h */


/* this for MACRO */

#define RELOAD_VALUE 1600000   /* 100ms milliseconds */
// #define RELOAD_VALUE 3200000   /* 200ms milliseconds */

/* main function */

int main(void)
{
  /* function call */

  rcc_config();
  peripheral_bus_config();
  gpioc_moder_config();

  while(1)  /* continuously running */
  {
    led_blinking();
  }
}

/* rcc config */

void rcc_config(void)
{
  /* Reset clock control (RCC_CR) clock control register */

  /* set and clear 16th bit. HSEON */
  *RCC_CR &= (~1 << 16);

  /* 16th bit set 1. HSEON : HSE clock enable. 1 : HSE oscillator ON */
  *RCC_CR |= (1 << 16);

  /* 17th bit set 1. HSERDY : HSE clock enable. 1 : HSE oscillator ready. checking for HSE oscillator ready or not */
  while(!(*RCC_CR & 1 << 17) == 0);

  /* Reset clock control (RCC_CFGR) clock configuration register */

  /* set and clear 0th bit and 1th bit. SW, 11 : not allowed */
  *RCC_CFGR &= (~0x00000003);

  /* 0th bit set 1. SW : system clock switch. 01 : HSE oscillator selected as system clock */
  *RCC_CFGR |= (1 << 0);

  /* 2nd bit set 1. SWS : system clock switch status. 01 : HSE oscillator used as system clock */
  while(!(*RCC_CFGR & 0x00000004) == 4);
}

/* peripheral bus config */

void peripheral_bus_config(void)
{
  /* Advanced High Bus 1 (RCC_AHB1ENR) peripheral clock enable register */
  /* select POART C. GPIOCEN : IO port clock enable. 2nd bit set 1 */
  *RCC_AHB1ENR |= (1 << 2);
}

/* gpioc moder config */

void gpioc_moder_config(void)
{
  /* General purpose input and output (GPIOC_MODER) port mode register */

  /* select PORTC 13th pin and select MODER13. select 01 : General purpose output mode. set 26th bit 1 */
  *GPIOC_MODER |= (1 << 26);
}

/* SysTick timer delay config */

void systick_timer_delay(int val)
{
  /* SysTick timer (STK). 24-bit system timer */

  /* 24-bit set and clear load register */
  *STK_LOAD &= (~0x00FFFFFF);

  /* step 1 ---> reload value. (STK_LOAD) SysTick reload value register */
  *STK_LOAD |= val;

  /* (STK_CTRL) SysTick control and status register */
  /* step 2 ---> counter enable 0th bit set 1. CLKSOURCE : Clock source selection 2nd bit set 1 : Processor clock (AHB).
     binary value 0101 ---> decimal value 5. */
  *STK_CTRL |=  (5 << 0);

  /* step 3 ---> program control and register value. STK_(CTRL) */
  /* 16th bit wait for COUNTFLAG. if timer counted to 0 since last time was read. Return 1 */
  while(!(*STK_CTRL & (1 << 16)));

  /* step 4 ---> COUTNFLAG clear */
  *STK_CTRL &= (~1 << 16);
}

/* led config for ODR */

void led_blinking(void)
{
  /* General Purpose Input and Output (GPIOC_ODR) port output data register */
  /* select the ODR13 set 13th pin, PORTC 13th pin pull up register there */
  *GPIOC_ODR &= (~1 << 13);  /* led on */
  systick_timer_delay(RELOAD_VALUE);  /* delay for 100ms milliseconds stop */

  *GPIOC_ODR |= (1 << 13);  /* led off */
  systick_timer_delay(RELOAD_VALUE);  /* delay for 100ms milliseconds stop */
}
