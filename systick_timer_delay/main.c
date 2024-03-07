
////////////////////////////////////////////////////////////////////
/* Author      : Murugan M                                        */
/* Date        : 06-02-2024                                       */
/* File name   : systick timer                                    */
/* Describtion : The processor has a 24-bit system timer, SysTick */
////////////////////////////////////////////////////////////////////



// clock control register RCC pin config
volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;  // 0x4002 3800 this for RCC base address

// clock configuration register
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;

// peripheral clock enable register
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

// port mode register GPIO pin config
volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;  // 0x4002 0000 this for GPIOA base address

// port output data register
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;

// STK systick timer, CTRL control and status register
volatile unsigned int *STK_CTRL    = (volatile unsigned int *)0xE000E010;  // 0xE000E010 this for STK_CTRL base address

// STK systick timer, LOAD reload value register
volatile unsigned int *STK_LOAD    = (volatile unsigned int *)0xE000E014;  // 0xE000E014 this for STK_LOAD base address

void rcc_config(void);
void gpio_moder(void);
void led_blink(void);
void sys_timer(void);

void rcc_config()
{
  // HSE clock 16th bit clear --> CR
  *RCC_CR = *RCC_CR & ~(0x00010000);

  // HSE clock enable 16th set 1 --> CR
  *RCC_CR = *RCC_CR | (1<<16);

  // HSERDY clock ready flag check for 17th bit set 1 --> CR
  while((*RCC_CR & (1<<17))==0);

  // System clock switch 0th bit and 1th bit clear --> CFGR
  *RCC_CFGR = *RCC_CFGR & ~(0x00000003);

  // System clock switch enable for HSE 0th bit set 1 --> CFGR
  *RCC_CFGR = *RCC_CFGR | (1<<0);

  // System clock switch status check for HSE 2nd bit set 1 --> CFGR
  while(!(*RCC_CFGR & (0x00000004))==4);

  // IO port A clock enable 0th bit set 1 --> AHB1ENR
  *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);
}

void gpio_moder()
{
  // General purpose output mode 0th bit set 1
  *GPIOA_MODER = *GPIOA_MODER | (1<<0);
}

void led_blink()
{
  int i,j;
  
  // output mode 
  *GPIOA_ODR   = *GPIOA_ODR | (1<<0); //led on
  // sys_timer();  //100 mille seconds

  for(i=0; i<10; i++) // totally 1000 mille seconds 
  {
    sys_timer();  // i give for 100 mille seconds
  }

  *GPIOA_ODR   = *GPIOA_ODR & (0<<0);  //led off
  //  sys_timer();  // 100 mille seconds

  for(j=0; j<10; j++)
  {
    sys_timer();
  }
}

// here use for systick timer
void sys_timer()
{
  // *STK_LOAD = 250;
  // *STK_LOAD = 2499; 100 micro seconds, load the value

  // systick reload value register, reload 0th bit to 23th bit
  *STK_LOAD = 2499999; // 100 mille seconds, load the value

  // Counter enable 0th bit set 1, systick control and status register
  *STK_CTRL = *STK_CTRL | (1<<0); //systick control and status register, 0 bit counter enable

  // clock source selection, 2nd bit set 1, processor clock (AHB)
  *STK_CTRL = *STK_CTRL | (1<<2); //systick control and status register, 2 bit clock source selection, set 1

  // return 1 if timer count to 0 last time this was read
  while(!(*STK_CTRL & (1<<16)));  //16 bit count falg
}

int main()
{
  // here a all function call
  rcc_config();
  gpio_moder();
  while(1)
  {
    led_blink();
  }
}
