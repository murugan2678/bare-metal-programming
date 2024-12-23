/*
 * arm.h
 *
 *  Created on: Dec 23, 2024
 *      Author: murugan
 */

#ifndef ARM_H_
#define ARM_H_

/* rcc register */

volatile unsigned int *RCC_CR = (volatile unsigned int *)0x40023800; /* add with RCC base address and CR register Address offset value 0x00 */
volatile unsigned int *RCC_CFGR = (volatile unsigned int *)0x40023808;   /* CFGR register Address offset value 0x08 */
volatile unsigned int *RCC_AHB1ENR =  (volatile unsigned int *)0x40023830; /* AHB1ENR register Address offset value 0x30 */

/* gpio register */

volatile unsigned int *GPIOC_MODER = (volatile unsigned int *)0x40020800; /* add with GPIOC base address and MODER register Address offset value 0x00 */
volatile unsigned int *GPIOC_ODR   = (volatile unsigned int *)0x40020814; /* ODR register Address offset value 0x14 */

/* systick timer register */

volatile unsigned int *STK_CTRL = (volatile unsigned int *)0xE000E010;  /* add with STK base address and CTRL register Address offset value 0x00 */
volatile unsigned int *STK_LOAD = (volatile unsigned int *)0xE000E014;  /* LOAD register Address offset value 0x04 */

/* function declaration */

void rcc_config(void);
void peripheral_bus_config(void);
void gpioc_moder_config(void);
void systick_timer_delay(int ms);
void led_blinking(void);

#endif /* ARM_H_ */
