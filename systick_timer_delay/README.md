                                                        STM32F401CCU6

						   program for systick timer


systick timer (STK)

  * The processor has a 24-bit system timer, SysTick, that counts down from the reload value to
zero, reloads (wraps to) the value in the STK_LOAD register on the next clock edge, then counts down on subsequent clocks.
  * systick timer is used to generate a periodic interrupt to the Cortex-M3 processor.

-------------------------------------------------------------------------------------------------------------------------------------------

register config

  step1 : RCC_CR control register select
  
  step2 : RCC_CFGR clock configuration register select

  step3 : RCC_AHB1ENR peripheral clock enable register 

  step4 : GPIOA_MODER port mode register GPIO pin config

  step5 : GPIOA_ODR port output data register

  step6 : STK_CTRL control register and status register

  step7 : STK_LOAD reload value register, systick timer

-------------------------------------------------------------------------------------------------------------------------------------------

systick timer config

  step1 : STK_LOAD systick reload value register, reload 0th bit to 20th bit
          this register load a value for 2499999 ---> this value for 100ms milli seconds

  step2 : STK_CTRL enable 0th bit set 1, systick control and status register
          systick control and status register, 0th bit counter enable

  step3 : STK_CTRL clock source selection, 2nd bit set 1, select for processor clock (AHB)
          systick control and status register, 2nd bit clock source selection set 1

  step4 : STK_CTRL return 1 if timer count to 0 last time this was read
          16th bit count flag

-------------------------------------------------------------------------------------------------------------------------------------------

gpio_odr port output data register
   
  step1 : select port A 0th bit set 1, led on

  step2 : systick call function 100ms milli seconds on

  step3 : select port A 0th bit set 0, led off
   
  step4 : systick call function 100ms milli seconds off

-------------------------------------------------------------------------------------------------------------------------------------------

systick timer calculation

  
   100 milli seconds                            stm32f401ccu6  this device use for 25 MHZ

   = Required delay x Frequency – 1             25 MHZ for 10^6
   = 100 x 10^-3 x 25 x 10^6 -1                 milli seconds value =  10^-3
   = 100 x 25 x 10^3 -1
   = 100 x 25 x 1000 -1                         PSC prescalar  value load for = 1000
   = 2500 x 1000 -1                             ARR auto reload register value load for = 2500
   = 2500000 – 1
   = 2499999       

   the value for 2499999 -----> 100ms milli seconds

------------------------------------------------------------------------------------------------------------------------------------------

systick timer
   
   stm32 official website  -----> you download sperate datasheet for systick timer

