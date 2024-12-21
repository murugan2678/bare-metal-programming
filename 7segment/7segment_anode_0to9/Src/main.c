
/*******************************************************************************
 *                                                                             *
 * Author      : Murugan M                                                     *
 * Date        : 20-12-2024                                                    *
 * File        : 7segment_anode_0 to 9                                         *
 * Description : Common Anode 7segment                                         *
 *               7segement blink for PORTA 0 to 7th pin.                       *
 *               using for one peripheral bus AHB1ENR                          *
 *                                                                             *
 *******************************************************************************/

/* rcc register  */
volatile unsigned int *RCC_CR = (volatile unsigned int *)0x40023800;   /* add with RCC register base address and CR register. Address offset value 0x00 */
volatile unsigned int *RCC_CFGR = (volatile unsigned int *)0x40023808;  /* CFGR register. Addres soffset value 0x08 */
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;  /* AHB1ENR register. Address offset value 0x08 */

/* gpio register */
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000; /* GPIOA register base address and MODER register. Address offset value 0x00 */
volatile unsigned int *GPIOA_ODR = (volatile unsigned int *)0x40020014;   /* ODR register. Address offset value 0x14 */

/* function deceleration */
void rcc_config(void);
void peripheral_bus_config(void);
void gpioa_moder_config(void);
void delay_ms(int ms);
void seven_segment_led(void);

/* this for 7segment common anode 7 segment Display value. */

int arr[10] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90};

int main(void)
{
  /* function call */

  rcc_config();
  peripheral_bus_config();
  gpioa_moder_config();

  while(1)   /* 1 means true. led continuously blinking */
  {
    seven_segment_led();  /* 7segment function call */
  }
}

/* rcc_config */

void rcc_config()
{
  /* Reset clock control (RCC_CR) clock control register */

  /* set and clear the HSE clock enable 16th bit */
  *RCC_CR = *RCC_CR & (~0x00010000);

  /* HSE clock enable 16th bit set 1 */
  *RCC_CR = *RCC_CR | (1 << 16);

  /* set by hardware indicating, HSE clock ready flag. 17th bit set 1. 1 means HSE oscillator ready, checking ready or not */
  while(!(*RCC_CR & 1 << 17) == 0);

  /* Reset clock control (RCC_CFGR) clock configuration register */

  /* set and clear the bit. sw : system clock switch for 0th and 1st bit. set 11 : not allowed */
  *RCC_CFGR = *RCC_CFGR & (~0x00000003);

  /* sw : system clock switch, 01 : HSE oscillator selected as system clock, 0th bit set 1 value */
  *RCC_CFGR = *RCC_CFGR | (1 << 0);

  /* set and clear the bit. sws : system clock switch status. 01 : HSE oscillator used as the system clock. 2nd bit set value 1 */
  while(!(*RCC_CFGR & 0x00000004) == 4);
}

/* peripheral_bus_config */
void peripheral_bus_config()
{

  /* Advanced High-performance Bus 1 (RCC_AHB1ENR) peripheral clock enable  register */

  /* PORT A. GPIOAEN : IO port A clock enable. select 0th bit set 1 */
  *RCC_AHB1ENR = *RCC_AHB1ENR | (1 << 0);  /* PORT A enable */
}

/* gpioa_moder_config */
void gpioa_moder_config()
{
  /* General Purpose Input and Output (GPIOA_MODER) port mode register */

  /* select PORTA. 01: General purpose output mode */
  /* MODER0 to MODER7 totally 8 MODER select. All MODER give for output mode*/
  for(int i = 0; i < 15; i++)
  {
    *GPIOA_MODER = *GPIOA_MODER | (1 << i); /* set 1 enable */
    i = i + 1;   /* 0 + 1 adding. 8-bit enable 0, 2, 4, 6, 8, 10, 12, 14  ===> totaly come 8 pin enable */
  }
}

/* delay */
void delay_ms(int ms)
{
  for(int j = 0; j < ms * 4000; j++)  /* 1000 means 1 milliseconds */
  {
    __asm("NOP");   /* asm means assembly language. NOP means no operation */
  }
}

/* 7segment led */
void seven_segment_led()
{
  /* General Purpose Input and Output (GPIOA_ODR) port output data register */

  /* select ODR0 to ODR7 and 0 to 7 ---> 8 pin config for ODR output data register */
  for(int k = 0; k < 10; k++) /* 7segment 0 to 7 bit. totally 8-bit data */
  {
    *GPIOA_ODR = arr[k];  /* 7segment display value load. Display ON */
    delay_ms(500);         /* delay 200 milliseconds stop */
  }
}
