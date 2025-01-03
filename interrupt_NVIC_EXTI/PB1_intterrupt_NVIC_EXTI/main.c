/**********************************************************************************************************
 *                                                                                                        *
 * Author      : Murugan M                                                                                *
 * Date        : 03-1-2025                                                                                *
 * File        : PB1_interrupt_NVIC_EXTI                                                                  *
 * Description : Select Hardware Internal Pull-Up Resistor PUPDR, FTSR Falling trigger selection register *
 *               NVIC Nested vector interrupt controller                                                  *
 *               Using for one External interrupt EXTI1 Line 1 interrupt,                                 *
 *               																				          *
 *********************************************************************************************************/

/* RCC Register */
volatile unsigned int *RCC_CR = (volatile unsigned int *)0x40023800;   /* Add with RCC register base address and CR register. Address offset value 0x00 */
volatile unsigned int *RCC_CFGR = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;
volatile unsigned int *RCC_APB2ENR = (volatile unsigned int *)0x40023844;

/* GPIOA Register */
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;

/* GPIOB Register */
volatile unsigned int *GPIOB_PUPDR = (volatile unsigned int *)0x4002040C;

/* EXTI External Interrupt Register */
volatile unsigned int *EXTI_IMR  = (volatile unsigned int *)0x40013C00;
volatile unsigned int *EXTI_FTSR = (volatile unsigned int *)0x40013C0C;
volatile unsigned int *EXTI_PR   = (volatile unsigned int *)0x40013C14;

/* SYSCFG System configuration controller */
volatile unsigned int *SYSCFG_EXTICR1 = (volatile unsigned int *)0x40013808;

/* NVIC Nested Vector Interrupt Controller */
volatile unsigned int *NVIC_ISER0 = (volatile unsigned int *)0xE000E100;

/* function deceleration */
void rcc_config(void);
void peripheral_bus_config(void);
void gpiob_moder_config(void);
void delay_ms(int ms);
void exti1_config(void);
void EXTI1_IRQHandler(void);

/* main function */
int main(void)
{
	/* function call */
	rcc_config();
	peripheral_bus_config();
	gpiob_moder_config();
	exti1_config();
	while(1)
    {
		*GPIOA_ODR |= (1 << 0);
	}
}

/* function definition */
void rcc_config(void)
{
	/* RCC Reset Clock Controller. (RCC_CR) clock control register */

	/* set 1 and clear 0, 16th bit.  */
	/*set 1 16th bit. HSEON : clock enable. 1 : HSE oscillator enable. HSE means External oscillator Enable */
	*RCC_CR = (*RCC_CR & (~0x00010000)) | (1 << 16);  /* 0 | 1 = 1. symbol | Bitwise OR gate any 1 means condition true */

	/* set 1 17th bit. HSERDY : clock ready flag. 1 : HSE oscillator ready. ready or checking flag */
	while(!(*RCC_CR & (1 << 17)));

	/* (RCC_CFGR) clock configuration register */

    /* set 1 and clear 0, 0th bit and 1st bit. 11 : not allowed */
	/* set 1 0th bit. SW : System clock switch. 01 : HSE oscillator selected as system clock */
	*RCC_CFGR = (*RCC_CFGR & (~0x00000003)) | (1 << 0);  /* 0 | 1 = 1. symbol | Bitwise OR gate any 1 means condition true */

	/* set 1 2nd bit. SWS : System clock switch status. 01 : HSE oscillator used as system clock */
	while(!(*RCC_CFGR & (1 << 2)));
}

void peripheral_bus_config(void)
{
	/* (RCC_AHB1ENR) Advanced High Bus 1 Enable Register */

	/* PORT A clock enable and PORT B clock enable */
	*RCC_AHB1ENR = (*RCC_AHB1ENR | (1 << 0)) | (1 << 1);

	/* (RCC_APB2ENR) Advanced Peripheral Bus 2 Enable Register */

	/* set 1 14th bit. SYSCFGEN : System configuration controller clock enable */
	*RCC_APB2ENR |= (1 << 14);  /* SYSCFG enable */
}

void gpiob_moder_config(void)
{
	/* GPIO General purpose input and output. (GPIOA_MODER) port mode register */

	/* PORT A select MODER0, 01 : General purpose output mode, 0th set 1 */
    *GPIOA_MODER |= (1 << 0);

    /* PORT B. select PUPDR0 1st pin. set 1 2nd bit. 01 : Pull-up */
	*GPIOB_PUPDR |= (1 << 2);
}

void delay_ms(int ms)
{
	/* delay */
	for(int i = 0; i < ms * 1000; i++)
	{
		__asm("NOP");   /* asm means Assembly Language. NOP no operation */
	}
}

void exti1_config(void)
{
	/* (SYSCFG) System configuration controller (SYSCFG_EXTICR1) external interrupt configuration register 1
	* set and clear for EXTI1. 4th bit to 7th bit. 4th bit set 1 means 0001 : PB[x] pin enable */
	*SYSCFG_EXTICR1 = (*SYSCFG_EXTICR1 & (~0x0F << 4)) | (1 << 4);


	/* (EXTI) External interrupt. (EXTI_IMR) Interrupt mask register.
	 * PORTB B1 1st pin. MR1 1st bit set 1. 1 : interrupt request from line x is not allowed */
	*EXTI_IMR |= (1 << 1);

	/* (EXTI_FTSR) Falling trigger selection register.
	 * PORTB B1 1st pin. TR1 1st bit set 1. 1: Falling trigger enabled (for Event and Interrupt) for input line.*/
    *EXTI_FTSR |= (1 << 1);

    /* (NVIC) Nested vector interrupt controller. (NVIC_ISER0) Interrupt set-enable register 0.
     * EXTI1 position for 7 */
	*NVIC_ISER0 |= (1 << 7);
}

/* line number  152 : .word EXTI1_IRQHandler. this function for assembly file there startup_stm32f01ccux.s, .s means assembly file
 * EXTI Line1 interrupt */
void EXTI1_IRQHandler(void)
{
	 /* (EXTI_PR) Pending register */
	if(*EXTI_PR & (1 << 1))  /* checking for trigger. NVIC set or not for pending register */
	{
		for(int j = 0; j < 5; j++) /* five time running for LED */
		{
			/* (GPIO_ODR) General Purpose output data register */
			*GPIOA_ODR |= (1 << 0);  /* PORTA 0th pin. 6th bit set 1. LED ON */
			delay_ms(200);           /* led on time delay 200ms milliseconds */
			*GPIOA_ODR &= ~(1 << 0); /* led off*/
			delay_ms(200);           /* led off time delay 200ms milliseconds */
		}
		*EXTI_PR |= (1 << 1);  /* this bit is cleared by programming it to '1' */
	}
}
