volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

volatile unsigned int *GPIOA_MODER  = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_PUPDR  = (volatile unsigned int *)0x4002000c;
volatile unsigned int *GPIOA_ODR    = (volatile unsigned int *)0x40020014;
volatile unsigned int *GPIOA_IDR    = (volatile unsigned int *)0x40020010;

volatile unsigned int *GPIOB_MODER = (volatile unsigned int *)0x40020400;
volatile unsigned int *GPIOB_ODR   = (volatile unsigned int *)0x40020414;
