/**********************************************************************************
 *                                                                                *
 *  Author      : Murugan M                                                       *
 *  Date        : 07-02-2025                                                      *
 *  File        : pwm_register_timer10                                            *
 *  Description : Led blink for PORTB 8th pin.                                    *
 *                using for two pheripheral bus AHB1ENR and APB2ENR in TIMER 10   *
 *		  i give 16ms milliseconds on 4ms milliseconds off                *
 *                using for HSI : internal clock for 16 MHz                       *
 *                using for PWM register in timer10                               *
 *********************************************************************************/

#include "arm.h"  /* this header file for all register inside there. using for structure is user-defined data type */

/* main function */
int main(void)
{
  /* function call */
  rcc_config();
  peripheral_bus_config();
  gpiob_moder_config();
  timer10_pwm_config();
  while(1)
  {
    led_blinking();
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

  /* Reset clock control configuration register (RCC_CFGR) */

  /* SW : System clock switch. 00 : HSI oscillator selected as system clock. 0th bit and 1st bit. set 00 */
  RCC->CFGR &= ~(3 << 0);

  /* SWS : System clock switch status. 00 : HSI oscillator used as the system clock. 2nd bit and 3rd bit. set 00  */
  while((RCC->CFGR & (3 << 2)) != (0 << 2));
}

void peripheral_bus_config(void)
{
  /* Advanced High Bus 1. peripheral clock enable register (RCC_AHB1ENR) */

  /* port b clock enable */
  RCC->AHB1ENR |= (1 << 1);

  /* Advanced peripheral Bus 2. peripheral clock enable register (RCC_APB2ENR) */
  /* port b TIMER10 clock enable */
  RCC->APB2ENR |= (1 << 17);
}

void gpiob_moder_config(void)
{
  /* General purpose input and output (GPIOB_MODER) */

  /* PORT B MODER8. I select for port B 8th pin for TIMER 10. */
  GPIOB->MODER |= (1 << 17);   // 17th bit set 1. 10 : Alternate function mode

  /* output speed register (GPIOB_OSPEEDR) */
  GPIOB->OSPEEDR |= (1 << 17);  // 10 : High speed

  /* pull-up/pull-down register (GPIOB_PUPDR) */
  GPIOB->PUPDR |= (1 << 17);   // 17th bit set 1. 10 : pull-down.

  /* alternate function high register (GPIOB_AFRH) */
  GPIOB->AFRH |= (0x3 << 0);   // 0011 : AF3, timer10 pin. PORTB 8th pin
}

void timer10_pwm_config(void)
{
  /* CCER : capture/compare enable register (TIM10_CCER) */

  // CC1E : capture/compare 1 output enable, 0 : off - OC1 is not active
  TIM10->CCER &= ~(1<<0);

  /* capture/compare mode register 1 (TIM10_CCMR1) */

  /* OC1PE : output compare 1 preload enable. 0 : preload register on TIM10_CCR1 disabled.
   *  write OC1PE = 0 to disable preload register */
  TIM10->CCMR1 &= ~(1 << 3);

  /* CC1S : capture/compare 1 selection
   * 00 : CC1 channel is configured as output */
  TIM10->CCMR1 &= ~(3 << 0);

  /* OC1M : output compare 1 mode. output pin select.
   * 110 : PWM mode 1 - channel 1 is active as long as TIM10_CNT < TIM10_CCR1 else inactive */
  TIM10->CCMR1 |= (6 << 4);

  /* OC1FE : output compare 1 fast enable.
   * 1 : An active edge on the trigger input acts like a compare match on CC1 output. */
  TIM10->CCMR1 |= (1 << 2);

  /* capture/compare register 1 (TIM10_CCR1)
   * if channel CC1 is configured as output */
  TIM10->CCR1 = 1;

  /* control register 1 (TIM10_CR1)
   * ARPE : Auto-reload preload enable. 1 : TIM10_ARR register is buffered */
  TIM10->CR1 |= (1 << 7);

  /* auto-reload register (TIM10_ARR)
   * 16ms milliseconds LED ON. 4ms milliseconds LED OFF  */
  TIM10->ARR = 256;

  /* prescaler (TIM10_PSC)
   * prescaler value : 1000 */
  TIM10->PSC = 1000;

  /* counter (TIM10_CNT)
   * starting with 0 */
  TIM10->CNT = 0;

  /* capture/compare enable register (TIM11_CCER) */

  /* CC1P : Capture/compare 1 output polarity.
   * CC1 channel configured as output
   * 0 : OCC1 active high
   * write CC1P : 0 to select active high polarity  */
  TIM10->CCER &= ~(1 << 1);

  /* CC1NP : capture/compare 1 complementary output polarity.
   * CC1 channel configured as output : CC1NP must be kept cleared. */
  TIM10->CCER &= ~(1 << 3);

  /* CC1E : Capture/compare 1 output enable.
   * write CC1E : 1 to enable the output.
   * The state of the external I/O pins connected to the standard OCx channels depends on the
   * OCx channel state and the GPIO registers. */
  /* 1 : on - OCC1 signal is output on the corresponding output pin */
  TIM10->CCER |= (1 << 0);

  /* 0C1PE : output comapre 1 preload enable.
   * 1 : preload register on TIM10_CCR1 enable */
  TIM10->CCMR1 |=  (1 << 3);

  /* even generation register (TIM11_EGR)
   * UG : update generation
   * 1 : re-initialize the counter and generate an update of the registers.  */
  TIM10->EGR |= (3 << 0);

  /* control register 1 (TIM10_CR1)
   * CEN : counter enabled */
  TIM10->CR1 |= (1 << 0);
}

void led_blinking(void)
{

  int i, j;
  for (i = 0; i <= 256; i++)  // led on 16ms. led off 4ms. ms means milliseconds
  {
    /* status register (TIM10_SR) */
    while(!(TIM10->SR & (1 << 0)));  //
    TIM10->SR &= ~(3 << 0);   // clear the status register. UIF 0th bit and CC1IF bit
    TIM10->CCR1 = i;          // value load to actual capture/compare 1 register (preload value )
  }

  for (j = 256; j >= 0; j--)
  {
    while(!(TIM10->SR & (1 << 0)));
    TIM10->SR &= ~(3 << 0);
    TIM10->CCR1 = j;
  }
}
