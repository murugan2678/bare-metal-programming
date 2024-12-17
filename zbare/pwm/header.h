                                   // RCC register config

// clock control register, select HSIRDY internal high-speed clock ready flag
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;  // RCC base address 0x40023800

// peripheral clock enable register, select GPIOBEN : IO port B clock enable
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830; 

// peripheral clock enable register, select TIM10EN and TIM11EN clock enable
volatile unsigned int *RCC_APB2ENR = (volatile unsigned int *)0x40023844;

                                  // GPIO port mode register config

// port mode register, select moder for alternate function mode
volatile unsigned int *GPIOB_MODER   = (volatile unsigned int *)0x40020400; // GPIOB base address 0x40020400

// port output speed register, select the high speed
volatile unsigned int *GPIOB_OSPEEDR = (volatile unsigned int *)0x40020408;

// port pull-up/pull-down register, select pull-down
volatile unsigned int *GPIOB_PUPDR   = (volatile unsigned int *)0x4002040C; 

// alternate function high register, select AF3
volatile unsigned int *GPIOB_AFRH    = (volatile unsigned int *)0x40020424;


				 // TIMER 10 register config

// control register
volatile unsigned int *TIM10_CR1   = (volatile unsigned int *)0x40014400; // TIM10 base address 0x40014400

// status register
volatile unsigned int *TIM10_SR    = (volatile unsigned int *)0x40014410;

// event generation register
volatile unsigned int *TIM10_EGR   = (volatile unsigned int *)0x40014414;

// capture/compare mode register 1
volatile unsigned int *TIM10_CCMR1 = (volatile unsigned int *)0x40014418;

// capture/compare enable register
volatile unsigned int *TIM10_CCER  = (volatile unsigned int *)0x40014420;

// counter 
volatile unsigned int *TIM10_CNT   = (volatile unsigned int *)0x40014424; 

// presclar
volatile unsigned int *TIM10_PSC   = (volatile unsigned int *)0x40014428;

// auto-reload register
volatile unsigned int *TIM10_ARR   = (volatile unsigned int *)0x4001442C;

// capture/compare register 1
volatile unsigned int *TIM10_CCR1  = (volatile unsigned int *)0x40014434;

                                // TIMER 11 register config

// control register 
volatile unsigned int *TIM11_CR1   = (volatile unsigned int *)0x40014800;

// status register
volatile unsigned int *TIM11_SR    = (volatile unsigned int *)0x40014810;

// counter 
volatile unsigned int *TIM11_CNT   = (volatile unsigned int *)0x40014824;

// prescaler 
volatile unsigned int *TIM11_PSC   = (volatile unsigned int *)0x40014828;

// auto-reload register
volatile unsigned int *TIM11_ARR   = (volatile unsigned int *)0x4001482C;

// event generation register
volatile unsigned int *TIM11_EGR   = (volatile unsigned int *)0x40014814;
