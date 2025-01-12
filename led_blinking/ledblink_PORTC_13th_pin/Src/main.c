	/**
 ******************************************************************************
 *
 *  Author      : Murugan M
 *  Date        : 16-12-2024  
 *  File        : PORTC_13th_pin
 *  Description : Led blink for PORTC 13th pin.  
 *                using for one pheripheral bus AHB1ENR 
 *
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


/* rcc config */

volatile unsigned int *RCC_CR = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

/* port config */

volatile unsigned int *GPIOC_MODER = (volatile unsigned int *)0x40020800;
volatile unsigned int *GPIOC_ODR = (volatile unsigned int *)0x40020814;

/* function declaration */

void rcc_config(void);
void peripheral_bus_config(void);
void gpioc_moder(void);
void led_blinking(void);
void delay_ms(int ms);

/* main function */

int main(void)
{
       /* function call */
       
       rcc_config();
       peripheral_bus_config();
       gpioc_moder();
       
       while(1)
       {
    	   led_blinking(); /* keep on run for led blinking */
       }
}

/* function defination */

void rcc_config()
{
	/* rcc register. starting page number is 102 */
	/* Clock Control Register (RCC_CR) */

	*RCC_CR = *RCC_CR & (~0x00010000);  /* HSEON : HSE clock disable, 16th bit  */

	*RCC_CR = *RCC_CR | (1 << 16);   /* HSEON : HSE clock enable, 16th bit  */

	while(!(*RCC_CR & 1 << 17) == 0);  /* HSERDY : HSE oscillator ready, external high speed clock ready flag */

	/* Clock Configuration Register (RCC_CFGR) */

	*RCC_CFGR = *RCC_CFGR & (~0x00000003);  /* clear system clock switch, 0th bit and 1st bit set value 11, after the clear */

	*RCC_CFGR = *RCC_CFGR | (1 << 0);  /* set HSE oscillator selected as system clock, 0th bit set 1 */

	while(!(*RCC_CFGR & (0x00000004)) == 4); /* HSE oscillator used as system clock, 2nd bit set 1, status checking */

}

void peripheral_bus_config()
{
	/* Advanced High-Performance Bus 1, peripheral clock enable register (RCC_AHB1ENR) */

	*RCC_AHB1ENR = *RCC_AHB1ENR |= (1 << 2); /* IO port c clock enable, 2nd bit set 1 */
}

void gpioc_moder()
{
	/* gpio register starting page number is 158 */
	/* port mode register (GPIOC_MODER) */

	*GPIOC_MODER = *GPIOC_MODER |= (1 << 26);  /*  01 : general purpose output mode, port c 13th pin in moder 13th pin 26th bit set 1 */
}

void delay_ms(int ms)
{
	/* delay */
	for (int i = 0; i < ms * 4000; i++)
	{
		__asm("NOP");   /* asm means assembly */
	}
}

void led_blinking()
{
	/* port output data register (GPIOC_ODR) */
	/* select the ODR 13th pin, set 1 and clear */
	/* PORTC 13th pin led */

	if (*GPIOC_ODR & 1 << 13)  /* condition checking */
	{
		*GPIOC_ODR = *GPIOC_ODR & ~(1 << 13); /* led on */
		delay_ms(1000);
	}
	else
	{
		*GPIOC_ODR = *GPIOC_ODR | (1 << 13); /* led off */
		delay_ms(100);
	}
}
