/**************************************************************************************************
 *                                                                                                *
 * Author      : Murugan M                                                                        *
 * Date        : 01-1-2025                                                                        *
 * File        : PB0_interrupt_NVIC_EXTI                                                          *
 * Description : Select Hardware Internal Pull-Down Resistor PUPDR                                *
 *               NVIC Nested vector interrupt controller                                          *
 *               Using for one External interrupt EXTI0                                           *
 *               Input take for GPIOB0 PORT B 0th pin connect to push button                      *
 *               Output take for GPIOB1 PORT B 2nd pin. connect to led.                           *
 *                                                                                                *
 *************************************************************************************************/

/* RCC base address for STM32F401CCU6 data sheet register boundary addresses page number : 51 */

/* rcc register */
volatile unsigned int *RCC_CR = (volatile unsigned int *)0x40023800;  /* Add with RCC register base address and CR register. Address offset value 0x00 */
volatile unsigned int *RCC_CFGR = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;
volatile unsigned int *RCC_APB2ENR = (volatile unsigned int *)0x40023844;

/* GPIOB base address for STM32F401CCU6 data sheet register boundary addresses page number : 51 */

/* gpio register */                                          
volatile unsigned int *GPIOB_MODER = (volatile unsigned int *)0x40020400; 
volatile unsigned int *GPIOB_ODR   = (volatile unsigned int *)0x40020414;
volatile unsigned int *GPIOB_PUPDR = (volatile unsigned int *)0x4002040C;

/* EXTI base address for STM32F401CCU6 data sheet register boundary addresses page number : 52 */

/* EXTI register External interrupt */
volatile unsigned int *EXTI_IMR  = (volatile unsigned int *)0x40013C00;
volatile unsigned int *EXTI_RTSR = (volatile unsigned int *)0x40013C08;
volatile unsigned int *EXTI_PR   = (volatile unsigned int *)0x40013C14;

/* SYSCFG base address for STM32F401CCU6 data sheet register boundary addresses page number : 52 */

/* SYSCFG register System configuration controller. EXTCR1 */
volatile unsigned int *SYSCFG_EXTICR1 = (volatile unsigned int *)0x40013808;

/* Cortex-M4 internal peripherals base address for STM32F401CCU6 data sheet register boundary addresses page number : 51 */

/* NVIC_ISER0 this register Address for Data sheet program manual STM32 Cortex-M4 page number : 208 */

/* NVIC register Nested vector interrupt controller. 6 EXTI Line 0 interrupt. */
volatile unsigned int *NVIC_ISER0 = (volatile unsigned int *)0xE000E100;

/*  function deceleration */
void rcc_config(void);
void peripheral_bus_config(void);
void gpiob_moder_config(void);
void delay_ms(int ms);
void exti_config(void);
void EXTI0_IRQHandler(void);

/* main function */
int main()
{
  /* function call */
  rcc_config();
  peripheral_bus_config();
  gpiob_moder_config();
  exti_config();
  while(1)
  {
    *GPIOB_ODR |= (1 << 2);  /* keep on LED ON. PORTB 2nd pin */
  }
}

/* function definition */
void rcc_config(void)
{
  /* RCC_CR this register for Data sheet Reference manual page number : 103 and 104 */

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
  /* RCC_AHB1ENR this register for Data sheet Reference manual page number : 118 and 119 */

  /* (RCC_AHB1ENR) Advanced High Bus 1 Enable Register */

  /* set 1 1st bit. 1GPIOBEN : IO port B clock enable. 1 : IO port B clock enabled */
  *RCC_AHB1ENR |= (1 << 1);


  /* RCC_APB2ENR this register for Data sheet Reference manual page number : 122 and 123 */

  /* (RCC_APB2ENR) Advanced Peripheral Bus 2 Enable Register */

  /* set 1 14th bit. SYSCFGEN : System configuration controller clock enable */
  *RCC_APB2ENR |= (1 << 14);
}

void gpiob_moder_config(void)
{
  /* GPIOB_MODER this register for Data sheet Reference manual page number : 158 */

  /* GPIO General purpose input and output. (GPIOA_MODER) port mode register */

  /* PORT B select MODER1 2nd pin. set 1 4th bit. 01 : General purpose output mode */
  *GPIOB_MODER |= (1 << 4);


  /* GPIOB_PUPDR this register for Data sheet Reference manual page number : 159 and 160 */

  /* (GPIOx_PUPDR) port pull-up/pull-down register */

  /* PORT B. select PUPDR0 0th pin. set 1 1st bit. 10 : Pull-down */
  *GPIOB_PUPDR |= (1 << 1);
}

void delay_ms(int ms)
{
  /* delay */
  for(int i = 0; i < ms * 1000; i++)
  {
    __asm("NOP"); /* asm means Assembly Language. NOP no operation */
  }
}

/* select exti0_config PORTB 0th pin select */
void exti_config(void)
{
  /* External interrupt/event line mapping and GPIO mapping page number : 208 */
  /* SYSCFG_EXTICR1 this register for Data sheet Reference manual page number : 142 */

  /* (SYSCFG) System configuration controller (SYSCFG_EXTICR1) external interrupt configuration register 1
   * set and clear for EXTI0. 0bit to 3rd bit */
  *SYSCFG_EXTICR1 &= (~0x0F << 0);

  /* select the source input for the EXTI0, 0001 : PB[x] pin. select PORTB 0th pin. set 0th bit 1 */
  *SYSCFG_EXTICR1 |= (1 << 0);

  /* EXTI_IMR this register for Data sheet Reference manual page number : 209 */

  /* (EXTI) External interrupt. (EXTI_IMR) Interrupt mask register.
   * MR0 0th bit set 1. 1 : interrupt request from line x is not allowed */
  *EXTI_IMR |= (1 << 0);

  /* EXTI_RTSR this register for Data sheet Reference manual page number : 210 */

  /* (EXTI_RTSR) Rising trigger selection register.
   * TR0 0th bit set 1. 1 : Rising trigger enabled (for Event and interrupt) for input line */
  *EXTI_RTSR |= (1 << 0);

  /* NVIC External interrupt/event controller (EXTI). Vector table for Data sheet Reference manual page number : 202 */

  /* NVIC_ISER0 this register for Data sheet program manual STM32 Cortex-M4 page number : 210 */

  /* (NVIC) Nested vector interrupt controller. (NVIC_ISER0) Interrupt set-enable register 0.
   *  EXTI0 position for 6 */
  *NVIC_ISER0 |= (1 << 6);  /* EXTI Line 0 interrupt */
}

/* line number  151 : .word EXTI0_IRQHandler. this function for assembly file there startup_stm32f01ccux.s, .s means assembly file
 * EXTI Line0 interrupt */
void EXTI0_IRQHandler(void)  /* IRQ means Interrupt Request Handler */
{
  /* (EXTI_PR) Pending register. this register for Reference manual page number : 211 */
  if(*EXTI_PR & (1 << 0))  /* checking for trigger. NVIC set or not for pending register */
  {
    for(int j = 0; j < 5; j++)  /* five time running for LED */
    {
      /* GPIOB_ODR this register for Reference manual page number : 160 */
      *GPIOB_ODR |= (1 << 2);   /* PORTB 1st pin. 2nd bit set 1. LED ON */
      delay_ms(500);            /* 200ms milliseconds stop */
      *GPIOB_ODR &= ~(1 << 2);  /* LED OFF*/
      delay_ms(500);
    }
    *EXTI_PR |= (1 << 0);  /* this bit is cleared by programming it to '1' */
  }
}
