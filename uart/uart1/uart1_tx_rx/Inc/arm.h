/*
 * arm.h
 *
 *  Created on: Feb 19, 2025
 *      Author: murugan
 */

#ifndef ARM_H_
#define ARM_H_


/* structure struct struct_tag (or) struct name */
struct rcc
{
	/* structure syntax data type member (or) field (or) elements */
	/* RCC register 			  offset value */
	unsigned int CR;                // 0x00
	unsigned int PLLCFGR;           // 0x04
	unsigned int CFGR;              // 0x08
	unsigned int CIR;				// 0x0C
	unsigned int AHB1RSTR;			// 0x10
	unsigned int AHB2RSTR;			// 0x14
	unsigned int rsd0;              // 0x18  Reserved address for rsd0
	unsigned int rsd1;				// 0x1C	 Reserved address for rsd1
	unsigned int APB1RSTR;			// 0x20
	unsigned int APB2RSTR;			// 0x24
	unsigned int rsd2;				// 0x28	 Reserved address for rsd2
	unsigned int rsd3;				// 0x2C  Reserved address for rsd3
	unsigned int AHB1ENR; 			// 0x30
	unsigned int AHB2ENR; 			// 0x34
	unsigned int rsd4;				// 0x38	 Reserved address for rsd4
	unsigned int rsd5;				// 0x3C	 Reserved address for rsd5
	unsigned int APB1ENR;			// 0x40
	unsigned int APB2ENR;           // 0x44
	unsigned int rsd6;				// 0x48	 Reserved address for rsd6
	unsigned int rsd7;				// 0x4C	 Reserved address for rsd7
	unsigned int AHB1LPENR;			// 0x50
	unsigned int AHB2LPENR;			// 0x54
	unsigned int rsd8;				// 0x58	 Reserved address for rsd8
	unsigned int rsd9;				// 0x5C	 Reserved address for rsd9
	unsigned int APB1LPENR;         // 0x60
	unsigned int APB2LPENR;			// 0x64
	unsigned int rsd10;				// 0x68	 Reserved address for rsd10
	unsigned int rsd11;				// 0x6C	 Reserved address for rsd11
	unsigned int BDCR;				// 0x70
	unsigned int CSR;				// 0x74
	unsigned int rsd12;				// 0x78	 Reserved address for rsd12
	unsigned int rsd13;				// 0x7C	 Reserved address for rsd13
	unsigned int SSCGR;				// 0x80
	unsigned int PLLI2SCFGR;		// 0x84
	unsigned int rsd14;				// 0x88	 Reserved address for rsd14
	unsigned int DCKCFGR;			// 0x8C
};
volatile struct rcc *RCC = (volatile struct rcc *)0x40023800;  // RCC Register Base address 0x4002 3800
/* here type casting for struct name */

/* structure struct struct_tag (or) struct name */
struct gpio
{
	/* structure syntax data type member (or) field (or) elements */
	/* gpio register 			 offset value  */
	unsigned int MODER; 			// 0x00
	unsigned int OTYPER;			// 0x04
	unsigned int OSPEEDR;			// 0x08
	unsigned int PUPDR;				// 0x0C
	unsigned int IDR;				// 0x10
	unsigned int ODR;				// 0x14
	unsigned int BSRR;				// 0x18
	unsigned int LCKR;				// 0x1C
	unsigned int AFRL;				// 0x20
	unsigned int AFRH;				// 0x24
};
volatile struct gpio *GPIOA = (volatile struct gpio *)0x40020000;  // PORTA  GPIOA Register Base address 0x4002 0000
/* here type casting for struct name */

struct uart
{                               // offset value
	unsigned int SR;			// 0x00
	unsigned int DR;			// 0x04
	unsigned int BRR;			// 0x08
	unsigned int CR1;			// 0x0C
	unsigned int CR2;			// 0x10
	unsigned int CR3;			// 0x14
	unsigned int GTPR;			// 0x18
};
volatile struct uart *USART = (volatile struct uart *)0x40011000; // UART1 Register Base address 0x4001 1000;

#endif /* ARM_H_ */

/* function deceleration */
void rcc_config(void);
void peripheral_bus_config(void);
void gpioa_moder_config(void);
void uart_rx_config(void);
unsigned char uart_rx(void);
void uart_tx(unsigned char data);
