/************************************************************************************************************
 *                                                                                                          *
 * Author      : Murugan M                                                                                  *
 * Date        : 03-1-2025                                                                                  *
 * File        : PB0_PB1_PB15_interrupt_NVIC_EXTI                                                           *
 * Description : Select Hardware Internal Pull-Down Resistor PUPDR, RTSR  Rising trigger selection register *
 *               NVIC Nested vector interrupt controller                                                    *
 *               Using for one External interrupt EXTI0 Line 0 interrupt,                                   *
 *               EXTI1 Line 1 interrupt, EXTI15_10 Line[15 :10] interrupt                                   *
 *               																				            *
 ***********************************************************************************************************/

/* RCC Register */
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;   /* Add with RCC register base address and CR register. Address offset value 0x00 */
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;
volatile unsigned int *RCC_APB2ENR = (volatile unsigned int *)0x40023844;

/* GPIOB Register */
volatile unsigned int *GPIOB_MODER = (volatile unsigned int *)0x40020400;
volatile unsigned int *GPIOB_ODR   = (volatile unsigned int *)0x40020414;
volatile unsigned int *GPIOB_PUPDR = (volatile unsigned int *)0x4002040C;

/* EXTI Register */
volatile unsigned int *EXTI_IMR  = (volatile unsigned int *)0x40013C00;
volatile unsigned int *EXTI_RTSR = (volatile unsigned int *)0x40013C08;
volatile unsigned int *EXTI_PR   = (volatile unsigned int *)0x40013C14;

/* SYSCFG Register */
volatile unsigned int *SYSCFG_EXTICR1 = (volatile unsigned int *)0x40013808;
volatile unsigned int *SYSCFG_EXTICR4 = (volatile unsigned int *)0x40013814;

/* NVIC Register */
volatile unsigned int *NVIC_ISER0 = (volatile unsigned int *)0xE000E100;
volatile unsigned int *NVIC_ISER1 = (volatile unsigned int *)0xE000E104;

