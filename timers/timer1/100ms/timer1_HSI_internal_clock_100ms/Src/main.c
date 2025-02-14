/******************************************************************************
 *                                                                            *
 *  Author      : Murugan M                                                   *
 *  Date        : 14-02-2025                                                  *
 *  File        : timer1_HSI_internal_clock_100ms                             *
 *  Description : Led blink for PORTA 0th pin.                                *
 *                using for two pheripheral bus AHB1ENR and APB2ENR           *
 *	           	  i give 100ms milliseconds              			          *
 *                using for HSI : internal clock for 16 MHz      		      *
 *****************************************************************************/

#include "arm.h"  /* this header file for all register inside there. using for structure is user-defined data type */

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

	/* GPIOAEN : port A clock enable */
	RCC->AHB1ENR |= (1 << 0);  // port A clock enable

	/* Advanced peripheral Bus 2. peripheral clock enable register (RCC_APB2ENR) */

	/* TIM1EN: TIM1 clock enable */
	RCC->APB2ENR |= (1 << 0);
}

void gpioa_moder_config(void)
{
	/* General purpose input and output (GPIO) */
	/* GPIO port mode register (GPIOA_MODER) */

	/* PORT A MODER2. I select for port A 2nd pin for TIMER 1 */
	GPIOA->MODER |= (1 << 0);  // MODER 1 select. 01: General purpose output
}

void timer1_config(void)
{
	/* TIM1 control register 1 (TIM1_CR1) */

	/* CEN : counter disabled. 0th bit set 0 */
	TIM1->CR1 &= ~(1 << 0);

	/* URS : update request source. 2nd bit set 0 */
	TIM1->CR1 &= ~(1 << 2);

	/* UDIS : update disable. 1st bit set 1 : UEV disabled */
	TIM1->CR1 |= (1 << 1);

	/* ARPE : Auto-reload preload enable. 7th bit set 1 : TIM1_ARR register is buffered */
    TIM1->CR1 |= (1 << 7);

	/* TIM1 event generation register (TIM1_EGR) */

	/* UG : update generation. 0th bit set 1 */
    TIM1->EGR |= (1 << 0);

    /* TIM1 counter (TIM1_CNT) */
    TIM1->CNT = 0;   // counter starting with 0. I select upcounting mode
}

void timer1_delay(int val)
{
	/* TIM1 prescaler (TIM1_PSC) */
	TIM1->PSC = 999;  // prescaler value : 999 (or) 1000

	/* TIM1 auto-reload register (TIM1_ARR) */
	TIM1->ARR = val;   // auto-eload value : 16000 this value for 1seconds means 1000ms milliseconds

	/* TIM1 control register 1 (TIM1_CR1) */

	/* CEN : counter enabled. 0th bit set 1*/
	TIM1->CR1 |= (1 << 0);

	/* TIM1 status register 1 (TIM1_SR) */

	/* UF : update interrupt flag. 0th bit set 1. 1: update interrupt flag */
	while(!(TIM1->SR & (1 << 0)));

	/* UF : update interrupt flag. 0th bit set 0. 0 : no update occurred */
	TIM1->SR &= ~(1 << 0);
}

void led_blinking(void)
{
	/* General purpose Input and Output (GPIOA) */

	/* GPIO port output data register (GPIOA_ODR) */

	/* PORT A 0th pin set 1. led on*/
	GPIOA->ODR |= (1 << 0);

	/* 500ms led on. 100ms milliseconds value : 1600 */
	timer1_delay(1600);          // 100ms milliseconds value : 1600, 500ms milliseconds value : 8000, 1s seconds means 1000ms value : 16000

	/* PORT A 0th pin set 0. led off */
	GPIOA->ODR &= ~(1 << 0);

	/* 500ms led on. 100ms milliseconds value : 1600 */
	timer1_delay(1600);
}
