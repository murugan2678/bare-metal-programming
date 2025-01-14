/**************************************************************************************************
 *                                                                                                *
 * Author      : Murugan M                                                                        *
 * Date        : 14-01-2025                                                                       *
 * File        : PA1_PB0_interrupt_NVIC_EXTI                                                      *
 * Description : Select Hardware Internal Pull-Down Resistor PUPDR                                *
 *               NVIC Nested vector interrupt controller                                          *
 *               Using for two External interrupt EXTI0 and EXTI1                                 *
 *               Input take for GPIOB0 PORT B 0th pin connect to first push button                *
 *               Output take for GPIOB1 PORT B 2nd pin. connect to first led                      *
 *               Input take for GPIOA1 PORT A 1st pin connect to second push button               *
 *               Output take for GPIOA0 PORT A 0th pin connect to second led.                     *
 *               Interrupt priority register x (NVIC_IPRx) priority using 		     			  *
 *               																				  *
 *************************************************************************************************/






/* rcc register */
volatile unsigned int *RCC_CR = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR =  (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;
volatile unsigned int *RCC_APB2ENR = (volatile unsigned int *)0x40023844;

/* gpio register PORT A */
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;
volatile unsigned int *GPIOA_PUPDR = (volatile unsigned int *)0x4002000C;

/* gpio register PORT B */
volatile unsigned int *GPIOB_MODER = (volatile unsigned int *)0x40020400;
volatile unsigned int *GPIOB_ODR   = (volatile unsigned int *)0x40020414;
volatile unsigned int *GPIOB_PUPDR = (volatile unsigned int *)0x4002040C;

/* EXTI register */
volatile unsigned int *EXTI_IMR = (volatile unsigned int *)0x40013C00;  // 0x4001 3C00
volatile unsigned int *EXTI_RTSR = (volatile unsigned int *)0x40013C08;
volatile unsigned int *EXTI_PR   = (volatile unsigned int *)0x40013C14;

/* system config */
volatile unsigned int *SYSCFG_EXTICR1 = (volatile unsigned int *)0x40013808;  // 0x4001 3800

/* NVIC register */
volatile unsigned int *NVIC_ISER0 = (volatile unsigned int *)0xE000E100; // 0xE000E100;

/* interrupt priority register */
volatile unsigned int *NVIC_IPR1 = (volatile unsigned int *)0xE000E404;

volatile unsigned char *NVIC_IP6 = (volatile unsigned char *)0xE000E406;  /* interrupt priority IP6. NVIC table EXT0 position for 6 */
volatile unsigned char *NVIC_IP7 = (volatile unsigned char *)0xE000E407;  /* interrupt priority IP7. NVIC table EXT1 position for 7 */

/* function deceleration */
void rcc_config(void);
void peripheral_bus_config(void);
void gpioa_gpiob_moder_config(void);
void exti_config(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void delay_ms(int ms);

/* main function */
int main(void)
{
    /* function call */
	rcc_config();
	peripheral_bus_config();
	gpioa_gpiob_moder_config();
	exti_config();
	while(1)
	{
		*GPIOB_ODR |= (1 << 2);
		*GPIOA_ODR |= (1 << 0);
	}
}

/* function definition */
void rcc_config(void)
{
	/* RCC Reset Clock Controller. (RCC_CR) clock control register */

	/* set 1 and clear 0, 16th bit */
	*RCC_CR &= (~0x00010000);

	/* set 1 16th bit. HSEON : clock enable. 1 : HSE oscillator enable. HSE means External oscillator Enable  */
	*RCC_CR |= (1 << 16);

	/* set 1 17th bit. HSERDY : clock ready flag. 1 : HSE oscillator ready. ready or checking flag */
	while(!(*RCC_CR & (1 << 17)));

	/* RCC_CFGR this register for Data sheet Reference manual page number : 107, 108 and 109 */

	/* (RCC_CFGR) clock configuration register */

	/* set 1 and clear 0, 0th bit and 1st bit. 11 : not allowed */
	*RCC_CFGR &= (~0x00000003);

	/* set 1 0th bit. SW : System clock switch. 01 : HSE oscillator selected as system clock */
	*RCC_CFGR |= (1 << 0);

	/* set 1 2nd bit. SWS : System clock switch status. 01 : HSE oscillator used as system clock */
	while(!(*RCC_CFGR & (1 << 2)));
}

void peripheral_bus_config(void)
{
	/* (RCC_AHB1ENR) Advanced High Bus 1 Enable Register */

	/* set 1 0th bit. GPIOAEN : IO port A clock enable. 1 : IO port A clock enabled */
	*RCC_AHB1ENR |= (1 << 0);

	/* set 1 1st bit. 1GPIOBEN : IO port B clock enable. 1 : IO port B clock enabled */
	*RCC_AHB1ENR |= (1 << 1);

	/* (RCC_APB2ENR) Advanced Peripheral Bus 2 Enable Register */

	/* set 1 14th bit. SYSCFGEN : System configuration controller clock enable */
	*RCC_APB2ENR |= (1 << 14);
}

void gpioa_gpiob_moder_config(void)
{
	/* (GPIO) General purpose input and output.(GPIOA_MODER) port mode register */

	/* PORT A select MODER0 0th pin. set 1 0th bit. 01 : General purpose output mode */
	*GPIOA_MODER |= (1 << 0);  /* PA0 this for led */

	/* PORT B select MODER1 2nd pin. set 1 4th bit. 01 : General purpose output mode */
	*GPIOB_MODER |= (1 << 4);  /* PB2 this for led */

	/* (GPIOx_PUPDR) port pull-up/pull-down register */

	/* PORT B. select PUPDR0 0th pin. set 1 1st bit. 10 : Pull-down */
	*GPIOB_PUPDR |= (1 << 1);  /* PB0. 10 : pull-down */

	/* PORT A. select PUPDR1 1st pin. set 1 3rd bit. 10 : Pull-down */
	*GPIOA_PUPDR |= (1 << 3);  /* PA1. 10 : pull-down */
}

void delay_ms(int ms)
{
	/* delay */
	for(int k = 0; k < ms * 1000; k++)
	{
		__asm("NOP");   /* asm means Assembly Language. NOP no operation */
	}
}

void exti_config(void)
{
	/* EXTI 1 0000 : PA[1] pin */

	/* (SYSCFG) System configuration controller */

	/* set and clear for EXTI1 */
	*SYSCFG_EXTICR1 &= (~0x0F << 0);

	/* select the source input for the EXTI1, 0000 : PA[x] pin. select PORTA 1st pin. set 4th bit 0 */
	*SYSCFG_EXTICR1 |= (0 << 4);

	/* (EXTI) External interrupt */

	/* (EXTI_IMR) Interrupt mask register.  MR0 1st bit set 1. 1 : interrupt request from line x is not allowed */
	*EXTI_IMR  |= (1 << 1);

	/* (EXTI_RTSR) Rising trigger selection register. TR0 0th bit set 1. 1 : Rising trigger enabled (for Event and interrupt) for input line*/
	*EXTI_RTSR |= (1 << 1);

	/* (NVIC) Nested vector interrupt controller */

	/* (NVIC_ISER0) Interrupt set-enable register 1 */
	*NVIC_ISER0 |= (1 << 7);  /* NVIC table EXT1 position 7. EXTI Line 1 interrupt*/

	/* (NVIC_IPR1) Interrupt priority register  */
	*NVIC_IP7 |= (1 << 6);  /* priority bit set position. IP7 Interrupt priority */

	/* EXTI 0 0001 : PB[0] pin */

	/* set and clear for EXTI0. 0bit to 3rd bit */
	*SYSCFG_EXTICR1 &= (~0x0F << 0);

	/* select the source input for the EXTI0, 0001 : PB[x] pin. select PORTB 0th pin. set 0th bit 1 */
	*SYSCFG_EXTICR1 |= (1 << 0);     /* 0001 : PB[0] pin */

	/* (EXTI) External interrupt.(EXTI_IMR) Interrupt mask register.
	 * MR0 0th bit set 1. 1 : interrupt request from line x is not allowed */
	*EXTI_IMR  |= (1 << 0);

	/* (EXTI_RTSR) Rising trigger selection register. TR0 0th bit set 1. 1 : Rising trigger enabled (for Event and interrupt) for input line */
	*EXTI_RTSR |= (1 << 0);

	/* NVIC_ISER0 this register for Data sheet program manual STM32 Cortex-M4 page number : 210 */

    /* (NVIC) Nested vector interrup controller. (NVIC_ISER0) Interrupt set-enable register 0.
     *  EXTI0 position for 6 */
	*NVIC_ISER0 |= (1 << 6);      /* NVIC table EXTI0 position 6. EXTI Line 0 interrupt */
	*NVIC_IP6 |= (1 << 7);     /* priority bit set position. IP6 Interrupt priority */
}

void EXTI0_IRQHandler(void)
{
	/* (EXTI_PR) Pending register */
	if (*EXTI_PR & (1 << 0)) /* checking for trigger. NVIC set or not for pending register */
	{
		for(int i = 0 ; i < 5; i++)  /* five time running for LED */
		{
			*GPIOB_ODR |= (1 << 2);   /* PORTB 1st pin. 2nd bit set 1. LED ON */
			delay_ms(300);            /* 300ms milliseconds stop */
			*GPIOB_ODR &= ~(1 << 2);  /* LED OFF*/
			delay_ms(300);
		}
		*EXTI_PR |= (1 << 1);   /* this bit is cleared by programming it to '1' */
	}
}

void EXTI1_IRQHandler(void)
{
	/* (EXTI_PR) Pending register */
	if (*EXTI_PR & (1 << 1))   /* checking for trigger. NVIC set or not for pending register */
	{
		for(int j = 0; j < 5; j++)    /* five time running for LED */
		{
			*GPIOA_ODR |= (1 << 0);   /* PORTA 0th pin.  0th bit set 1. LED ON */
			delay_ms(300);            /* 300ms milliseconds stop */
			*GPIOA_ODR &= ~(1 << 0);  /* LED OFF */
			delay_ms(300);
		}
		*EXTI_PR |= (1 << 1);   /* this bit is cleared by programming it to '1' */
	}
}
