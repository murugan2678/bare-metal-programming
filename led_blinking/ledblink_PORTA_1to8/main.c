
/*******************************************************************************
 *                                                                             *
 * Author      : Murugan M                                                     *
 * Date        : 18-12-2024                                                    *
 * File        : PORTA_1to8th_pin                                              *
 * Description : Led blink for PORTA 1 to 8th pin.                             *
 *               using for one pheripheral bus AHB1ENR                         *
 *                                                                             *
 *******************************************************************************/


/* rcc register */
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;   /* this for RCC base address, CR offset value 0x00, two register base address and offset value add */
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;   /* CFGR offset value 0x08 */
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;   /* AHB1ENR offset value 0x30 */


/* gpio register */
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;   /* this for GPIOA PORTA base address, MODER offset value 0x00 */
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;   /* ODR offset value 0x00 */

/* function declaration */
void rcc_config(void);
void pheripheral_bus_config(void);
void gpioa_mode_config(void);
void delay_ms(int ms);
void ledBlinking(void);

/* array */

int arr1[9] = {0x80, 0x40, 0x20, 0x10, 0x01, 0x02, 0x04, 0x08, 0x00};   /* this for Hexa-decimal value 0x80 */
int arr2[9] = {0x08, 0x04, 0x02, 0x01, 0x10, 0x20, 0x40, 0x80, 0x00};
int arr3[9] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00};
int arr4[5] = {0b00000001, 0b00000100, 0b00010000, 0b01000000, 0b00000000};  /* this for Binary value 0xb00000001 */
int arr5[6] = {0b00000010, 0b00001000, 0b00100000, 0b10000000, 0b00000000};
int arr6[4] = {0b01010101, 0b00000000, 0b10101010, 0b00000000};
int arr7[4] = {0b11111111, 0b00000000, 0b11111111, 0b00000000};

/* main function */

int main(void)
{
        /* function call */
        
	rcc_config();
	pheripheral_bus_config();
	gpioa_mode_config();

	while(1)  /* continuously running led */
	{
		ledBlinking();  /* led blinking */
	}
}

/* function defination */

void rcc_config()
{
	/* Reset clock control (RCC_CR) clock control register */

	/* set and clear for 16th bit HSE */
	*RCC_CR = *RCC_CR & (~0x0001000);

	/* 1 : HSE oscillator ON, 16th bit set 1. external clock enable */
	*RCC_CR = *RCC_CR | (1 << 16);

	/* set and clear for 17th bit HSERDY. HSE clock ready flag, checking ready or not */
	while(!(*RCC_CR & 1 << 17) == 0);

	/* Reset clock control (RCC_CFGR) clock configuration register */

	/* set and clear for 0th bit and 1st bit system clock switch */
	*RCC_CFGR = *RCC_CFGR & (~0x00000003);

	/* SW system clock switch. 01 : HSE oscillator selected as system clock, set 0th bit 1 */
	*RCC_CFGR = *RCC_CFGR | (1 << 0);

	/* SWS system clock switch status, 01 : HSE oscillator selected as system clock, checking status */
	while(!(*RCC_CFGR & 0x00000004) == 4);

}

void pheripheral_bus_config()
{
	/* Advanced High-performance Bus 1 (RCC_AHB1ENR) peripheral clock enable register */

	/* GPIOAEN : IO port A enable. set 1 for 0th bit enable */
	*RCC_AHB1ENR = *RCC_AHB1ENR | (1 << 0);

}

void gpioa_mode_config()
{
	/* General purpose input and output (GPIOA_MODER) port mode register */

	/* select for (MODER0 to MODER7) 0 to 7th bit enable mode for PORTA mode, select 0 to 15 bit*/
	for(int i = 0; i <= 16; i++)
	{
		*GPIOA_MODER = *GPIOA_MODER | (1 << i);
		i = i + 1;
	}
}

void delay_ms(int ms)
{
        /* delay */
	for (int s = 0; s < ms * 4000; s++)
	{
		__asm("NOP");
	}

}

void ledBlinking()
{
	/* (GPIOA_ODR) port output data register */

	/* select the ODR0 to ODR7 to 7th bit*/

	for(int j = 0; j < 8; j++)  /* 0 to 7 Down to Up Ascending order led blinking one by one */
	{
		*GPIOA_ODR = *GPIOA_ODR | (1 << j);  /* LED ON , set value 1 */  
		delay_ms(200);
		*GPIOA_ODR = *GPIOA_ODR & ~(1 << j);  /* LED OFF, clear value */
		delay_ms(200);
	}

	for(int k = 0; k < 9; k++)  /* 7 to 4 Up to Mid and 0 to 3 Down to Mid led blinking one bye one */
	{
		*GPIOA_ODR = arr1[k];  /* array elements here only one bye one come */
		delay_ms(200);
	}

    for(int m = 0; m < 5; m++)  /* odd number 1,3,5,7 led blinking */
    {
    	*GPIOA_ODR = arr4[m];   
    	delay_ms(200);
    }

    for (int n = 0; n < 4; n++)  /* all led on and off */
    {
    	*GPIOA_ODR = arr7[n];
    	delay_ms(200);
    }
    for(int p = 0; p < 9; p++)  /* 3 to 0 Mid to Down and 4 to 7 Mid to Up led blinking one bye one */
    {
    	*GPIOA_ODR = arr2[p];
    	delay_ms(200);
    }

    for(int q = 0; q < 5; q++)  /* even number 0,2,4,8 led blinking */
    {
    	*GPIOA_ODR = arr5[q];
    	delay_ms(200);
    }

    for(int r = 0; r < 9; r++)  /* 7 to 0 Up to Down Descending order led blinking one by one  */
    {
    	*GPIOA_ODR = arr3[r];
    	delay_ms(200);
    }

    for(int t = 0; t < 4; t++)  /* odd number 1,3,5,7 led blinking and even number 0,2,4,8 led blinking, ON and OFF */
    {
    	*GPIOA_ODR = arr6[t];
    	delay_ms(200);
    }

}
