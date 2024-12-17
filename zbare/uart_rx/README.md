                                   STM32F401CCU6 32-bit microcontroller

				       program for uart receiver rx



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

   * UART send data format for LSB to MSB, LSB ------> Lest Significant Bit MSB ------> Most significant Bit.
   * UART start bit 0 and stop bit 1. 

     start bit 0                   data frame                           parity                       stop bit 1
   --------------------------------------------------------------------------------------------------------------------
   |              |                                            |                            |                         |
   | start bit 0  | data bit 0 to 7 [8 bit] or 0 to 8 [9 bit]  | parity bits 0 (or) 1 bits  | stop bit 1 (or) 2 bits  |	
   |              |					       |                            |                         |
   --------------------------------------------------------------------------------------------------------------------

UART program register procedure

 register

   * RCC_AHB1ENR peripheral clock enable register, port A clock enable.
   * RCC_APB2ENR peripheral clock enable register, uart 1 clock enable.
   * GPIOA_MODER port mode register select for alternate function mode.
   * GPIOA_OSPEEDR port output speed register.
   * GPIOA_AFRH alternate function high register.
   * USART_CR1 control register 1.
   * USART_CR2 control register 2.
   * USART_BRR baud rate register.
   * USART_SR status register.
   * USART_DR data register.

UART program register
 
 RCC config
   
   * RCC_APB2ENR first clear for USART1 clock enable register.
   * RCC_APB2ENR USART1 clock enable register.
   * RCC_AHB1ENR port A clock enable.

 gpio config

   * GPIOA_MODER clear a 18th bit and 19th bit in moder.
   * GPIOA_MODER port mode register, select for alternate function mode, 19th bit set 1.
   * GPIOA_OSPEEDR clear for 18th and 19th bit.
   * GPIOA_OSPEEDR port output speed register, select fast speed, 19th bit set 1. 
   * GPIOA_AFRH clear 4th bit.
   * GPIOA_AFRH alternate function high register, 0111 AF7 select for USART1, port A TX pin A9.

 uart1 rx config ---> receiver 
   
   * USART_CR1 control register 1, USART enable 13th bit set for 1.
   * USART_CR1 control register 1, word length, 8 data bits, 12 bit set 0.
   * USART_CR2 clear 12th bit.
   * USART_CR2 control register 2, select 1 stop bit, 13th bit set 0, 12th bit set 0.
   * USART_BRR baud rate fraction of uart division, value for 4.
   * USART_BRR baud rate mantissa of uart division, value for 103 this value set for 4th bit.
   * USART_CR1 control register 1, receiver enable.
   * USART_SR a clear RXEN receiver for 5th bit.
   * USART_CR1 transmitter enable 3rd bit set 1.


 char send uart1 rx

   * USART_SR set RXNE 5th bit set 1, wait untill 0 come.
   * uart transfor a receiver data store.
   * USART_SR RXNE 5th bit clear.
   * return a receiver data.

 uart1 tx ---> transmitter data send 
   
   * USART_DR transmitter from the data.
   * USART_SR wait for transmitter complete or not, 6th bit set 1.
   * USART_SR clear a transmitter bit, 6th bit clear.
 

 main function
    
   * function call for rcc.
   * function call for gpio.
   * function call for uart1 rx config.
   * receiver data store from a transmitter data.
   * function call uart1 tx.
