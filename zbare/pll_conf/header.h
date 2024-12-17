// RCC_CR PLLON pll select 24 bit set 1 for on
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;

// RCC_PLLCFGR pll configuration
volatile unsigned int *RCC_PLLCFGR = (volatile unsigned int *)0x40023804;

// RCC_AHB1ENR peripheral clock enable register
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

// GPIO_MODER port moder register, select a port A
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;

// GPIO_ODR port output data register, select a port A is output
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;
