//RCC_CR clock control register
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;

//RCC_CFGR clock configuration register 
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;

//RCC_AHB1ENR peripheral clock enable register, select for GPIOA port A clock enable
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

//RCC_APB2ENR peripheral clock enable register, select for TIMER 11 clock enable 
volatile unsigned int *RCC_APB2ENR = (volatile unsigned int *)0x40023844;

//GPIOA_MODER port mode register 
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;

//GPIOA_ODR port output data register
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014; 

//TIM11_CR1 control register
volatile unsigned int *TIM11_CR1   = (volatile unsigned int *)0x40014800;

//TIM11_CNT counter 
volatile unsigned int *TIM11_CNT   = (volatile unsigned int *)0x40014824;

//TIM11_PSC prescaler
volatile unsigned int *TIM11_PSC   = (volatile unsigned int *)0x40014828;

//TIM11_ARR auto-reload register
volatile unsigned int *TIM11_ARR   = (volatile unsigned int *)0x4001482c; 

//TIM11_SR status register 
volatile unsigned int *TIM11_SR    = (volatile unsigned int *)0x40014810;
