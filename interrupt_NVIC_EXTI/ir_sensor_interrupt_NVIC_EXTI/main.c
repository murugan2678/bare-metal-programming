/**************************************************************************************************
 *                                                                                                *
 * Author      : Murugan M                                                                        *
 * Date        : 31-12-2024                                                                       *
 * File        : ir_sensor_interrupt_NVIC_EXTI                                                                        *
 * Description : IR-Sensor select Hardware Internal Pull-up Resistor PUPDR                        *
 *               Input take for GPIOB0 PORT B 0th pin connect with IR-Sensor out pin              *
 *               Output take for GPIOA0 PORT A 0th pin.                                           *
 *               Output 1k resistor using connect to LED (+) VCC Positive and (-) GND Ground      *
 *               Using for one peripheral bus AHB1ENR.                                            *
 *               power supply input voltage 3v to 5v.                                             *
 *************************************************************************************************/

/* rcc_register */
volatile unsigned int *RCC_CR = (volatile unsigned int *)0x40023800;   /* Add with RCC register base address and CR register. Address offset value 0x00 */
volatile unsigned int *RCC_CFGR = (volatile unsigned int *)0x40023808;  /* CFGR register. Address offset value 0x08 */
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;   /* AHB1ENR register. Address offset value 0x30 */
volatile unsigned int *RCC_APB2ENR = (volatile unsigned int *)0x40023844;   /* APB2ENR register. Address offset value 0x44 */

/* gpio moder register */
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;  /* GPIOA register base address and MODER register. Address offset value 0x00 */
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;  /* ODR register. Address offset value 0x14 */
volatile unsigned int *GPIOA_PUPDR = (volatile unsigned int *)0x4002000C;  /* GPIOB register base address and MODER register. Address offset value 0x00 */

/* External interrupt */
volatile unsigned int *EXTI_IMR = (volatile unsigned int *)0x40013C00;  /* IMR register. Address offset value 0X00 */
volatile unsigned int *EXTI_RTSR = (volatile unsigned int *)0x40013C08; /* RTSR register. Address offset value 0X08 */
volatile unsigned int *EXTI_PR = (volatile unsigned int *)0x40013C14;   /* PR register. Address offset value 0X0C */

/* System configuration controller */
volatile unsigned int *SYSCFG_EXTICR1 = (volatile unsigned int *)0x40013808;  /* EXTICR1 register. Address offset value 0X08 */

/* NVIC vectored interrupt controller */
volatile unsigned int *NVIC_ISER0 = (volatile unsigned int *)0xE000E100;  /* ISER0 register. Address offset value 0X00 */

/* function deceleration */
void rcc_config(void);
void peripheral_bus_config(void);
void gpioa_moder_config(void);
void delay_ms(int ms);
void led_blinking(void);
void exti_config(void);
void EXTI0_IRQHandler(void);

/* main function */
int main(void)
{
  /* function call */
  rcc_config();
  peripheral_bus_config();
  gpioa_moder_config();
  exti_config();
  while(1)
  {
    /* 0 to 8 led ON */
    *GPIOA_ODR |= (0xFF);
    delay_ms(300);
  }
}

/* function definition */
void rcc_config(void)
{
  /* Reset clock control (RCC_CR) clock control register */

  /* set and clear 16th bit */
  *RCC_CR = *RCC_CR & (~0x00010000);

  /* select 16th bit HSEON : HSE clock enable. 1 : HSE oscillator ON */
  *RCC_CR = *RCC_CR | (1 << 16);

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

  /* Advanced peripheral Bus 2 (RCC_APB2ENR) peripheral clock enable  register */
  /* Enable SYSCFG clock */
  *RCC_APB2ENR |= (1 << 14);  /* System configuration controller clock enable */
}

void gpioa_moder_config(void)
{
  /* General Purpose Input and Output (GPIOA_MODER) port mode register */
  /* PORT A select MODER0 to MODER8 0 to 8th pin select 01 : General purpose output mode */
  *GPIOA_MODER |= (0x00015554);  /* output mode */

  /* select PUPDR0. PORTB 0th pin select. 1st pin set a 10 : Pull-up */
  *GPIOA_PUPDR = (*GPIOA_PUPDR & ~(0x3 << 0)) | (1 << 0);
}

void delay_ms(int ms)
{
  /* delay */
  for (int i = 0; i < ms * 1000; i++)
  {
    __asm("NOP");  /* asm means Assembly Language. NOP means no operation */
  }
}

void exti_config(void)
{
  /* (SYSCFG) System configuration controller. (EXTICR1) external interrupt configuration register 1 */
  /* select source input for the EXTI0 external interrupt. 0000 : PA[x]pin. i select PORT A 0th pin*/
  //*SYSCFG_EXTICR1 = (*SYSCFG_EXTICR1 & ~(0x0F << 0)) | (0 << 0);
  *SYSCFG_EXTICR1 &= (~0x0F << 0);
  *SYSCFG_EXTICR1 |= (0 << 0);

  /* (EXTI) External interrupt. (IMR) Interrupt mask register */
  *EXTI_IMR |= (1 << 0);

  /* (EXTI_RTSR) Rising trigger selection register */
  /* 1: Rising trigger enabled (for Event and Interrupt) for input line. select TR0 0th bit */
  *EXTI_RTSR |= (1 << 0);

  /* (NVIC_ISER0) NVIC Nested vector interrupt controller. ISER0 Interrupt set-enable register */
  /* I select for EXTI0 --> EXTI Line0 interrupt. position for 6th */
  *NVIC_ISER0 |= (1 << 6);
}

/* EXTI Line0 interrupt */
void EXTI0_IRQHandler(void)
{
  /* (EXTI_PR) Pending register */
  /* checking for trigger. NVIC set or not */
  if(*EXTI_PR & (1 << 0))
  {
    /* step 1 : this condition Pull-up means High 1.  condition True means 1 */
    /* Five times run led blinking */
    for (int j = 0; j < 5; j++)
    {
      /* General Purpose Input and Output (GPIOA_ODR) output data register */
      /* select ODR1 to ODR8. output */
      for(int i = 1; i <= 8; i++)
      {
	*GPIOA_ODR |= (1 << i);   /* Led On */
	delay_ms(200);            /* 200ms milliseconds stop */
	*GPIOA_ODR &= ~(1 << i);  /* Led Off */
      }
    }
    /* clear pending register. This bit is cleared by programming it to ‘1’.*/
    *EXTI_PR |= (1 << 0);
  }
}
