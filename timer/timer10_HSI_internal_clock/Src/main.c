/******************************************************************************
 *                                                                            *
 *  Author      : Murugan M                                                   *
 *  Date        : 06-02-2025                                                  *
 *  File        : timer10_HSI_internal_clock                                  *
 *  Description : Led blink for PORTB 8th pin.                                *
 *                using for two pheripheral bus AHB1ENR and APB2ENR           *
 *		  i give 100ms	milli seconds				      *
 *                using for HSI : internal clock for 16 MHz   		      *
 *****************************************************************************/


#include "arm.h"  /* this header file for all register inside there. using for structure is user-defined data type */


/* main function */
int main(void)
{
  /* function call */
  rcc_config();
  peripheral_bus_config();
  gpioa_moder_config();
  //timer10_config();
  while(1)
  {
    led_blinking();
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

  /* HSIRDY : Internal high-speed clock ready flag. 1st bit set 1. 1 : HSI oscillator ready */
  while(!(RCC->CR & (1 << 1)));

  /* Reset clock control configuration register (RCC_CFGR) */

  /* SW : System clock switch. 00 : HSI oscillator selected as system clock. 0th bit and 1st bit. set 00 */
  RCC->CFGR &= ~(3 << 0);

   /* SWS : System clock switch status. 00 : HSI oscillator used as the system clock. 2nd bit and 3rd bit. set 00  */
  while((RCC->CFGR & (3 << 2)) != (0 << 2));
}

void peripheral_bus_config(void)
{
  /* Advanced High Bus 1. peripheral clock enable register (RCC_AHB1ENR) */

  /* GPIOAEN : port b clock enable */
  RCC->AHB1ENR |= (1 << 1);

  /* Advanced peripheral Bus 2. peripheral clock enable register (RCC_APB2ENR) */

  /* TIMER10 : port b TIMER10 clock enable */
  RCC->APB2ENR |= (1 << 17);
}

void gpioa_moder_config(void)
{
  /* PORT B MODER8. I select for port B 8th pin for TIMER 10 */
  GPIOB->MODER |= (1 << 16);
}

void timer10_config(void)
{
  /* TIMER10 control register 1 (TIM10_CR1) */
 
  TIM10->CR1 &= ~(1 << 2); //  URS : Update request source. 0 : Any of following events generate an UEV if enabled

  TIM10->CR1 |= (1 << 1);  // UDIS : update disable. 1 : UEV disabled

  TIM10->CR1 |= (1 << 7);  // ARPE : Auto-reload preload enable, 1 : TIM10_ARR register is buffered

  /* TIMER10 counter (TIM10_CNT) */
  TIM10->CNT = 0;          // counter value starting with  0
}

void timer10_delay(int val)
{
  /* TIMER10 auto-reload register (TIM10_ARR) */
  TIM10->ARR = val;   // 100ms  Auto reload value

  /* TIMER10 prescaler (TIM10_PSC) */
  TIM10->PSC = 999;

  /* TIMER10 control register (TIM10_CR1) */
  /* CEN : Counter enable. 1 : Counter enable */
  TIM10->CR1 |= (1 << 0);

  /* TIMER10 status register (TIM10_EGR) */
  /* UIF : Update interrupt flag. set 1, 1 : Update interrupt pending. */
  while(!(TIM10->SR & (1 << 0)));

  /* clear the status registers. set 0.  0 : No update occurred.*/
  TIM10->SR &= ~(1 << 0);
}

void led_blinking(void)
{

  GPIOB->ODR |= (1 << 8);    // led on
  timer10_delay(1600);       // 100ms. calculate HSI internal clock frequency for 16MHz
  GPIOB->ODR &= ~(1 << 8);   // led off
  timer10_delay(1600);       // 100ms. calculate HSI internal clock frequency for 16MHz
}

