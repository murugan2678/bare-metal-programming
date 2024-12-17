                                              STM32F401CCU6 32-bit Microcontroller

					            program for pll config



Author : Murugan M


what is pll config
 
  * PLL (Phase-Locked Loop) provides a finely configurable 
    clock source for SYSCLK and allows to reach precise and 
    high clock speeds,depending on the input source.

  * frequency up to 84MHz.
  * calculate the frequency of a inside 84MHz only support. more than 84MHz give MHz not working.
 
RCC_CR clock control register

  * RCC_CR PLLCFGR first clear this bit.
  * RCC_CR PLLCFGR select 24th bit set 1 pll on.
  * RCC_CR PLLRDY clock ready flag check.
 
RCC_PLLCFGR PLL configuration register
  
  * step1 = /M division factor 25/5 = 5, 0 to 5 bits.
    --> PLLM set value for 5 --> 000101 ,range bit for 0 to 5th bit.
  
  * step2 = xN multiplication factor 5x8 = 40, 6 to 14 bits.
    --> PLLN set value for 8 --> 000001000, range bit for 6 to 14th bit.
  
  * step3 = /P division factor main system clock 40/2 = 20, finally come 20MHZ, 16 to 17 bits.
    --> PLLP set value for 2 --> 00 , range bit for 16 to 17th bit.
  
  * RCC_AHB1ENR peripheral clock enable register, select PORTA.

GPIOA_MODER port mode register, GPIO ---> general purpose input output 
  
  * MODER select PORTA 

GPIOA_ODR port output data register
 
  * select pin for PORTA 0th pin
