/*****************************************************************************************
 *                                                                                       *
 *  Author      : Murugan M                                                              *
 *  Date        : 24-02-2025                                                             *
 *  File        : UART1_tx_rx                                                            *
 *  Description : Communication protocols. Inter system Protocol for UART                *
 *                USART ---> Universal Synchronous Asynchronous Receiver and Transmitter *
 *                USART1 TX pin for PORTA 9th pin                                        *
 *                USART1 RX pin for PORTA 10th pin                                       *
 *                using for two pheripheral bus AHB1ENR and APB2ENR                      *
 *                single data transmitter. baud rate using 9600bps.                      *
 *                value 103 or 104. fraction value 4                                     *
 *                using for HSI : internal clock for 16 MHz      		         *
 ****************************************************************************************/

#include "arm.h" /* this header file for all register inside there. using for structure is user-defined data type */

/* main function */
int main(void)
{
  /* function call */
  rcc_config();
  peripheral_bus_config();
  gpioa_moder_config();
  uart_rx_config();

  while(1)
  {
    /* data receiver transfer to transmitter */
    unsigned char data = uart_rx();
    uart_tx(data);
  }
}

/* function definition */
void rcc_config(void)
{
  /* select HSI internal clock for 16 MHz ---> Mega hertz. Mega means 10^6 */

  /* Reset clock control register (RCC_CR) */

  /* set and clear for HSION bit */
  RCC->CR &= ~(1 << 0);

  /* HSION : Internal high-speed clock enable. 0th bit set 1. 1 : HSI oscillator ON */
  RCC->CR |= (1 << 0);

  /* HSIRDY : Internal high-speed clock ready flag. 1st bit set 1. 1 : HSI oscillator ready */
  while(!(RCC->CR & (1 << 1)));

  /* SW : System clock switch. 00 : HSI oscillator selected as system clock. 0th bit and 1st bit. set 00 */
  RCC->CFGR &= ~(3 << 0);

  /* SWS : System clock switch status. 00 : HSI oscillator used as the system clock. 2nd bit and 3rd bit. set 00  */
  while((RCC->CFGR & (3 << 2)) != (0 << 2));
}

void peripheral_bus_config(void)
{
  /* Advanced High Bus 1. peripheral clock enable register (RCC_AHB1ENR) */

  /* GPIOAEN : port A clock enable */
  RCC->AHB1ENR |= (1 << 0);

  /* Advanced peripheral Bus 2. peripheral clock enable register (RCC_APB2ENR) */

  /* APB2ENR UART1EN : UART1 clock enable */
  RCC->APB2ENR |= (1 << 4);
}

void gpioa_moder_config(void)
{
  /* General purpose input and output (GPIO) */
  /* GPIO port mode register (GPIOA_MODER) */

  /* PORT A MODER9. I select for port A 9th pin for USART 1 */
  GPIOA->MODER |= (1 << 19);  // TX pin PORT A 9th pin.  10 : Alternate function mode
  GPIOA->MODER |= (1 << 21);  // RX pin PORT A 10th pin. 10 : Alternate function mode

  /* output speed register (GPIOB_OSPEEDR) */
  GPIOA->OSPEEDR |= (1 << 19);  // 10 : High speed
  GPIOA->OSPEEDR |= (1 << 21);

  /* alternate function high register (GPIOB_AFRH) */
  GPIOA->AFRH |= (7 << 4);  // 0111 : AF7  UART1 pin. PORTA 9th pin for USART1
  GPIOA->AFRH |= (7 << 8);  // 0111 : AF7  UART1 pin. PORTA 10th pin for USART1
}

void uart_rx_config(void)
{
  /* USART Universal Synchronous Asynchronous Receiver and Transmitter */

  /* control register 1 (USART_CR1) */

  /* UE : USART enable, 13th bit set 1. 1 : USART enabled  */
  USART->CR1 |= (1 << 13);  // UE : USART enable

  /* M : word length. 12th bit set 0. 0 : 1 start bit 8 data bits, n stop bit */
  USART->CR1 &= ~(1 << 12);  // M : word length. 0 : 8 bit data

  /* STOP : STOP bits. 12th bit set 00. 00 : 1 Stop bit */
  USART->CR2 &= ~(3 << 12);  // stop : stop bits

  /* Baud rate register (USART_BRR) */
  /* fraction of USARTDIV for 0 to 3rd bit, [3:0] 4bit define the fraction. set fraction value 4 */
  USART->BRR |= 4;

  /* mantissa of USARTDIV for 4 to 15th bit. [15:4] 12bit define the mantissa. set mantissa value 104. I select 9600bps bps --> bit per seconds */
  USART->BRR |= (104 << 4);  // mantissa value

  /* RE : Receiver enable. 2nd bit set 1. 1 : Receiver enable */
  USART->CR1 |= (1 << 2);  // RE : Receiver enable

  USART->SR &= (~1 << 5);  // RXNE : Read data register not empty

  /* TE : Transmitter enable. 3rd bit set 1. 1 : transmitter enable */
  USART->CR1 |= (1 << 3);  // TE : Transmitter enable
}

unsigned char uart_rx(void)
{
  /* status register (USART_SR) */

  /* RXNE : Read data register not empty
   * 1 : Received data is ready to be read */
  while(!(USART->SR & (1 << 5)));

  /* character variable deceleration. passing data */
  unsigned char val_data = USART->DR;

  /* 0 : Data is not received */
  USART->SR &= ~(1 << 5);

  /* return data to tx transmitter */
  return val_data;
}

void uart_tx(unsigned char data)
{
  /* data register (USART_DR) */

  /* data transfrom  a tx */
  USART->DR = data;

  /* TC : Transmission complete. 1 : Transmission is complete */
  while(!(USART->SR & (1 << 6)));

  /* 	0 : Transmission is not complete */
  USART->SR &= ~(1 << 6);
}
