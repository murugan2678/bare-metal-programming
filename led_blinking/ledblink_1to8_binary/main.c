
/*******************************************************************************
 *                                                                             *
 * Author      : Murugan M                                                     *
 * Date        : 19-12-2024                                                    *
 * File        : ledblink_1to8_binary                                          *
 * Description : Binary value only using all Address                           *
 *               using for one peripheral bus AHB1ENR                          *
 *                                                                             *
 *******************************************************************************/


/* this program using Binary value only */

/* rcc register */
                                                         /*   0x40023800; */
                                                         /*      4   0    0   2   3 8    0    0   */
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0b01000000000000100011100000000000;  /* add with RCC register base address   and CR register. Address offset value  0x00 */
                                                         /*   0x40023808; */
							 /*      4   0    0   2   3 8    0  8     */                             
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0b01000000000000100011100000001000;  /* CFGR register. Address offset value 0x08 */

							 /*   0x40023830; */
                                                         /*      4   0    0   2   3 8     3   0   */
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0b01000000000000100011100000110000;  /* AHB1ENR register. Address offset value 0x08 */


/* gpio register */
                                                         /*   0x40020000; */
							 /*      4    0    0  2   0   0   0   0   */
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0b01000000000000100000000000000000;  /* GPIOA register base address and 
												      MODER register. Address offset value 0x00 */

                                                         /*   0x40020014; */
                                                         /*      4    0    0  2   0    0   1 4   */
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0b01000000000000100000000000010100; /* ODR register. Address offset value 0x14 */


/* function declaration */

void rcc_config(void);
void peripheral_bus_config(void);
void gpioa_moder_config(void);
void ledBlinking(void);

/* array inside given for binary value. 0b means represented by binary */

int arr[4] = {0b01010101, 0b00000000, 0b10101010, 0b00000000};

/* main function */

int main()
{
  /* function call */
  
  rcc_config();
  peripheral_bus_config();
  gpioa_moder_config();

  while(1)   /* continuosly running */
  {
    ledBlinking();  /* ledBlinking function call */
  }
}

void rcc_config()
{
  /* Reset clock control (RCC_CR) clock control register */

  /* set and clear the HSE clock enable 16th bit */

  /* totaly 32-bit binary value   31 30 29 28   27 26 25 24   23 22 21 20   19 18 17 16   15 14 13 12   11 10 9 8   7 6 5 4   3 2 1 0 */
  *RCC_CR = *RCC_CR & (~0b00000000000000010000000000000000);

  /* HSE clock enable 16th bit set 1 */
  *RCC_CR = *RCC_CR | (0b00000000000000010000000000000000);

  /* set by hardware indicating, HSE clock ready flag. 17th bit set 1. 1 means HSE oscillator ready, checking ready or not */
  while(!(*RCC_CR & 0b00000000000000100000000000000000) == 0);  /* 0 means HSE oscillator not ready, when come 1 that time ready */

  /* Reset clock control (RCC_CFGR) clock configuration register */

  /* set and clear the bit. sw : system clock switch for 0th and 1st bit. set 11 : not allowed */
  *RCC_CFGR = *RCC_CFGR & (~0b00000000000000000000000000000011);

  /* sw : system clock switch, 01 : HSE oscillator selected as system clock, 0th bit set 1 value */
  *RCC_CFGR = *RCC_CFGR | (0b00000000000000000000000000000001);

  /* set and clear the bit. sws : system clock switch status. 01 : HSE oscillator used as the system clock. 2nd bit set value 1 */
  while(!(*RCC_CFGR & 0b00000000000000000000000000000100) == 4);
}

void peripheral_bus_config()
{
  /* Advanced High-performance Bus 1 (RCC_AHB1ENR) peripheral clock enable  register */

  /* RCC_AHB1ENR ---> GPIOAEN : IO port A clock enable. 0th bit set 1 means enable */
  *RCC_AHB1ENR = *RCC_AHB1ENR | (0b00000000000000000000000000000001);
}

void gpioa_moder_config()
{
  /* General Purpose Input and Output (GPIOA_MODER) port mode register */

  /* select MODER0 to MODER7 and the 0 to 7 ---> 8 pin config for moder */
  /* 8 pin select 01 : General Purpose output mode */
  /* 8-bit enable 0, 2, 4, 6, 8, 10, 12, 14  ===> totaly come 8 pin enable */
  *GPIOA_MODER = *GPIOA_MODER | (0b00000000000000010101010101010101);
  
}

void delay_ms(int ms)
{
  /* delay */

  for(int i = 0; i < ms * 4000; i++)
  {
    __asm("NOP");   /* asm for assembly language. NOP ---> no operstion*/
  }
}

void ledBlinking()
{
  /* General Purpose Input and Output (GPIOA_ODR) port output data register */

  /* select ODR0 to ODR7 and 0 to 7 ---> 8 pin config for ODR output data register */

  for(int j = 0; j < 4; j++)
  {
    *GPIOA_ODR = arr[j];  /* array elements here only come value */
    delay_ms(200);        /* delay for 200 milliseconds stop led */
  }
}
