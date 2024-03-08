
                                                       STM32F401CCU6
                                                  program for timer11 steps


First write all peripheral base address and address offset value

  Base address  End address  Peripheral   Select register     Addressoffset            Address offset

  0x4002 3800 - 0x4002 3BFF    RCC          RCC_CR               0x00          RCC_CR  = 0x40023800, base address add
                                                                               with RCC_CR address offset value 0x00

                                            RCC_CFGR             0x08          RCC_CFGR = 0x40023808, base add with RCC_CFGR 
					                                       address offset value 0x08
                             
        			            RCC_AHB1ENR          0x30          RCC_AHB1ENR = 0x40023800, base add with 
			     		                                       address offset value 0x30

                                            RCC_APB2ENR          0x44          RCC_APB2ENR = 0x40023844, base addition with 
                                                                               address offset value 0x44

  0x4002 0000 - 0x4002 03FF    GPIOA        GPIOA_MODER          0x00          GPIOA_MODER =  0x40020000, base addition 
                                                                               with address offset value 0x00

                                            GPIOA_ODR            0x14          GPIOA_ODR   =  0x40020014, base addition 
					                                       with address offset value 0x14

  0x4001 4800 - 0x4001 4BFF    TIM11        TIM11_CR1            0x00          TIM11_CR1  =  0x40014800, base address 
                                                                               add with address offset value 0x00

                                            TIM11_CNT            0x24          TIM11_CR1  = 0x40014824, base address 
					                                       add with address offset value 0x24

					    TIM11_PSC            0x28          TIM11_PSC  = 0x40014828, base address 
					                                       add with address offset value 0x28
                                          
					    TIM11_ARR            0x2C          TIM11_ARR  = 0x4001482C, base address
					                                       add with address offset value 0x2C

					    TIM11_SR             0x10          TIM11_SR   = 0x40014810, base address 
					                                       add with address offset value 0x10
-------------------------------------------------------------------------------------------------------------------------------------------

Second write program below steps follow

    Step1 : RCC_CR        ---> clock control register RCC_CR, HSEON : select 16th bit HSE clock enable, external clock
                               HSERDY : 17th bit HSE clock ready flag check, set for 1.

    Step2 : RCC_CFGR      ---> clock configuration register, SW : 0th bit system clock switch, select HSE, value for 01,
                               set for 0th bit 1, SWS : sysatem clock switch status ,HSE 2nd bit for 1, check for status.

    Step3 : RCC_AHB1ENR   ---> peripheral clock enable register, i select for 0th bit for PORTA clock enable.

    Step4 : GPIOA_MODER   ---> port mode register,select 01 general purpose output mode.

    Step5 : GPIOA_ODR     ---> port output data register, select for 0th bit, set in 1.

    Step6 : RCC_APB2ENR   ---> peripheral clock enable register, 17th bit TIM10 clock enable 1.

    Step7 : TIM11_CR1     ---> control register, 0th bit counter disable for 0.

    Step8 : TIM11_CR1     ---> control register, 1th bit update disable for 1.

    Step9 : TIM11_CR1     ---> control register, 2nd bit update request source disable for 0.

    Step10 : TIM11_ARPE   ---> control register, auto-reload preload enable for 1.

    Step11 : TIM11_CNT    ---> counter value give 0.

    Step12 : TIM11_PSC    ---> prescaler value, i give for timer10 delay for 100 milli seconds calculate for totally value 2500000,
                               separate prescaler value give = 1000.

    Step13 : TIM11_ARR    ---> auto reload register value give = 2500, 100 milli seconds.

    Step14 : TIM11_CR1    ---> control register, 0th bit counter enable 1.

    Step15 : TIM11_SR     ---> status register, 0th bit update interrupt flag.
                               update interrupt flag data fully store means come 1, data not fully store untill 0 only.

    Step16 : TIM11_SR     ---> status register 0th bit clear

-------------------------------------------------------------------------------------------------------------------------------------------

Timer10 calculation

   100 milli seconds                            stm32f401ccu6  25 MHZ using

   = Required delay x Frequency – 1             25 MHZ for 10^6
   = 100 x 10^-3 x 25 x 10^6 -1                 milli seconds value =  10^-3
   = 100 x 25 x 10^3 -1
   = 100 x 25 x 1000 -1                         PSC prescalar  value load for = 1000
   = 2500 x 1000 -1                             ARR auto reload register value load for = 2500
   = 2500000 – 1
   = 2499999

-------------------------------------------------------------------------------------------------------------------------------------------

How calculate all peripheral base address and address offset value
  
   Base address  End address               peripheral
   0x4002 3800 - 0x4002 3BFF                  RCC                 RCC = Reset clock control
                          
                           RCC_CR  =  clock control register
   
   RCC base address value          = 0x4002 3800  this value for 32 bit value
   RCC_CR adrress offset value     = 0x00         this value for 8 bit value

    Example
      
      RCC and RCC_CR two adrress add
       
        0x4002 3800
	         00   (+)   RCC_CR address offset value = 0x00
        -----------
        0x4002 3800         now RCC_CR value for  =  0x4002 3800  
        -----------
	                  
	                 RCC_CFGR = clock configuration register
			 
   RCC base address               = 0x4002 3800
   RCC_CFGR address offset value  = 0x08

    Example
           
      RCC and RCC_CFGR two address add
                     
        0x4002 3800
                 08  (+)   RCC_CFGR address offset value = 0x08
        -----------
        0x4002 3808        now RCC_CFGR value for  =  0x4002 3808
        -----------
	                
   Base address  End address           peripheral
   0x4002 0000 - 0x4002 03FF           GPIOA now RCC_CR value for  =  0x4002 3800  
                                            
                                 GPIO = General purpose input output
                                                    
                         GPIOA_MODER  =  port mode register

   GPIOA base address value           =  0x4002 0000  this value for 32 bit value
   GPIOA_MODER address offset value   =  0x00         this value for 8 bit value
    
    Example
          
      GPIOA and GPIOA_MODER two address addition
                       
        0x4002 0000
                 00   (+)  GPIOA_MODER address offset value = 0x00
        -----------
        0x4002 0000        now RCC_MODER value for  =  0x4002 0000
        -----------

                         GPIOA_ODR  = port output data register
   
   GPIOA base address               =  0x4002 0000
   GPIOA_ODR address offset value   =  0x14
       
    Example

      GPIOA and GPIOA_ODR two address addition
                         
        0x4002 0000
                 14   (+)  GPIOA_ODR address offset value = 0x14
        -----------
        0x4002 0014        now GPIOA_ODR value for  =  0x4002 0014
        -----------
     
   Base address  End address               peripheral
   0x4001 4800 - 0x4001 4BFF                TIM11

                        TIM11_CR1  =   control register

   TIM11 base address value        =  0x4001 4800  this value for 32 bit value
   TIM11_CR1 address offset value  =  0x00         this value for 8 bit value

    Example

      TIM11 and TIM11_CR1 two address addition
                          
        0x4001 4800
                 00   (+)   TIM11_CR1 address offset value = 0x00
        -----------
        0x4001 4800         now TIM11_CR1 value for  =   0x4001 4800 
        -----------

                        TIM11_CNT  =  counter
   
   TIM11 base address              =  0x4001 4800
   TIM11_CNT address offset value  =  0x14
        
    Example
            
      TIM11 and TIM11_CNT two address addition
                        
        0x4001 4800
                 24   (+)   TIM11_CNT address offset value = 0x24
        -----------
        0x4001 4824         now TIM11_CNT value for  =   0x4001 4824
        -----------
