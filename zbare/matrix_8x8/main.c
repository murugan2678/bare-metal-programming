volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800; //RCC_CR base address
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808; //RCC_CFGR, rcc base address and add the offset value 0x08
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830; //RCC_AHB1ENR, rcc base address and add the offset value 0x30

volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000; //GPIOA_MODER base address select GPIOA PORTA
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014; //GPIOA_ODR GPIOA_MODER base address and add offset value 0x14

volatile unsigned int *GPIOB_MODER = (volatile unsigned int *)0x40020400;
volatile unsigned int *GPIOB_ODR   = (volatile unsigned int *)0x40020414;

void rcc_config(void);
void gpio_moder(void);
void led_blink(void);
void delay(void);

void rcc_config(void)
{
  *RCC_CR = *RCC_CR & (~0x00010000); //first clear HSE external clock
  *RCC_CR = *RCC_CR | (1<<16); //External clock enable, 16th bit set for 1
  while((*RCC_CR & (1<<17))==0); //HSE 0 for oscillator not ready, clock ready flag, 17th bit set 0

  *RCC_CFGR = *RCC_CFGR & ~(0x00000003); //HSE select the system clock switch
  *RCC_CFGR = *RCC_CFGR | (1<<0); //HSE External system clock, 0th bit set for 1
  while(!(*RCC_CFGR & (0x00000004))==4); //HSE select the system clock switch status
  
  *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0); //select port A clock enable, 0th bit set for 1
  *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<1);
}

void gpio_moder()
{
  int k,m,s,b;
  for(k=0; k<16; k++)
  {
    *GPIOA_MODER = *GPIOA_MODER | (1<<k);
    k++;
  }
  for(s=0; s<=4; s++)
  {
    *GPIOB_MODER = *GPIOB_MODER | (1<<s);
    s++;
  }
  for(m=10; m<=15; m++)
  {
    *GPIOB_MODER = *GPIOB_MODER | (1<<m);
    m++;
  }
  for(b=16; b<20; b++)
  {
    *GPIOB_MODER = *GPIOB_MODER | (1<<b);
    b++;
  }

}  

void led_blink()
{
  int i,j;
  while(1)
  {
   for(i=0;i<10;i++)
   {
     delay();
     *GPIOA_ODR = *GPIOA_ODR | (1<<i);
      for(j=0;j<8;j++)
      {
        *GPIOB_ODR = *GPIOB_ODR | (1<<i);
      }
   }
   for(i=0; i<8; i++)
   {
        *GPIOA_ODR = *GPIOA_ODR & (0<<i);
   }
   /*for(i=0;i<10;i++)
   {
     delay();
     *GPIOA_ODR = *GPIOA_ODR | (1<<i);
      for(j=0;j<8;j++)
      {
        *GPIOB_ODR = *GPIOB_ODR | (1<<i);
      }
   }
   delay();
   for(i=0; i<10; i++)
   {
     *GPIOA_ODR = *GPIOA_ODR & (0<<i);
     *GPIOB_ODR = *GPIOB_ODR & (0<<i);
  } */
   /*for(i=0; i<10; i++)
   {
     *GPIOA_ODR = *GPIOA_ODR | (1<<i);
     for(j=0; j<10; j++)
     {
       *GPIOB_ODR = *GPIOB_ODR | (1<<j);
       delay();
       *GPIOB_ODR = *GPIOB_ODR & (0<<j);
     }
   } */
  }
}

void delay()
{
  int k;
  for(k=0; k<250000; k++);
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
