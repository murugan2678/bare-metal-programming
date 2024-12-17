// RCC_AHB1ENR peripheral clock enable register, port A clock enable 
volatile unsigned int *RCC_AHB1ENR   = (volatile unsigned int *)0x40023830; //0x4002 3800 RCC base address

// RCC_APB2ENR peripheral clock enable register, usart 1 clock enable  
volatile unsigned int *RCC_APB2ENR   = (volatile unsigned int *)0x40023844;

// GPIOA_MODER port mode register select for alternate function mode   
volatile unsigned int *GPIOA_MODER   = (volatile unsigned int *)0x40020000; //0x4002 0000 GPIOA base address

// GPIOA_OSPEEDR port output speed register 
volatile unsigned int *GPIOA_OSPEEDR = (volatile unsigned int *)0x40020008;

// GPIOA_AFRH alternate function high register
volatile unsigned int *GPIOA_AFRH    = (volatile unsigned int *)0x40020024;

// USART1_CR1 control register 1
volatile unsigned int *USART_CR1     = (volatile unsigned int *)0x4001100C; //0x4001 1000 USART1 base address

// USART1_CR2 control register 2
volatile unsigned int *USART_CR2     = (volatile unsigned int *)0x40011010;

// USART1_BRR baud rate register
volatile unsigned int *USART_BRR     = (volatile unsigned int *)0x40011008;

// USART1_SR status register
volatile unsigned int *USART_SR      = (volatile unsigned int *)0x40011000;

// USART1_DR data register
volatile unsigned int *USART_DR      = (volatile unsigned int *)0x40011004;
