
//////////////////////////////////////////////////////
/* Author      : Murugan M                          */
/* Date        : 01-03-2024                         */
/* File name   : uart_tx                            */
/* Describtion : uart transmitter send data         */
/*               Asynchronous serial communication  */
//////////////////////////////////////////////////////

// uart data send for least significant to the most significant
// start bit 0, stop bit 1

#include "header.h"
#include "fun.c"


void rcc_config()
{
  // first clear data
  *RCC_APB2ENR &= ~(1<<4);

  // set : peripheral clock enable register USART1 clock enable
  *RCC_APB2ENR |= (1<<4);

  // peripheral clock enable register port A clock enable
  *RCC_AHB1ENR |= (1<<0); 
}

void gpio_config()
{
  // clear 18th and 19th bit in MODER
  *GPIOA_MODER &= ~(3<<18);

  // set : port mode register,select for alternate function mode
  *GPIOA_MODER |= (1<<19);

  // clear 18th and 19th bit in OSPEEDR
  *GPIOA_OSPEEDR &= ~(2<<18);

  // set : port output speed register, fast speed
  *GPIOA_OSPEEDR |= (1<<19);

  //clear 4th bit in AFRH
  *GPIOA_AFRH &= ~(1<<4);

  // set : alternate function high register, 0111 AF7 select for UART1, port A TX pin A9 
  *GPIOA_AFRH |= (7<<4);
}

void uart1_config()
{
  // control register 1, word length, 8 data bits, 12th bit set for 0 
  *USART_CR1 &= ~(1<<12);

  // control register 1, USART enable, 13th bit set for 1
  *USART_CR1 |= (1<<13);

  // clear 12th bit 
  *USART_CR2 &= ~(1<<12);

  // set : control register 2, 2 stop bits, 13th bit set 1
  *USART_CR2 |= (1<<13);

  // baud rate fraction of uart division
  *USART_BRR |= 4; 
   
  // baud rate mantissa of uart division
  *USART_BRR |= (103<<4);

  // control register 1, TE transmitter enable
  *USART_CR1 |= (1<<3);

  // set : status register, transmit data register empty, wait for until one, set for 1
  while(!(*USART_SR & (1<<7)));
  
  // clear 7th bit
  *USART_SR &= ~(1<<7);
}

void uart1_tx()
{
  // uart send the data
  *USART_DR = 'A';

  // clear, TC transmission complete, 6th bit clear
  while(!(*USART_SR &= (1<<6)));

  // set, TC transmission complete, 6th bit set for 1
  *USART_SR |= (1<<6);
}  
 
int main()
{
  rcc_config();
  gpio_config();
  uart1_config();
  uart1_tx();
}
