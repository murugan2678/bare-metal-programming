
///////////////////////////////////////////////////////////////////////
/* Author      : Murugan M                                           */
/* Date        : 01-03-2024                                          */
/* File name   : uart_rx                                             */
/* Describtion : uart receiver, data receiver after send transmitter */
/*               Asynchronous serial communication                   */ 
///////////////////////////////////////////////////////////////////////

// uart data send for least significant to the most significant
// start bit 0, stop bit 1

#include "header.h"
#include "fun.c"


char uart1_rx(void);
void uart1_tx(unsigned char data);

void rcc_config()
{
  // peripheral clock enable register, select the usart1 clock this 4th bit clear
  *RCC_APB2ENR &= ~(1<<4);
 
  // peripheral clock enable register, select the usart1 clock this 4th bit set 1
  *RCC_APB2ENR |= (1<<4);

  // peripheral clock enable register, select the port A clock enable 0th bit set 1
  *RCC_AHB1ENR |= (1<<0);
}

void gpio_port()
{
  // TX pin A9, clear
  *GPIOA_MODER &= ~(3<<18);

  // TX pin A9, set
  *GPIOA_MODER |= (1<<19);

  // rx pin A10, port mode register, select the alternate function mode, clear for 20th bit and 21th bit
  *GPIOA_MODER &= ~(3<<20);

  // rx pin A10, port mode register, select the alternate function mode, set for 21th bit
  *GPIOA_MODER |= (1<<21);


  // RX pin A10, port output speed register, clear for 20th bit and 21th bit
  *GPIOA_OSPEEDR &= ~(3<<20);

  // RX pin A10 port output speed register, select for fast speed, set for 21th bit 1
  *GPIOA_OSPEEDR |= (1<<21);

  // TX pin A9, clear
  *GPIOA_AFRH &= ~(7<<4);

  // TX pin A9, set for aternative function mode 0111 :AF7
  *GPIOA_AFRH |= (7<<4);

  // RX pin A10 AFRH10 20,21,23,24 this bits clear 
  *GPIOA_AFRH &= ~(7<<8);

  // RX pin A10 alternate function high register, select for 0111 : AF7 this select for usart1
  // select AFRH10 0111 this value give for 20th bit, for portA PIN A10 for RX
  *GPIOA_AFRH |= (7<<8);
}

void uart1_config()
{
  // control register1, set 1 for USART enable
  *USART_CR1 |= (1<<13);

  // control register1, 12th bit clear,set 0 means 8 bit data only send   
  *USART_CR1 &= (~1<<12);

  // control register2, 13th bit set for 1, select for 0 0 : 1 stop bits
  *USART_CR2 &= (~3<<12);

  // baud rate register, fraction of USARTDIV
  *USART_BRR |= 4;

  // baud rate register, mantissa of USARTDIV
  *USART_BRR |= (103<<4);

  // control register1, receiver enable
  *USART_CR1 |= (1<<2);

  //a clear RXEN receiver for 5th bit
  *USART_SR &= (~1<<5);

  // control register1, transmitter enable, set 1
  *USART_CR1 |= (1<<3);

}

char uart1_rx(void)
{
  // wait untill set RXNE bit 1
  while(!(*USART_SR &= (1<<5)));

  // return recived data
  char r_buffer_data = *USART_DR;

  // clear RXNE bit
  *USART_SR &= ~(1<<5);
  
  // return a data
  return r_buffer_data;

}

void uart1_tx(unsigned char data)
{
  // tx send a data
  *USART_DR = data;

  // tc transmite complete
  while(!(*USART_SR & (1<<6)));

  // status register, select for 6th bit tc clear
  *USART_SR &= ~(1<<6);
} 

int main()
{
  rcc_config();
  gpio_port();
  uart1_config();
  while(1)
  {
    // function call rx function pass to data
    unsigned char data = uart1_rx();
        
    // data pass to tx function
    uart1_tx(data);
  }
}
