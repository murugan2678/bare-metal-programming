
/*******************************************************************************
 *                                                                             * 
 * Author      : Murugan M                                                     *
 * Date        : 17-12-2024                                                    *
 * File        : PORTB_0th_pin                                                 *
 * Description : Led blink for PORTB 0th pin.                                  *
 *               using for one pheripheral bus AHB1ENR                         *
 *                                                                             *
 *******************************************************************************/


/* rcc register */

volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;  /* this for RCC base address 0x40023800. CR offset value 0x00 */
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;  /* CFGR offset value 0x08. add with RCC base address and CFGR offset value */
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;  /* AHB1ENR offset value 0x30 */

/* gpio register */

volatile unsigned int *GPIOB_MODER = (volatile unsigned int *)0x40020400;  /* this for GPIOB base address 0x40020400. MODER offset value 0x00 */
volatile unsigned int *GPIOB_ODR   = (volatile unsigned int *)0x40020414;  /* ODR offset value 0x14. add with ODR base address and ODR  offset value */

/* function declaration */

void rcc_config(void);
void peripheral_bus(void);
void gpiob_moder_config(void);
void delay_ms(int ms);
void led_blinking(void);

int main(void)
{
    /* function call */

	rcc_config();
	peripheral_bus();
	gpiob_moder_config();

	while(1)  /* continuously running */
	{
		led_blinking(); /* led blinking */
	}
}

/* function defination */

void rcc_config()
{
	/* Clock control register (RCC_CR). starting page number is 103 */

	*RCC_CR = *RCC_CR & (~0x00010000); /* set and clear for 16th bit for HSE clock, HSE - external clock */

	*RCC_CR = *RCC_CR | (1 << 16); /* HSE clock enable, 16th bit set 1, 1 : HSE oscillator ON */

	while(!(*RCC_CR & 1 << 17) == 0); /* HSERDY HSE clock ready flag, 17th bit set 1. 1 : HSE oscillator ready, checking ready or not */


	/* Clock configuration register (RCC_CFGR) */

	*RCC_CFGR = *RCC_CFGR & (~0x00000003); /* SW : system clock switch. 0th bit and 1st bit set and clear bit */

	*RCC_CFGR = *RCC_CFGR | (1 << 0); /* SW : system clock switch, 01 HSE oscillator selected as system clock, set 0th bit for */

	while(!(*RCC_CFGR & 0x00000004) == 4); /* SWS : system clock switch status, checking status, 2nd bit set 1. select this 01 HSE oscillator used as the system clock */


}

void peripheral_bus()
{
	/* Advanced peripheral bus 1 (RCC_AHB1ENR) peripheral clock enable register */

	*RCC_AHB1ENR = *RCC_AHB1ENR | (1 << 1); /* GPIOBEN : IO port B clock enable, 1st bit set 1 */


}

void gpiob_moder_config()
{
	/* General purpose input and output (GPIOB_MODER) port mode register. starting page number is 158 */

	*GPIOB_MODER = *GPIOB_MODER |= (1 << 0); /* select MODER 0th bit. select 01 : General purpose output mode, 0th bit set 1 */


}

void delay_ms(int ms)
{
	/* delay */
	for (int i = 0; i < ms * 4000; i++)
	{
		__asm("NOP");
	}
}
void led_blinking()
{
	/* port output data register (GPIOB_ODR), select ODR0 0th pin, 0th pin set 1 */

	*GPIOB_ODR = *GPIOB_ODR | (1 << 0);  /* led ON, set value 1*/
	delay_ms(300);

	*GPIOB_ODR = *GPIOB_ODR & ~(1 << 0);  /* led OFF, clear the value */
	delay_ms(300);
}
