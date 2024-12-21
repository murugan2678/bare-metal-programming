
/*******************************************************************************
 *                                                                             *
 * Author      : Murugan M                                                     *
 * Date        : 19-12-2024                                                    *
 * File        : ledblink_1to8_hexa-decimal                                    *
 * Description : Hexa-decimal value only using all Address                     *
 *               using for one peripheral bus AHB1ENR                          *
 *                                                                             *
 *******************************************************************************/


/* this program using Hexa-decimal value only */	

/* RCC register */
                                             /* this for Hexa-decimal value */
volatile unsigned int *RCC_CR = (volatile unsigned int *)0x40023800;  /* add with RCC register base address and CR register offset value 0x00 */
volatile unsigned int *RCC_CFGR = (volatile unsigned int *)0x40023808;  /* CFGR offset value 0x08 */
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;  /* AHB1ENR offset value 0x08 */

/* GPIO register */

volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000; /* GPIOA register base address and MODER register offset value 0x00 */
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014; /* ODR offset value 0x14 */

/* function declaration */

void rcc_config(void);
void pheripheral_bus(void);
void gpioa_moder_config(void);
void delay_ms(int ms);
void ledBlinking(void);

/* array inside given for binary value. 0b means represented by binary */

int arr[9] = {0x80, 0x40, 0x20, 0x10, 0x01, 0x02, 0x04, 0x08, 0x00};
int arr1[9] = {0x10, 0x20, 0x40, 0x80, 0x08, 0x04, 0x02, 0x01, 0x00};

int main()
{
  /* function call */

  rcc_config();
  pheripheral_bus();
  gpioa_moder_config();

  while(1)
  {
    ledBlinking();
  }
}

void rcc_config()
{
  /* Reset clock control (RCC_CR) clock control register */

  /* set and clear the HSE clock enable 16th bit */
  *RCC_CR = *RCC_CR & (~0x00010000); 

  /* HSE clock enable 16th bit set 1 */
  *RCC_CR =  *RCC_CR | (0x00010000);         // *RCC_CR = 0x00010000;

  /* set by hardware indicating, HSE clock ready flag. 17th bit set 1. 1 means HSE oscillator ready, checking ready or not */
  while(!(*RCC_CR & 0x00020000) == 0); /* 0 means HSE oscillator not ready, when come 1 that time ready */

  /* Reset clock control (RCC_CFGR) clock configuration register */

  /* set and clear the bit. sw : system clock switch for 0th and 1th bit. set 11 : not allowed */
  *RCC_CFGR = *RCC_CFGR & (~0x00000003);

  /* sw : system clock switch, 01 : HSE oscillator selected as system clock, 0th bit set 1 value */
  *RCC_CFGR = *RCC_CFGR | (0x00000001);  // *RCC_CFGR = 0x00000001;

  /* set and clear the bit. sws : system clock switch status. 01 : HSE oscillator used as the system clock. 2nd bit set value 1 */
  while(!(*RCC_CFGR & 0x00000004) == 4);
}

void pheripheral_bus()
{
  /* Advanced High-performance Bus 1 (RCC_AHB1ENR) peripheral clock enable register */

  /* RCC_AHB1ENR ---> GPIOAEN : IO port A clock enable. 0th bit set 1 means enable */
  *RCC_AHB1ENR = *RCC_AHB1ENR | (0x00000001);  // *RCC_AHB1ENR = 0x00000001;
}

void gpioa_moder_config()
{
  /* General Purpose Input and Output (GPIOA_MODER) port mode register */

  /* select MODER0 to MODER7 and the 0 to 7 ---> 8 pin config for moder */
  *GPIOA_MODER = *GPIOA_MODER | (0x00015555);  /* 8 pin select 01 : General Purpose output mode */
                                               /* 8-bit enable 0, 2, 4, 6, 8, 10, 12, 14  ===> totaly come 8 pin enable */
}

void delay_ms(int ms)
{
  /* delay */

  for(int i = 0; i < ms * 400; i++)
  {
    __asm("NOP");   /* asm for assembly language. NOP ---> no operation */
  }
}

void ledBlinking()
{
  /* General Purpose Input and output (GPIOA_ODR) port output data register */

  /* select ODR0 to ODR7 and the 0 to 7 ---> 8 pin config for ODR output data register */
  for(int j = 0; j < 9; j++)
  {
    *GPIOA_ODR = arr[j];  /* array elements here only come value */
    delay_ms(500);        /* delay for 500 milliseconds stop led */
  } 
  delay_ms(100);

  for(int k = 0; k < 9; k++)
  {
    *GPIOA_ODR = arr1[k];  /* array elements here only come value */
    delay_ms(500);         /* delay for 500 milliseconds stop led */
  }
}
