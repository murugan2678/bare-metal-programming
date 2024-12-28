
/***********************************************************************************************
 *                                                                                             *
 * Author      : Murugan M                                                                     *
 * Date        : 26-12-2024                                                                    *
 * File        : pull_up_push_button                                                           *
 * Description : Pull-Up Using Hardware External Resistor using for 4.7k                       *
 *               Input take for GPIOB0 PORT B 0th pin.                                         *
 *               Output take for GPIOA0 PORT A 0th pin.                                        *
 *               Output 1k resistor using connect to LED (+) VCC Positive and (-) GND Ground   *
 *               Using for one peripheral bus AHB1ENR                                          *
 *                                                                                             *
 ***********************************************************************************************/


/* rcc register */
volatile unsigned int *RCC_CR = (volatile unsigned int *)0x40023800;  /* Add with RCC register base address and CR register. Address offset value 0x00 */
volatile unsigned int *RCC_CFGR = (volatile unsigned int *)0x40023808;  /* CFGR register. Address offset value 0x08 */
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830; /* AHB1ENR register. Address offset value 0x30 */

/* gpio register */
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;  /* GPIOA register base address and MODER register. Address offset value 0x00 */
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;  /* ODR register. Address offset value 0x14 */
volatile unsigned int *GPIOB_MODER = (volatile unsigned int *)0x40020400;  /* GPIOB register base address and MODER register. Address offset value 0x00 */
volatile unsigned int *GPIOB_IDR   = (volatile unsigned int *)0x40020410;  /* IDR register. Address offset value 0x10 */

/* function deceleration */
void rcc_config(void);
void peripheral_bus_config(void);
void gpioa_gpiob_moder_config(void);
void delay_ms(int ms);
void led_blinking(void);


/* main function */

int main(void)
{
  /* function call */

  rcc_config();
  peripheral_bus_config();
  gpioa_gpiob_moder_config();
  while(1)    /* condition true 1 continuously running */
  {
    led_blinking();
  }
}

/* function definition */

void rcc_config(void)
{
  /* Reset clock control (RCC_CR) clock control register */

  /* set and clear 16th bit */
  *RCC_CR &= (~1<<16);

  /* select 16th bit HSEON : HSE clock enable. 1 : HSE oscillator ON */
  *RCC_CR |= (1 << 16);

  /* select 17th bit HSERDY : HSE clock ready flag. 1 : HSE oscillator ready. HSE external oscillator ready or not checking */
  while(!(*RCC_CR & (1 << 17)));

  /* Reset clock control (RCC_CFGR) clock configuration register */

  /* set and clear 0th bit and 1st bit. 11 : not allowed */
  *RCC_CFGR &= (~3 << 0);

  /* select 0th bit SW  : System clock switch. 01 : HSE oscillator selected as system clock */
  *RCC_CFGR |= (1 << 0);

  /* select 2nd bit SWS : System clock switch status. 01 : HSE oscillator used as the system clock */
  while(!(*RCC_CFGR & 1 << 2));
}

void peripheral_bus_config(void)
{
  /* Advanced peripheral Bus 1 (RCC_AHB1ENR) peripheral clock enable  register */

  /* select 0th bit GPIOAEN : Input/Output  port A enable */
  *RCC_AHB1ENR |= (1 << 0);

  /* select 1th bit GPIOBEN : Input/Output PORT B enable */
  *RCC_AHB1ENR |= (1 << 1);
}

void gpioa_gpiob_moder_config(void)
{
  /* General Purpose Input and Output (GPIOA_MODER) port mode register */
  /* select MODER0. PORTA 0th pin select. 0th pin set a 01 : General purpose output mode */
  *GPIOA_MODER |= (1 << 0);

  /* select MODER0. PORTB 0th pin select. 0th pin set a 00 : Input */
  *GPIOB_MODER &= (~3 << 0);
}

void delay_ms(int ms)
{
  /* delay */
  for(int i = 0; i < ms * 1000; i++)
  {
    __asm("NOP");  /* asm means Assembly Language. NOP means no operation */
  }
}

void led_blinking(void)
{
  /* General Purpose Input and Output (GPIOA_IDR) input data register */
  /* select IDR. PORTB 0th pin select */

  /* step 1 : this condition Pull-Up means HIGH 1. push button not press means condition True means 1 */
  /* step 2 : Push Button press means condition False means 0 */
  if((*GPIOB_IDR & 1 << 0) == 1)
  {
    /* General Purpose Input and Output (GPIOA_ODR) output data register */
    /* select ODR0. PORTA 0th pin select. */
    *GPIOA_ODR |= (1 << 0); /* led on */
    delay_ms(100);          /* 100ms milliseconds stop */
  }
  else
  {
    *GPIOA_ODR &= (0 << 0); /* led off */
  }
}
