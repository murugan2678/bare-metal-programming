// peripheral clock enable register, this register use for select for port A clock enable
volatile unsigned int *RCC_AHB1ENR    = (volatile unsigned int *)0x40023830;  // 0x4002 3800 this for RCC base address

// peripheral clock enable register, this register use for select for usart1 clock enable
volatile unsigned int *RCC_APB2ENR    = (volatile unsigned int *)0x40023844; 

// port mode register this register, use for select for alternate function mode
volatile unsigned int *GPIOA_MODER    = (volatile unsigned int *)0x40020000;  // 0x4002 0000 this for GPIOA base address

// port output speed register, this register use for  select for fast mode
volatile unsigned int *GPIOA_OSPEEDR  = (volatile unsigned int *)0x40020008;

// alternate function high register, this register use for select for usart1
volatile unsigned int *GPIOA_AFRH     = (volatile unsigned int *)0x40020024;

// control register 1, this register use for select for 8 data bit
volatile unsigned int *USART_CR1      = (volatile unsigned int *)0x4001100C;  // 0x4001 1000 this for UART1 base address

// control register 2, this register use for select for stop bit
volatile unsigned int *USART_CR2      = (volatile unsigned int *)0x40011010;

// baud rate register
volatile unsigned int *USART_BRR      = (volatile unsigned int *)0x40011008;

// status register
volatile unsigned int *USART_SR       = (volatile unsigned int *)0x40011000;

// data register
volatile unsigned int *USART_DR       = (volatile unsigned int *)0x40011004;
