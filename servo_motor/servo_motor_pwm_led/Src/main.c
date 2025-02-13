/***************************************************************************************************************
 *                                                                                                             *
 *  Author      : Murugan M                                                                                    *
 *  Date        : 11-02-2025                                                                                   *
 *  File        : servo_motor_pwm_led_timer10                                                                  *
 *  Description : Led blink for PORTB 8th pin.                                                                 *
 *                using for two pheripheral bus AHB1ENR and APB2ENR in TIMER 10                                *
 *		  using pwm register for timer10                                                               *
 *		  0 degree value for 500 microseconds  (or) 1000 microseconds,                                 *
 *                90 degree value for 1500 microseconds and 180 degree value for 2500 (or) 3000 microseconds   *
 *                using for HSI : internal clock for 16 MHz                                                    *
 **************************************************************************************************************/

#include "arm.h"   /* this header file for all register inside there. using for structure is user-defined data type */

/* main function */

int main(void)
{
  /* function call */
  rcc_config();
  peripheral_bus_config();
  gpioa_gpiob_moder_config();
  timer10_pwm_config();
  while(1)
  {
    servo_motor_pwm_0_degree();
    servo_motor_pwm_90_degree();
    servo_motor_pwm_180_degree();
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
  while((RCC->CFGR & (1 << 3)) != (0 << 2));
}

void peripheral_bus_config(void)
{
  /* Advanced High Bus 1. peripheral clock enable register (RCC_AHB1ENR) */

  /* GPIOAEN : port A clock enable */
  RCC->AHB1ENR |= (1 << 0);  

  /* GPIOAEN : port A clock enable */
  RCC->AHB1ENR |= (1 << 1); 

  /* Advanced peripheral Bus 2. peripheral clock enable register (RCC_APB2ENR) */
  /* TIMEREN : port b TIMER10 clock enable */
  RCC->APB2ENR |= (1 << 17);  // TIMER 10
}

void gpioa_gpiob_moder_config(void)
{
  /* General purpose input and output (GPIOB_MODER) */

  /* PORT A select MODER1 to MODER4 */
  GPIOA->MODER |= (0x00000154);  // 01: General purpose output mode

  /* PORT B MODER8. I select for port B 8th pin for TIMER 10. */
  GPIOB->MODER |= (1 << 17);  // 17th bit set 1. 10 : Alternate function mode

  /* output speed register (GPIOB_OSPEEDR) */
  GPIOB->OSPEEDR |= (3 << 16);  // 10 : High speed

  /* pull-up/pull-down register (GPIOB_PUPDR) */
  GPIOB->PUPDR |= (1 << 17);  // 17th bit set 1. 10 : pull-down.

  /* alternate function high register (GPIOB_AFRH) */
  GPIOB->AFRH |= (3 << 0);    // 0011 : AF3, timer10 pin. PORTB 8th pin
}

void timer10_pwm_config(void)
{
  /* CCER : capture/compare enable register (TIM10_CCER) */

  // CC1E : capture/compare 1 output enable, 0 : off - OC1 is not active. CC1E 0th bit
  TIM10->CCER &= ~(1 << 0);

  /* capture/compare mode register 1 (TIM10_CCMR1) */

  /* OC1PE : output compare 1 preload enable. 0 : preload register on TIM10_CCR1 disabled.
   *  write OC1PE = 0 to disable preload register. OC1M 4th bit */
  TIM10->CCMR1 |= (6 << 4);

  /* OC1PE : output compare 1 preload enable. 0 : preload register on TIM10_CCR1 disabled.
   *  write OC1PE = 0 to disable preload register. OC1PE 3rd bit */
  TIM10->CCMR1 |= (1 << 3);

  /* OC1FE : output compare 1 fast enable.
   * 1 : An active edge on the trigger input acts like a compare match on CC1 output. OC1FE 2nd bit */
  TIM10->CCMR1 |= (1 << 2);

  /* CC1S : capture/compare 1 selection
   * 00 : CC1 channel is configured as output. CC1S 0th bit */
  TIM10->CCMR1 &= ~(3 << 0);

  /* control register 1 (TIM10_CR1)
   * ARPE : Auto-reload preload enable. 1 : TIM10_ARR register is buffered. ARPE 7th bit set 1 */
  TIM10->CR1 |= (1 << 7);

  /* auto-reload register (TIM10_ARR)
   * 19999 (or) 20000 microseconds . ARR value */
  TIM10->ARR = 19999;

  /* prescaler (TIM10_PSC)
   * prescaler value : 15 */
  TIM10->PSC = 15;

  /* counter (TIM10_CNT)
   * starting with 0 */
  TIM10->CNT = 0;

  /* capture/compare enable register (TIM11_CCER) */

  /* CC1P : Capture/compare 1 output polarity.
   * CC1 channel configured as output
   * 0 : OCC1 active high
   * write CC1P : 0 to select active high polarity. CC1P 1st bit set 0 for high */
  TIM10->CCER &= ~(1 << 1);

  /* CC1E : Capture/compare 1 output enable.
   * write CC1E : 1 to enable the output.
   * The state of the external I/O pins connected to the standard OCx channels depends on the
   * OCx channel state and the GPIO registers. */
  /* 1 : on - OCC1 signal is output on the corresponding output pin. CC1E 0th bit set 1 */
  TIM10->CCER |= (1 << 0);

  /* CC1NP : capture/compare 1 complementary output polarity.
   * CC1 channel configured as output : CC1NP must be kept cleared. */
  TIM10->CCER &= ~(1 << 3);

  /* even generation register (TIM11_EGR)
   * UG : update generation
   * 1 : re-initialize the counter and generate an update of the registers.  */
  TIM10->EGR |= (3 << 0);


  /* control register 1 (TIM10_CR1)
   * CEN : counter enabled */
  TIM10->CR1 |= (1 << 0);
}

void servo_motor_pwm_0_degree(void)
{
  /* servo motor forward side rotate increase */
  for (int i = 0; i <= 1000; i = i + 10)  // servo_motor 0 degree, 500 microseconds (or) 1000 microseconds. 0.5ms milliseconds pulse width
  {
    /* status register (TIM10_SR) */
    while(!(TIM10->SR & (1 << 0)));  // 1: Update interrupt pending.
    TIM10->SR &= ~(3 << 0);     // clear the status register. UIF 0th bit and CC1IF bit
    TIM10->CCR1 = i;            // value load to actual capture/compare 1 register (preload value )
    GPIOA->ODR |= (1 << 1);     // ODR --> output data register. PORTA 1st pin select output led on
  }

  /* servo motor reverse side rotate decrease */
  for (int j = 1000; j >= 0; j = j - 10)
  {
    while(!(TIM10->SR & (1 << 0)));
    TIM10->SR &= ~(3 << 0);
    TIM10->CCR1 = j;
    GPIOA->ODR |= (1 << 1);  // led on
  }
  GPIOA->ODR &= ~(1 << 1); // led off
}

void servo_motor_pwm_90_degree(void)
{
  /* servo motor forward side rotate increase */
  for (int k = 0; k <= 1500; k = k + 10)  // servo_motor 90 degree, 1500 microseconds. 1.5ms milliseconds pulse width
  {
    while(!(TIM10->SR & (1 << 0)));
    TIM10->SR &= ~(3 << 0);
    TIM10->CCR1 = k;
    GPIOA->ODR |= (1 << 2);  // led on 
  }

  /* servo motor reverse side rotate decrease */
  for (int m = 1500; m >= 0; m = m - 10)
  {
    while(!(TIM10->SR & (1 << 0)));
    TIM10->SR &= ~(3 << 0);
    TIM10->CCR1 = m;
    GPIOA->ODR |= (1 << 2); // led on
  }
  GPIOA->ODR &= ~(1 << 2);  // led off
}

void servo_motor_pwm_180_degree(void)
{
  /* servo motor forward side rotate increase */
  for (int p = 0; p <= 2500; p = p + 10) // servo_motor 180 degree, 2500 microseconds (or) 3000 microseconds. 2ms milliseconds pulse width
  {
    while(!(TIM10->SR & (1 << 0)));
    TIM10->SR &= ~(3 << 0);
    TIM10->CCR1 = p;
    GPIOA->ODR |= (1 << 3);  // led on
  }

  /* servo motor reverse side rotate decrease */
  for (int q = 2500; q >= 0; q = q - 10)
  {
    while(!(TIM10->SR & (1 << 0)));
    TIM10->SR &= ~(3 << 0);
    TIM10->CCR1 = q;
    GPIOA->ODR |= (1 << 3); // led on
  }
  GPIOA->ODR &= ~(1 << 3);  // led off
}

