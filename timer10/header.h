//RCC_CR clock control register
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;

//RCC_CFGR clock configuration register
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;

//RCC_AHB1ENR peripheral clock enable register
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

//RCC_APB2ENR peripheral clock enable register timer10 clock enable
volatile unsigned int *RCC_APB2ENR = (volatile unsigned int *)0x40023844;

//GPIOA_MODER port mode register
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;

//GPIOA_ODR port output data register 
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014; 

//TIM10_CR1 control register
volatile unsigned int *TIM10_CR1   = (volatile unsigned int *)0x40014400;

//TIM10_CNT counter load a values 
volatile unsigned int *TIM10_CNT   = (volatile unsigned int *)0x40014424;

//TIM10_PSC prescaler load a values
volatile unsigned int *TIM10_PSC   = (volatile unsigned int *)0x40014428;

//TIM10_ARR auto-reload register
volatile unsigned int *TIM10_ARR   = (volatile unsigned int *)0x4001442c;

//TIM10_SR status register
volatile unsigned int *TIM10_SR    = (volatile unsigned int *)0x40014410;
