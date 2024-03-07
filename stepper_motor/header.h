// clock control register RCC pin config 
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;     // 0x4002 3800 this for RCC base address

// clock configuration register
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;

// peripheral clock enable register
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

// port mode register GPIO pin config
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;    // 0x4002 0000 this for GPIOA base address

// port output data register 
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;

// STK systick timer, CTRL control and status register
volatile unsigned int *STK_CTRL    = (volatile unsigned int *)0xE000E010;    // 0xE000E010 this for STK_CTRL base address

// STK systick timer, LOAD reload value register 
volatile unsigned int *STK_LOAD    = (volatile unsigned int *)0xE000E014;    // 0xE000E014 this for STK_LOAD base address
