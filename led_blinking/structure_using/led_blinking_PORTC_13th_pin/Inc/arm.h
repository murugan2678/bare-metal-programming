/*
 * arm.h
 *
 *  Created on: Jan 12, 2025
 *      Author: murugan
 */

#ifndef ARM_H_
#define ARM_H_


/* structure struct struct_tag (or) struct name */
struct rcc
{
  /* data type member (or) field (or) elements */
  /* RCC Register               offset value  */
  unsigned int CR;                // 0x00                                  1
  unsigned int PLLCFGR;           // 0x04                                  2
  unsigned int CFGR;              // 0x08                                  3
  unsigned int CIR;               // 0x0C 				   4
  unsigned int AHB1RSTR;          // 0x10				   5
  unsigned int AHB2RSTR;          // 0x14                                  6
  unsigned int rsd0;              // 0x18   Reserved address for rsd0      7
  unsigned int rsd1;              // 0x1C   Reserved address for rsd1      8
  unsigned int APB1RSTR;          // 0x20                                  9
  unsigned int APB2RSTR;          // 0x24                                  10
  unsigned int rsd2;              // 0x28   Reserved address for rsd2      11
  unsigned int rsd3;              // 0x2C   Reserved address for rsd3      12
  unsigned int AHB1ENR;           // 0x30			           13
  unsigned int AHB2ENR;           // 0x34				   14
  unsigned int rsd4;              // 0x38   Reserved address for rsd4      15
  unsigned int rsd5;              // 0x3C   Reserved address for rsd5      16
  unsigned int APB1ENR;           // 0x40                                  17
  unsigned int APB2ENR;           // 0x44                                  18
  unsigned int rsd6;              // 0x48   Reserved address for rsd6      19
  unsigned int rsd7;              // 0x4C   Reserved address for rsd7      20
  unsigned int AHB1LPENR;         // 0x50                                  21
  unsigned int AHB2LPENR;         // 0x54                                  22
  unsigned int rsd8;              // 0x58   Reserved address for rsd8      23
  unsigned int rsd9;              // 0x5C   Reserved address for rsd9      24
  unsigned int APB1LPENR;         // 0x60                                  25
  unsigned int APB2LPENR;         // 0x64                                  26
  unsigned int rsd10;             // 0x68   Reserved address for rsd10     27
  unsigned int rsd11;             // 0x6C   Reserved address for rsd11     28
  unsigned int BDCR;              // 0x70                                  29
  unsigned int CSR;               // 0x74                                  30
  unsigned int rsd12;             // 0x78   Reserved address for rsd12     31
  unsigned int rsd13;		  // 0x7C   Reserved address for rsd13     32
  unsigned int SSCGR;             // 0x80                                  33
  unsigned int PLLI2SCFGR; 	  // 0x84                                  34
  unsigned int rsd14;             // 0x88   Reserved address for rsd14     35
  unsigned int DCKCFGR;           // 0x8C                                  36
};
volatile struct rcc *RCC = (volatile struct rcc *)0x40023800; // RCC Register Base address 0x4002 3800
/* here type casting for struct name */

/* structure struct struct_tag (or) struct name */
struct gpio
{
  /* data type member (or) field (or) elements */
  /* GPIO Register              offset value */
  unsigned int MODER;             // 0x00               1
  unsigned int OTYPER;            // 0x04               2
  unsigned int OSPEEDR;           // 0x08               3
  unsigned int PUPDR;             // 0x0C               4
  unsigned int IDR;               // 0x10               5
  unsigned int ODR;               // 0x14               6
  unsigned int BSRR;              // 0x18               7
  unsigned int LCKR;              // 0x1C               8
  unsigned int AFRL;              // 0x20               9
  unsigned int AFRH;              // 0x24               10
};
volatile struct gpio *GPIOC = (volatile struct gpio *)0x40020800; // PORTC  GPIOC Register Base address 0x4002 0800
/* here type casting for struct name */

/* function declaration */
void rcc_config(void);
void peripheral_bus_config(void);
void gpioc_moder(void);
void led_blinking(void);
void delay_ms(int ms);

#endif /* ARM_H_ */
