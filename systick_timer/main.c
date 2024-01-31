/* systick timer program */

volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800;
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808;
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830;

volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000;
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014;

volatile unsigned int *STK_CTRL    = (volatile unsigned int *)0xE000E010;
volatile unsigned int *STK_LOAD    = (volatile unsigned int *)0xE000E014;

void rcc_config(void);
void gpio_moder(void);
void led_blink(void);
void sys_timer(void);

void rcc_config()
{
  *RCC_CR = *RCC_CR & ~(0x00010000);
  *RCC_CR = *RCC_CR | (1<<16);
  while((*RCC_CR & (1<<17))==0);

  *RCC_CFGR = *RCC_CFGR & ~(0x00000003);
  *RCC_CFGR = *RCC_CFGR | (1<<0);
  while(!(*RCC_CFGR & (0x00000004))==4);

  *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0);
}

void gpio_moder()
{
  *GPIOA_MODER = *GPIOA_MODER | (1<<0);
}

void led_blink()
{
  int i,j;
  *GPIOA_ODR   = *GPIOA_ODR | (1<<0); //led on
  for(i=0; i<10; i++) //1000 mille seconds 
  {
    sys_timer();  //100 mille seconds
  }
  *GPIOA_ODR   = *GPIOA_ODR & (0<<0);  //led off
  for(j=0; j<10; j++)
  {
    sys_timer();
  }
}

void sys_timer()
{
  //*STK_LOAD = 250;
  //*STK_LOAD = 2499;
  *STK_LOAD = 2499999; // 100 mille seconds, load the value
  *STK_CTRL = *STK_CTRL | (1<<0); //systick control and status register, 0 bit counter enable
  *STK_CTRL = *STK_CTRL | (1<<2); //systick control and status register, 2 bit clock source selection, set 1
  while(!(*STK_CTRL & (1<<16)));  //16 bit count falg
}

int main()
{
  rcc_config();
  gpio_moder();
  while(1)
  {
    led_blink();
  }
}