/* function deceleration */
void rcc_config(void);
void peripheral_bus_config(void);
void gpiob_moder_config(void);
void delay_ms(int ms);
void exti0_config(void);
void exti1_config(void);
void exti15_config(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI15_10_IRQHandler(void);

/* main function */
int main(void)
{
   /* function call */
    rcc_config();
    peripheral_bus_config();
    gpiob_moder_config();
    exti15_config();
    exti1_config();
    exti0_config();

    while(1)
    {
    	*GPIOB_ODR |= (1 << 5);
    	*GPIOB_ODR |= (1 << 6);
    	*GPIOB_ODR |= (1 << 7);
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

	/* set 1 1st bit. GPIOBEN : IO port B clock enable. 1 : IO port B clock enabled */
	*RCC_AHB1ENR |= (1 << 1);  /* PORT B I/O clock enable */

	/* (RCC_APB2ENR) Advanced Peripheral Bus 2 Enable Register */

	/* set 1 14th bit. SYSCFGEN : System configuration controller clock enable */
	*RCC_APB2ENR |= (1 << 14); /* SYSCFG clock enable */
}

void gpiob_moder_config(void)
{
	/* GPIO General purpose input and output. (GPIOA_MODER) port mode register */

	/* PORT B select MODER5, MODER6, MODER7. 01 : General purpose output mode, 10th bit, 12th bit, 14th bit set 1 */
	*GPIOB_MODER |= (0x00005400); /* MODER select for 5th, 6th and 7th,*/

	/* (GPIOx_PUPDR) port pull-up/pull-down register */

	/* PORT B. select PUPDR0 0th pin, PUPDR1 1st pin. PUPDR15	 */
	/* set 1 1st bit. 10 : Pull-down, set 1 3rd bit. 10 : Pull-down, set 1 31th bit. 10 : Pull-down */
	*GPIOB_PUPDR = ((*GPIOB_PUPDR | (1 << 1)) | (*GPIOB_PUPDR | (1 << 3))) | (*GPIOB_PUPDR | (1 << 31));
}

void delay_ms(int ms)
{
	/* delay */
	for (int i = 0; i < ms * 1000; i++)
	{
		__asm("NOP"); /* asm means Assembly Language. NOP no operation */
	}
}

/* select exti0_config PORTB 0th pin select */
void exti0_config(void)
{
	/* (SYSCFG) System configuration controller (SYSCFG_EXTICR1) external interrupt configuration register 1
	 * set and clear for EXTI0. 0th bit to 3rd bit. 0th bit set 1 means 0001 : PB[x] pin enable */
	*SYSCFG_EXTICR1 = (*SYSCFG_EXTICR1 & (~0x0F << 0)) | (1 << 0); /* PORT B B0 0th pin. 0 | 1 = 1. symbol | Bitwise OR gate any 1 means condition true */

	/* (EXTI) External interrupt. (EXTI_IMR) Interrupt mask register.
	 * PORTB B0 0th pin. MR0 0th bit set 1. 1 : interrupt request from line x is not allowed */
	*EXTI_IMR |= (1 << 0);

    /* (EXTI_RTSR) Rising trigger selection register.
	 * PORTB B0 0th pin. TR0 0th bit set 1. 1 : Rising trigger enabled (for Event and interrupt) for input line */
	*EXTI_RTSR |= (1 << 0);

	/* (NVIC) Nested vector interrupt controller. (NVIC_ISER0) Interrupt set-enable register 0.
	 *  EXTI0 position for 6 */
	*NVIC_ISER0 |= (1 << 6);  /* EXTI Line 0 interrupt */
}

/* line number  151 : .word EXTI0_IRQHandler. this function for assembly file there startup_stm32f01ccux.s, .s means assembly file
 * EXTI Line0 interrupt */
void EXTI0_IRQHandler(void)  /* IRQ means Interrupt Request Handler */
{
	 /* (EXTI_PR) Pending register */
	if(*EXTI_PR & (1 << 0))  /* checking for trigger. NVIC set or not for pending register */
	{
		for(int j = 0; j < 5; j++)  /* five time running for LED */
		{
			/* (GPIO_ODR) General Purpose output data register */
			*GPIOB_ODR |= (1 << 5);   /* PORTB 5th pin. 5th bit set 1. LED ON */
			delay_ms(200);            /* led on time delay 200ms milliseconds */
			*GPIOB_ODR &= ~(1 << 5);  /* led off */
			delay_ms(200);            /* led off time delay 200ms milliseconds */
		}
		*EXTI_PR |= (1 << 0);   /* this bit is cleared by programming it to '1' */
	}
}

/* select exti1_config PORTB 1st pin select */
void exti1_config(void)
{
	/* (SYSCFG) System configuration controller (SYSCFG_EXTICR1) external interrupt configuration register 1
	 * set and clear for EXTI1. 4th bit to 7th bit. 4th bit set 1 means 0001 : PB[x] pin enable */
	*SYSCFG_EXTICR1 = (*SYSCFG_EXTICR1 & (~0x0F << 4)) | (1 << 4);   /* PORT B B1 1st pin. 0 | 1 = 1. symbol | Bitwise OR gate any 1 means condition true */

	/* (EXTI) External interrupt. (EXTI_IMR) Interrupt mask register.
	 * PORTB B1 1st pin. MR1 1st bit set 1. 1 : interrupt request from line x is not allowed */
	*EXTI_IMR |= (1 << 1);

    /* (EXTI_RTSR) Rising trigger selection register.
	 * PORTB B1 1st pin. TR1 1st bit set 1. 1 : Rising trigger enabled (for Event and interrupt) for input line */
	*EXTI_RTSR |= (1 << 1);

	/* (NVIC) Nested vector interrupt controller. (NVIC_ISER0) Interrupt set-enable register 0.
     * EXTI1 position for 7 */
	*NVIC_ISER0 |= (1 << 7);  /* EXTI Line 0 interrupt */
}

/* line number  152 : .word EXTI1_IRQHandler. this function for assembly file there startup_stm32f01ccux.s, .s means assembly file
 * EXTI Line1 interrupt */
void EXTI1_IRQHandler(void)
{
	 /* (EXTI_PR) Pending register */
	if(*EXTI_PR & (1 << 1))  /* checking for trigger. NVIC set or not for pending register */
	{
		for(int j = 0; j < 5; j++)  /* five time running for LED */
		{
			/* (GPIO_ODR) General Purpose output data register */
			*GPIOB_ODR |= (1 << 6);   /* PORTB 6th pin. 6th bit set 1. LED ON */
			delay_ms(200);            /* led on time delay 200ms milliseconds */
			*GPIOB_ODR &= ~(1 << 6);  /* led off */
			delay_ms(200);            /* led on time delay 200ms milliseconds */
		}
		*EXTI_PR |= (1 << 1);   /* this bit is cleared by programming it to '1' */
	}
}

/* select exti15_config PORTB 15th pin select */
void exti15_config(void)
{
	/* (SYSCFG) System configuration controller (SYSCFG_EXTICR1) external interrupt configuration register 1
	 * set and clear for EXTI15_10. 12th bit to 15th bit. 12th bit set 1 means 0001 : PB[x] pin enable */
	*SYSCFG_EXTICR4 = (*SYSCFG_EXTICR4 & (~0x0F << 12)) | (1 << 12);

	/* (EXTI) External interrupt. (EXTI_IMR) Interrupt mask register.
	 * PORTB B15 15th pin. MR15 15th bit set 1. 1 : interrupt request from line x is not allowed */
	*EXTI_IMR |= (1 << 15);

	/* (EXTI_RTSR) Rising trigger selection register.
	 * PORTB B15 15th pin. TR15 15th bit set 1. 1 : Rising trigger enabled (for Event and interrupt) for input line */
	*EXTI_RTSR |= (1 << 15);

	/* (NVIC) Nested vector interrupt controller. (NVIC_ISER0) Interrupt set-enable register 0.
	 * EXTI15_10 position for 40. 40 - 32 = 8. position come 8 */
	*NVIC_ISER1 |= (1 << 8);
}

/* line number  185 : .word EXTI1_IRQHandler. this function for assembly file there startup_stm32f01ccux.s, .s means assembly file
 * EXTI Line1 interrupt */
void EXTI15_10_IRQHandler(void)
{
	 /* (EXTI_PR) Pending register */
	if(*EXTI_PR & (1 << 15))  /* checking for trigger. NVIC set or not for pending register */
	{
		for(int k = 0; k < 5; k++)  /* five time running for LED */
		{
			*GPIOB_ODR |= (1 << 7);  /* PORTB 7th pin. 7th bit set 1. LED ON */
			delay_ms(200);           /* led on time delay 200ms milliseconds */
			*GPIOB_ODR &= ~(1 << 7); /* led off*/
			delay_ms(200);           /* led off time delay 200ms milliseconds */
		}
		*EXTI_PR |= (1 << 15);   /* this bit is cleared by programming it to '1' */
	}
}
