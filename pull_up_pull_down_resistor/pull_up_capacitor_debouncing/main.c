
/*********************************************************************************************************************
 *                                                                                                                   *
 * Author      : Murugan M                                                                                           *
 * Date        : 3-01-2025                                                                                           *
 * File        : pull_up_capcitor_debouncing                                                                         *
 * Description : Pull-up select Hardware Internal Resistor PUPDR                                                     *
 *               Input take for GPIOA0 PORT A 0th pin.                                                               *
 *               Output take for GPIOA2 PORT A 2nd pin.                                                              *
 *               Output 1k resistor using connect to LED (+) VCC Positive and (-) GND Ground                         *
 *               Using for one peripheral bus AHB1ENR                                                                *
 *               External using ceramic capacitor 104 using Debouncing is removing unwanted input noise from buttons *
 *               smoothness giving                                                                                   *
 *														     *
 ********************************************************************************************************************/

/* RCC register */
volatile unsigned int *RCC_CR = (volatile unsigned int *)0x40023800;   /* Add with RCC register base address and CR register. Address offset value 0x00 */
volatile unsigned int *RCC_CFGR = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

/* GPIO register */
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_IDR   = (volatile unsigned int *)0x40020010;
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;
volatile unsigned int *GPIOA_PUPDR = (volatile unsigned int *)0x4002000C;

/* function deceleration */
void rcc_config(void);
void peripheral_bus_config(void);
void gpioa_moder_config(void);
void delay_ms(int ms);
void led_blinking(void);

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

void rcc_config(void)
{
	/* Reset clock control (RCC_CR) clock control register */

    /* set and clear 16th bit */
	*RCC_CR &= (~0x00010000);

	/* select 16th bit HSEON : HSE clock enable. 1 : HSE oscillator ON */
	*RCC_CR |= (1 << 16);

	/* select 17th bit HSERDY : HSE clock ready flag. 1 : HSE oscillator ready. HSE external oscillator ready or not checking */
	while(!(*RCC_CR & (1 << 17)));

	/* Reset clock control (RCC_CFGR) clock configuration register */

	/* set and clear 0th bit and 1st bit. 11 : not allowed */
	*RCC_CFGR &= (~0x00000003);

	/* select 0th bit SW  : System clock switch. 01 : HSE oscillator selected as system clock */
	*RCC_CFGR |= (1 << 0);

	/* select 2nd bit SWS : System clock switch status. 01 : HSE oscillator used as the system clock */
	while(!(*RCC_CFGR & (1 << 2)));
}

void peripheral_bus_config(void)
{
	/* Advanced peripheral Bus 1 (RCC_AHB1ENR) peripheral clock enable  register */

	/* select 0th bit GPIOAEN : Input/Output  port A enable */
	*RCC_AHB1ENR |= (1 << 0);
}

void gpioa_moder_config(void)
{
	/* GPIO General Purpose Input and Output. GPIOA_MODER port mode register */

    /* GPIOA_MODER 00 : Input (reset state). select MODER0 0th pin 0th bit set 0 */
    *GPIOA_MODER &= (~3 << 0);

    /* GPIOA_PUPDR 01 : Pull-up. select MODER0 0th pin 0th bit set 1 */
    *GPIOA_PUPDR |= (1 << 0);

	/* GPIOA_MODER 01 : General purpose output mode.
	 * MODER2 2nd pin 4th bit set 1 */
    *GPIOA_MODER |= (1 << 4);
}

void delay_ms(int ms)
{
	/* delay */
	for (int i = 0; i < ms * 1000; i++)
	{
		__asm("NOP");
	}
}

void led_blinking(void)
{
	/* General Purpose Input and Output (GPIOA_IDR) input data register */
	/* select IDR. PORTB 0th pin select */

	/* step 1 : this condition Pull-Up means High 1. push button not press means condition True means 1 */
	if ((*GPIOA_IDR & (1 << 0)) == 1)
	{
		/* General Purpose Input and Output (GPIOA_ODR) output data register */
		/* select ODR2. PORTA 2nd pin output select. */
		*GPIOA_ODR |= (1 << 2);
		delay_ms(100);   /* led on time delay 200ms milliseconds */
	}
	/* step 2 : Push Button press means condition False means 0 */
	if ((*GPIOA_IDR & (1 << 0)) == 0)
	{
		*GPIOA_ODR &= (~1 << 2);  /* led off */
	}
}
