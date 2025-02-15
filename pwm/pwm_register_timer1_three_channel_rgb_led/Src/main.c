/******************************************************************************
 *                                                                            *
 *  Author      : Murugan M                                                   *
 *  Date        : 15-02-2025                                                  *
 *  File        : pwm_register_timer1_three_channel_rgb_led                   *
 *  Description : Led blink for PORTA 8th, 9th and 10th pin.                  *
 *                using for two pheripheral bus AHB1ENR and APB2ENR           *
 *                pwm three channel for TIM1_CH1, TIM1_CH2, TIM1_CH3          *
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
    pwm_timer1_config();
    while(1)
    {
    	purple_color_led();
        green_color_led();
    	white_color_led();
    	navy_color_led();
    	yellow_color_led();
    	red_color_led();
    	cyan_color_led();
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

	/* SW : System clock switch. 00 : HSI oscillator selected as system clock. 0th bit and 1st bit. set 00 */
	RCC->CFGR &= ~(3 << 0);

    /* SWS : System clock switch status. 00 : HSI oscillator used as the system clock. 2nd bit and 3rd bit. set 00  */
	while((RCC->CFGR & (3 << 2)) != (0 << 2));
}

void peripheral_bus_config(void)
{
	/* Advanced High Bus 1. peripheral clock enable register (RCC_AHB1ENR) */

	/* GPIOAEN : port A clock enable */
	RCC->AHB1ENR |= (1 << 0); // PORT GPIOAEN : port a clock enable

	/* Advanced peripheral Bus 2. peripheral clock enable register (RCC_APB2ENR) */

	/* TIM1EN: TIM1 clock enable */
	RCC->APB2ENR |= (1 << 0); // TIM1 TIM1EN : timer 1 clock enable
}

void gpioa_moder_config(void)
{
	/* General purpose input and output (GPIO) */
	/* GPIO port mode register (GPIOA_MODER) */

	/* PORT A MODER8. I select for port A 8th pin for TIMER 1 TIM1_CH1 timer1 first channel */
	GPIOA->MODER |= (1 << 17); // MODER8 Port A 8th pin. 10 : Alternate function mode

	/* PORT A MODER9. I select for port A 9th pin for TIMER 1 TIM1_CH2 timer1 second channel */
	GPIOA->MODER |= (1 << 19); // MODER9 Port A 9th pin. 10 : Alternate function mode

	/* PORT A MODER10. I select for port A 10th pin for TIMER 1 TIM1_CH3 timer1 third channel */
	GPIOA->MODER |= (1 << 21); // MODER10 Port A 10th pin. 10 : Alternate function mode

	/* output speed register (GPIOB_OSPEEDR) */

	GPIOA->OSPEEDR |= (1 << 17); // 10 : High speed
	GPIOA->OSPEEDR |= (1 << 19);
	GPIOA->OSPEEDR |= (1 << 21);

	/* pull-up/pull-down register (GPIOB_PUPDR) */
	GPIOA->PUPDR |= (1 << 17);  // 10 : Pull-down
	GPIOA->PUPDR |= (1 << 19);
	GPIOA->PUPDR |= (1 << 21);

	/* alternate function high register (GPIOB_AFRH) */
	GPIOA->AFRH |= (1 << 0);  // 0001 : AF1 timer1 pin. PORTA 8th pin for TIM1_CH1 timer1 first channel
	GPIOA->AFRH |= (1 << 4);  // 0001 : AF1 timer1 pin. PORTA 9th pin for TIM1_CH2 timer1 second channel
	GPIOA->AFRH |= (1 << 8);  // 0001 : AF1 timer1 pin. PORTA 10th pin for TIM1_CH3 timer1 third channel
}

