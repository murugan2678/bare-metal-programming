// led blinking for 0to7 8 led blink program

volatile unsigned int *RCC_CR      = (volatile unsigned int *)0x40023800; //RCC_CR base address
volatile unsigned int *RCC_CFGR    = (volatile unsigned int *)0x40023808; //RCC_CFGR base address and this offset value add 
volatile unsigned int *RCC_AHB1ENR = (volatile unsigned int *)0x40023830; //RCC_AHB1ENR base address and this offset value add

volatile unsigned int *GPIOA_MODER = (volatile unsigned int *)0x40020000; //GPIOA_MODER PORTA base address
volatile unsigned int *GPIOA_ODR   = (volatile unsigned int *)0x40020014; //GPIOA_ODR PORTA base address and this offset value add

void rcc_config(void);
void gpio_moder(void);
void led_blink(void);
void delay(void);

void rcc_config()
{
  *RCC_CR = *RCC_CR & (~0x00010000); //clear the values 
  *RCC_CR = *RCC_CR | 1<<16; //RCC_CR set for 16th bit for HSE external clock enable
  while((*RCC_CR & (1<<17))==0); //RCC_CR set clock read flage 0 mean oscillator not ready, 17th bit set 0

  *RCC_CFGR = *RCC_CFGR & ~(0x00000003); //HSE system clock select 0th bit and 1st bit 
  *RCC_CFGR = *RCC_CFGR | (1<<0); //HSE oscillator select as system clock, 0th bit for 1 set value
  while(!(*RCC_CFGR & (0x00000004))==4); //HSE select clock switch status system clock

  *RCC_AHB1ENR = *RCC_AHB1ENR | (1<<0); //PORTA clock enable
}

void gpio_moder()
{
  for(int j=0; j<=16; j++)
  {
    *GPIOA_MODER = *GPIOA_MODER | (1<<j); //GPIOA_MODER PORTA select the bit for 0 to 16 bit, totally 8 bit
    j++;
  }

}

void led_blink()
{
  int l,p,m,s,i;
  while(1)
  {
    for(i=0; i<=7; i++)
    {
       delay();
      *GPIOA_ODR = *GPIOA_ODR | (1<<i);  //led one, GPIOA_ODR output mode
       delay();
      *GPIOA_ODR = *GPIOA_ODR & (0<<i);  //led off, GPIOA_ODR output mode
    }
    for(m=7; m>=0; m--)
    {
       delay();
      *GPIOA_ODR = *GPIOA_ODR | (1<<m);
       delay();
      *GPIOA_ODR = *GPIOA_ODR & (0<<m);
    } 
    for(l=0; l<=7; l++)
    {
      if(l%2==0)
      {
        delay();
        *GPIOA_ODR = *GPIOA_ODR | (1<<l);
        delay();
        *GPIOA_ODR = *GPIOA_ODR & (0<<l);
      }
    }
    for(m=0; m<=7; m++)
    {
      if(m%2!=0)
      {
        delay();
        *GPIOA_ODR = *GPIOA_ODR | (1<<m);
        delay();
        *GPIOA_ODR = *GPIOA_ODR & (1<<m);
      }
    }
    for(l=0; l<=7; l++)
    {
      *GPIOA_ODR = *GPIOA_ODR | (1<<l);
    }
  }
   
}

void delay()
{
  int k=0;
  for(k=0; k<250000; k++); //delay given
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
