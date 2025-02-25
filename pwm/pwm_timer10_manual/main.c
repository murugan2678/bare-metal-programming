/******************************************************************************
 *                                                                            *
 *  Author      : Murugan M                                                   *
 *  Date        : 17-01-2024                                                  *
 *  File        : pwm_timer10_manual                                          *
 *  Description : Led blink for PORTA 0th pin.                                *
 *                using for two pheripheral bus AHB1ENR and APB2ENR           *
 *		  i give 1ms milliseconds on 19ms milliseconds off            *
 *                							      *
 *****************************************************************************/



#include "arm.h"    /* this header file for all register inside there. using for structure is user-defined data type */

/* main function */
int main(void)
{
  /* function call */
  rcc_config();
  peripheral_bus_config();
  gpioa_moder_config();
  while(1)
  {
    led_blinking();
  }
}

/* function definition */
void rcc_config(void)
{
  /* Reset clock control (RCC_CR) clock control register */

  /* set and clear 16th bit */
  RCC->CR &= (~0x00010000);

  /* select 16th bit HSEON : HSE clock enable. 1 : HSE oscillator ON */
  RCC->CR |= (1 << 16);

  /* select 17th bit HSERDY : HSE clock ready flag. 1 : HSE oscillator ready. HSE external oscillator ready or not checking */
  while(!(RCC->CR & (1 << 17)));

  /* Reset clock control (RCC_CFGR) clock configuration register */

  /* set and clear 0th bit and 1st bit. 11 : not allowed */
  RCC->CFGR &= (~0x00000003);

  /* select 0th bit SW  : System clock switch. 01 : HSE oscillator selected as system clock */
  RCC->CFGR |= (1 << 0);

  /* select 2nd bit SWS : System clock switch status. 01 : HSE oscillator used as the system clock */
  while(!(RCC->CFGR & (0x00000004)));
}

void peripheral_bus_config(void)
{
  /* AHB1ENR Advanced High Bus 1 (RCC_AHB1ENR) peripheral enable register	*/
  RCC->AHB1ENR |= (1 << 0);  // GPIOAEN : IO port A clock enable. 0th bit set 1

  RCC->APB2ENR |= (1 << 17);  // TIMER10EN : TIM10 clock enable. 17th bit set 1
}

void gpioa_moder_config(void)
{
  /* (GPIO) General Purpose Input and Output (GPIOA_MODER) port mode register */
  /* PORT A MODER0 0th pin select. 0th bit set 1. */
  GPIOA->MODER |= (1 << 0);   // 01 : General purpose output mode
}

void timer10_config(void)
{
  /* (TIM10_CR1) control register 1 */

  TIM10->CR1 |= (1 << 1);     // UDIS : update disable. 1 : UEV disabled

  TIM10->CR1 &= ~(1 << 2);    //  URS : Update request source. 0 : Any of following events generate an UEV if enabled

  TIM10->CR1 |= (1 << 7);     // ARPE : Auto-reload preload enable, 1 : TIM10_ARR register is buffered

  TIM10->CNT = 0;             // counter value 0
}

void timer10_delay(int val)
{
  TIM10->PSC = 999;          // prescaler division by 1 from 4

  TIM10->ARR = val;          // 1ms milliseconds Auto reload value on, 19ms milliseconds off

  TIM10->CR1 |= (1 << 0);    // CEN : Counter enable. 1: Counter enable

  while(!(TIM10->SR & (1 << 0)));   // Update interrupt flag.  1 : Update interrupt pending.

  TIM10->SR &= ~(1 << 0);   // clear by software
}

void led_blinking(void)
{
  /* (GPIO_ODR) output data register */
  int i = 25;  // 25 means 1ms milliseconds value
  for (int j = 475; j >= 25; j = j - 25)   // high to low led on and off
  {

    GPIOA->ODR |= (1 << 0);   // PORT A 0th pin select. 0th set 1. led on
    timer10_delay(j);         // 19ms milliseconds decrement 475
    GPIOA->ODR &= ~(1 << 0);  // PORT A 0th pin select. 0th clear 0. led of
    timer10_delay(i);         // 1ms milliseconds increment 25
    i = i + 25;
  }
}
