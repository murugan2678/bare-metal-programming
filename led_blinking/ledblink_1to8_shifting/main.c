
/*******************************************************************************
 *                                                                             *
 * Author      : Murugan M                                                     *
 * Date        : 20-12-2024                                                    *
 * File        : ledblink_1to8_shifting                                        *
 * Description : Bitwise AND &, Bitwise OR |, Bitwise left shift << operator   *
 *               value only using all Address                                  *
 *               using for one peripheral bus AHB1ENR                          *
 *                                                                             *
 *******************************************************************************/


/* this program using Bitwise operator left shifting only */	

/* rcc register */

volatile unsigned int *RCC_CR = (volatile unsigned int *)0x40023800;  /* add with RCC register base address and CR register address offset value 0x00 */
volatile unsigned int *RCC_CFGR = (volatile unsigned int *)0x40023808;  /* CFGR address offset value 0x08 */
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830; /* AHB1ENR address offset value 0x30 */

/* gpio register */

volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000; /* MODER address offset value 0x00 */
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014; /* ODR address offset value 0x14 */

/* function declaration */

void rcc_config(void);
void peripheral_bus_config(void);
void gpiob_moder_config(void);
void delay_ms(int ms);
void ledBlinking(void);

/* main function */

int main()
{
  /* function call */

  rcc_config();
  peripheral_bus_config();
  gpiob_moder_config();

  while(1)  /* continuosly running for led blinking */
  {
    ledBlinking();
  }
}


/* rcc_config */

void rcc_config()
{
  /* Reset Clock Control (RCC_CR) clock control register */

  /* set and clear for 16th bit HSEON */
  *RCC_CR = *RCC_CR & (~1 << 16);

  /* HSEON : HSE clock enable. 16th bit select. set 1 : HSE for external oscillator ON */
  *RCC_CR = *RCC_CR | (1 << 16);

  /* HSERDY : HSE clock ready flag. 17th bit select. set 1 : HSE oscillator ready, here checking for ready or not */
  while(!(*RCC_CR & 1 << 17) == 0);				/* when 0 come that time not ready for HSE */

  /* Reset Clock Control (RCC_CFGR) clock configuration register */

  /* SW : System clock switch, set and clear select for 0th bit and 1st bit set value for 11 : not allowed */
  *RCC_CFGR = *RCC_CFGR & (~1 << 0);
  *RCC_CFGR = *RCC_CFGR & (~1 << 1);

  /* SW : System clock switch, 0th bit set 1, 01 : HSE oscillator selected as system clock */
  *RCC_CFGR = *RCC_CFGR | (1 << 0);

  /* SWS : System clock switch status, 2nd bit set 1, 01: HSE oscillator used as the system clock, checking status */
  while(!(*RCC_CFGR & 1 << 2) == 4);
}

/* peripheral_bus_config */

void peripheral_bus_config()
{
  /* Advanced High-performace Bus 1 (RCC_AHB1ENR) peripheral clock enable register */

  /* GPIOAEN : IO port A clock enable. 0th bit set 1 : IO port A clock enable */
  *RCC_AHB1ENR = *RCC_AHB1ENR | (1 << 0);
}

/* gpiob_moder_config */

void gpiob_moder_config()
{
  /* General Purpose Input and Output (GPIOA_MODER) port mode register */

  /* select for MODER0 to MODER7. 0 to 15 select. totaly come 8-bit. 8 pin select */

  for(int j = 0; j < 15; j++)
  {
    *GPIOA_MODER = *GPIOA_MODER | (1 << j); /* set 1 enable. 8 pin select 01 : General Purpose output mode */
    j = j + 1;   /* 0 + 1 adding. 8-bit enable 0, 2, 4, 6, 8, 10, 12, 14  ===> totaly come 8 pin enable */
  }
}

/* delay_ms */

void delay_ms(int ms)
{
  /* delay */

  for(int i = 0; i < ms * 4000; i++)
  {
    __asm("NOP");   /* asm for assembly language. NOP ---> no operation */
  }
}

/* ledBlinking */

void ledBlinking()
{
  /* General Purpose Input and Output (GPIOA_ODR) port mode register */

  /* select for ODR0 to ODR7. totaly come 8-bit. 8 pin select */

  for(int k = 0; k < 8; k++)         /* Down to Up Ascending order one by one 8 led blinking. */
  {
    *GPIOA_ODR = *GPIOA_ODR | (1 << k);  /* set 1 LED ON */
    delay_ms(200);                       /* delay for 200 milliseconds stop led */
    *GPIOA_ODR = *GPIOA_ODR & ~(1 << k); /* clear 1 come with 0 LED OFF */
    delay_ms(200);
  }

  for(int m = 7; m >= 0; m--)       /* Up to Down Descending order one by one 8 led blinking. */
  {
    *GPIOA_ODR = *GPIOA_ODR | (1 << m);  /* set 1 LED ON */
    delay_ms(200);                       /* delay for 200 milliseconds stop led */
    *GPIOA_ODR = *GPIOA_ODR & ~(1 << m); /* clear 1 come with 0 LED OFF */
    delay_ms(200);
  }
}
