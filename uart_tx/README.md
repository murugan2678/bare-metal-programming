                                              STM32F401CCU6  32-bit Microcontroller 
                                
					         program for uart transmitter tx



Author : Murugan M



what is uart 
    
   * UART - Universal Asynchronous Receiver-Transmitter.
   * UART offers a flexible means of full-duplex data exchange with external equipment requiring an industry standard
     NRZ asynchronous serial data format.
   * The USART offers a very wide range of baud rates using a fractional baud rate generator.   
 
UART main features

   * UART full duplex, asynchronous communications.
   * UART send data with least significant to the most significant.
   * UART start bit 0 and stop bit 1. 
   * Programmable data word length (8 or 9 bits), you select 8 bit data frame.
   * Configurable stop bits - supported for 1 or 2 stop bits.
   * Singal-wire half-duplex communication.
   * This interface uses a fractional baud rate generator - with a 12-bit mantissa and 4-bit fraction.
   
UART data frame

   * UART send data format for LSB to MSB, LSB ---> List Significant Bit  MSB ---> Most Significant Bit.
   * UART start bit 0 and stop 1.

       start bit 0                   data frame                                parity                       stop bit 1           
     -----------------------------------------------------------------------------------------------------------------------
     |               |                                             |                             |                         |
     | start bit 0   | data bit 0 to 7 [8 bit] (or) 0 to 8 [9 bit] | parity bits 0 (or) 1 bits   |  stop bit 1 (or) 2 bits |
     |               |                                             |                             |                         |
     -----------------------------------------------------------------------------------------------------------------------

UART program register procedure

   * RCC_AHB1ENR peripheral clock enbale register, port A clock enable.
   * RCC_APB2ENR peripheral clock enbale register, usart 1 clock enble.
   * GPIOA_MODER port mode register select for alternate function mode.
   * GPIOA_OSPEEDR port ouput speed register.
   * GPIOA_AFRH alternate function high register.
   * USART_CR1 control register 1.
   * USART_CR2 control register 2.
   * USART_BRR baud rate register.
   * USART_SR status register.
   * USART_DR data register.

UART program register 
   
 rcc config

   * RCC_APB2ENR first clear for USART1 clock enable register.
   * RCC_APB2EBR peripheral clock enble register, USART1 clock enable register.
   * RCC_AHB1ENR peripheral clock enable register, port A clock enable.

 gpio config
   
   * GPIOA_MODER clear a 18th bit and 19th bit in moder.
   * GPIOA_MODER port mode register, select for alternate function mode, 19th bit set 1.
   * GPIOA_OSPEEDR clear for 18th and 19th bit.
   * GPIOA_OSPEEDR port output speed register, selct fast speed, 19th bit set 1.
   * GPIOA_AFRH clear 4th bit.
   * GPIOA_AFRH alternate function high register, 0111 AF7 select for USART1, port A TX pin A9.

 uart config
   
   * USART1_CR1 cotrol register 1, word length, 8 data bits, 12 bit set 0.
   * USART1_CR1 control register 1, USART enable 13th bit set for 1.
   * USART1_CR2 clear 12th bit.
   * USART1_CR2 cotrol register 2, 2 stop bits, 13 bit set 1.
   * USART1_BRR baud rate fraction of uart division, value for 4.
   * USART1_BRR baud rate mantissa of uart division, value for 103 this value set for 4th bit.
   * USART1_CR1 control register, TE transmitter enable.
   * USART1_SR status register, transmit data register empty, wait for until one, set for 1.
   * USART1_SR clear a 7th bit.
 
 uart tx --> transmitter

   * USART_DR send a data.
   * USART_SR clear TC transmission complete 6th bit clear.
   * USART_SR TC transmission complete set 1 for 6th bit.


uart transmitter data 'A' and oscilloscope saw the data, below signal only show in oscilloscope  

                                    character 'A'
   |                                 
   |
   |
   |
   |start            data frame                             stop        
   |bit 0                  (0 to 7) 8 bit data               bit 1    
   |     -------                            -------     ------------
   |     |     |                            |     |     |
   |     |     |                            |     |     |
   |  0  |  1  |  0     0     0     0    0  |  1  |  0  |  1
   |     |     |                            |     |     |
___|_____|     |_____|_____|_____|_____|____|     |_____|_____________________
   |
   |
   |
   |
                data calculation  
				   
		   character for 'A'  A ----> hexa decimal value for = 41

                        8 bit data

'A' hexa value = 41 	4	  1  
  
 		     8 4 2 1   8 4 2 1

	    MSB <--- 0 1 0 0   0 0 0 1 ---> LSB

       uart data start for LSB 
             
        start                                   stop 
	bit = 0        data bit (0 to 7)        bit = 1  
           -----------------------------------------
	   |   |   |   |   |   |   |   |   |   |   |
	   | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 0 | 1 |
	   |   |   |   |   |   |   |   |   |   |   |
	   -----------------------------------------					       