void pwm_timer1_config(void)
{
	/* capture/compare mode register 1 (TIM1_CCMR1) */

	/* OC1M : output compare 1 mode. output pin select.
	* 110 : PWM mode 1 - in upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive. */
	TIM1->CCMR1 |= (6 << 4);  // pwm mode 1. OC1M. first pin TIM1_CH1

	/* OC1PE : output compare 1 preload enable.
	 * 0 : preload register on TIM1_CCR1 disabled. write OC1PE = 0 to disable preload register */
	TIM1->CCMR1 |= (1 << 3);  // OC1PE

	/* OC1FE : output compare 1 fast enable.
	 * 1 : An active edge on the trigger input acts like a compare match on CC1 output. */
	TIM1->CCMR1 |= (1 << 2);  // OC1FE

	/* CC1S : capture/compare 1 selection
	* 00 : CC1 channel is configured as output */
	TIM1->CCMR1 &= ~(3 << 0); // CC1S

	TIM1->CCMR1 |= (6 << 12); // pwm mode 1. OC2M. second pin TIM1_CH2
	TIM1->CCMR1 |= (1 << 11); // OC2PE
	TIM1->CCMR1 |= (1 << 10); // OC2FE
	TIM1->CCMR1 &= ~(3 << 8); // CC2S

	TIM1->CCMR2 |= (6 << 4);  // pwm mode 1. OC3M. third pin TIM1_CH3
	TIM1->CCMR2 |= (1 << 3);  // OC3PE
	TIM1->CCMR2 |= (1 << 2);  // OC3FE
	TIM1->CCMR2 &= ~(3 << 0); // CC3S

	/* auto-reload register (TIM1_ARR) */
	TIM1->ARR = 255;

	/* prescaler (TIM1_PSC)
	 * prescaler value : 1000 */
	TIM1->PSC = 999;

	/* counter (TIM10_CNT)
	 * starting with 0 */
	TIM1->CNT = 0;

	/* TIM1 event generation register (TIM1_EGR) */

	/* UG : update generation. 0th bit set 1
	 * CC1G: Capture/Compare 1 generation, 1sr bit set 1 */
	TIM1->EGR |= (3 << 0);

	/* capture/compare enable register (TIM1_CCER) */

	/* CC1E : Capture/compare 1 output enable.
	 * write CC1E : 1 to enable the output.
	 * 1 : on - OCC1 signal is output on the corresponding output pin */
	TIM1->CCER |= (1 << 0);  // CC1E

	/* CC1P : Capture/compare 1 output polarity.
	 * CC1 channel configured as output
	 * 0 : OCC1 active high
	 * write CC1P : 0 to select active high polarity  */
	TIM1->CCER &= ~(1 << 1); // CC1P


	TIM1->CCER |= (1 << 4);  // CC2E
	TIM1->CCER &= ~(1 << 5); // CC2P

	TIM1->CCER |= (1 << 8);  // CC3E
	TIM1->CCER &= ~(1 << 9); // CC3P

	/* break and dead-time register (TIM1_BDTR) */
	/* MOE : main output enable
	 * 1: OC and OCN outputs are enabled if their respective enable bits are set (CCxE, CCxNE in TIMx_CCER register). */
	TIM1->BDTR |= (1 << 15); // MOE

	/* control register 1 (TIM10_CR1)
	 * CEN : counter enabled */
	TIM1->CR1 |= (1 << 0);
}

void purple_color_led(void)
{
	/* purple color value : R = 128, G = 0, B = 128 */

	for (int i = 10; i <= 255; i = i + 5)   // led on 5ms. led off 5ms. ms means milliseconds
	{
		/* status register (TIM10_SR) */
		while(!(TIM1->SR & (1 << 0)));  // UIF: Update interrupt flag. 1: Update interrupt pending
		TIM1->SR &= ~(3 << 0);  // clear the status register. UIF 0th bit and CC1IF bit
		TIM1->CCR1 = i;          // value load to actual capture/compare 1 register (preload value)
		TIM1->CCR2 = 0;
		TIM1->CCR3 = i;
	}

	for (int j = 255; j >= 10; j = j - 5)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = j;
		TIM1->CCR2 = 0;
		TIM1->CCR3 = j;
	}
}

void green_color_led(void)
{
	/* green color value : R = 0, G = 255, B = 0 */

	for(int h = 16; h <= 255; h = h + 4)   // led on 16ms. led off 4ms. ms means milliseconds
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = 0;
		TIM1->CCR2 = h;
		TIM1->CCR3 = 0;
	}

	for(int b = 255; b >= 0; b = b - 4)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = 0;
		TIM1->CCR2 = b;
		TIM1->CCR3 = 0;
	}
}

void white_color_led(void)
{
	/* white color value : R = 255, G = 255, B = 255 */

	for(int c = 16; c <= 255; c = c + 4)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = c;
		TIM1->CCR2 = c;
		TIM1->CCR3 = c;
	}

	for(int w = 255; w >= 16; w = w - 4)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = w;
		TIM1->CCR2 = w;
		TIM1->CCR3 = w;
	}
}

void navy_color_led(void)
{
	/* navy color value : R = 0, G = 0, B = 128 */

	for(int k = 10; k <= 255; k = k + 5)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = 0;
		TIM1->CCR2 = 0;
		TIM1->CCR3 = k;
	}

	for(int m = 255; m >= 10; m = m - 5)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = 0;
		TIM1->CCR2 = 0;
		TIM1->CCR3 = m;
	}
}

void yellow_color_led(void)
{
	/* yellow color value : R = 255, G = 255, B = 0. */

	for(int r = 16; r <= 255; r = r + 4)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = r;
		TIM1->CCR2 = r;
		TIM1->CCR3 = 0;
	}

	for(int t = 255; t >= 16; t = t - 4)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = t;
		TIM1->CCR2 = t;
		TIM1->CCR3 = 0;
	}
}

void red_color_led(void)
{
	/* red color value : R = 255, G = 0, B = 0 */

	for(int p = 16; p <= 255; p = p + 4)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = p;
		TIM1->CCR2 = 0;
		TIM1->CCR3 = 0;
	}

	for(int q = 255; q >= 16; q = q - 4)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = q;
		TIM1->CCR2 = 0;
		TIM1->CCR3 = 0;
	}
}

void cyan_color_led(void)
{
	/* cyan color value : R = 0, G = 255, B = 255 */

	for(int z = 16; z < 255; z = z + 4)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = 0;
		TIM1->CCR2 = z;
		TIM1->CCR3 = z;
	}

	for(int w = 255; w >= 16; w = w - 4)
	{
		while(!(TIM1->SR & (1 << 0)));
		TIM1->SR &= ~(3 << 0);
		TIM1->CCR1 = 0;
		TIM1->CCR2 = w;
		TIM1->CCR3 = w;
	}
}
