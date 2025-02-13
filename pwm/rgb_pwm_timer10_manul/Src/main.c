/******************************************************************************
 *                                                                            *
 *  Author      : Murugan M                                                   *
 *  Date        : 05-01-2025                                                  *
 *  File        : pwm_timer10_manual                                          *
 *  Description : Led blink for PORTA 0th pin.                                *
 *                using for two pheripheral bus AHB1ENR and APB2ENR           *
 *		            RGB led. R - Red, G - Green, B - Blue                       *
 *                							                                              *
 *****************************************************************************/

#include "arm.h"    /* this header file for all register inside there. using for structure is user-defined data type */

/* main function */

int main(void)
{
  /* function call */

  rcc_config();
  peripheral_bus_config();
  gpioa_moder_config();
  while(1)
  {
    purple_color_led();
    green_color_led();
    white_color_led();
    navy_color_led();
    yellow_color_led();
    red_color_led();
    cyan_color_led();
  }
}

/* function definition */

void rcc_config(void)
{
  /* Reset clock control (RCC_CR) clock control register */

  /* set and clear 16th bit */
  RCC->CR &= (~0x00010000);

  /* select 16th bit HSEON : HSE clock enable. 1 : HSE oscillator ON */
  RCC->CR |= (1 << 16);

  /* select 17th bit HSERDY : HSE clock ready flag. 1 : HSE oscillator ready. HSE external oscillator ready or not checking */
  while(!(RCC->CR & (1 << 17)));

  /* Reset clock control (RCC_CFGR) clock configuration register */

  /* set and clear 0th bit and 1st bit. 11 : not allowed */
  RCC->CFGR &= (~0x00000003);

  /* select 0th bit SW  : System clock switch. 01 : HSE oscillator selected as system clock */
  RCC->CFGR |= (1 << 0);

  /* select 2nd bit SWS : System clock switch status. 01 : HSE oscillator used as the system clock */
  while(!(RCC->CFGR & (0x00000004)));
}

void peripheral_bus_config(void)
{
  /* AHB1ENR Advanced High Bus 1 (RCC_AHB1ENR) peripheral enable register	*/
  RCC->AHB1ENR |= (1 << 0);  // GPIOAEN : IO port A clock enable. 0th bit set 1

  RCC->APB2ENR |= (1 << 17);  // TIMER10EN : TIM10 clock enable. 17th bit set 1
}

void gpioa_moder_config(void)
{
  /* (GPIO) General Purpose Input and Output (GPIOA_MODER) port mode register */
  /* PORT A MODER0 0th pin select. 0th bit set 1. */
  GPIOA->MODER |= (0x00000015);   // 01 : General purpose output mode
}

void timer10_config(void)
{
  /* (TIM10_CR1) control register 1 */

  TIM10->CR1 |= (1 << 1);     // UDIS : update disable. 1 : UEV disabled

  TIM10->CR1 &= ~(1 << 2);    //  URS : Update request source. 0 : Any of following events generate an UEV if enabled

  TIM10->CR1 |= (1 << 7);     // ARPE : Auto-reload preload enable, 1 : TIM10_ARR register is buffered

  TIM10->CNT = 0;             // counter value 0
}

void timer10_delay(int val)
{
  TIM10->PSC = 999;          // prescaler division by 1 from 4

  TIM10->ARR = val;          // 1ms milliseconds Auto reload value on, 19ms milliseconds off

  TIM10->CR1 |= (1 << 0);    // CEN : Counter enable. 1: Counter enable

  while(!(TIM10->SR & (1 << 0)));   // Update interrupt flag.  1 : Update interrupt pending.

  TIM10->SR &= ~(1 << 0);   // clear by software
}

void purple_color_led(void)
{
  /* purple color value : R = 128, G = 0, B = 128 */

  int j = 10, m = 255;   /* 255 is common value. purple color calculate means this value come : 50.2% 10 milliseconds on. 5ms off */
  for (j = 10; j <= 255; j = j + 5)
  {
    GPIOA->ODR |= (1 << 0);    // red color led on
    GPIOA->ODR |= (1 << 2);    // blue color led on
    timer10_delay(j);        

    GPIOA->ODR &= ~(1 << 0);   // red color led off
    GPIOA->ODR &= ~(1 << 2);   // blue color led off
    timer10_delay(m);          // value 255 means fully OFF
    m = m - 5;
  }

  j = 255;
  for (int i = 10; i <= 255; i = i + 5)
  {
    GPIOA->ODR |= (1 << 0);   // red color led on
    GPIOA->ODR |= (1 << 2);   // blue color led on
    timer10_delay(j);

    GPIOA->ODR &= ~(1 << 0);  // red color led off
    GPIOA->ODR &= ~(1 << 2);  // blue color led off
    timer10_delay(i);
    j = j - 5;
  }

}


void navy_color_led(void)
{
  /* navy color value : R = 0, G = 0, B = 128 */

  int p = 10, q = 255;   /* 255 is common value. navy color calculate means this value come : 50.2% 10 milliseconds on. 5ms off */
  for (p = 10 ; p <= 255; p = p + 5)
  {
    GPIOA->ODR |= (1 << 2);  // Blue color led on
    timer10_delay(p);        

    GPIOA->ODR &= ~(1 << 2);   // Blue color led off
    timer10_delay(q);         
    q = q - 5;
  }

  p = 255;
  for (int s = 10; s <= 255; s = s + 5)
  {
    GPIOA->ODR |= (1 << 2);   // Blue color led on
    timer10_delay(p);

    GPIOA->ODR &= ~(1 << 2);   // Blue color led off
    timer10_delay(s);
    p = p - 5;
  }
}


void yellow_color_led(void)
{
  /* yellow color value : R = 255, G = 255, B = 0. */

  int b = 16, t = 255;    /* 255 is common value. yellow color value take 200%. 200 calculate means 16ms milliseconds on. 4ms off */
  for (b = 16; b <= 255; b = b + 4)
  {
    GPIOA->ODR |= (1 << 0);    // red color led on
    GPIOA->ODR |= (1 << 1);    // green color led on
    timer10_delay(b);

    GPIOA->ODR &= ~(1 << 0);   // red color led off
    GPIOA->ODR &= ~(1 << 1);   // green color led off
    timer10_delay(t);
    t = t - 4;
  }

  b = 255;
  for (int y = 16; y <= 255; y = y + 4)
  {
    GPIOA->ODR |= (1 << 0);  // red color led on
    GPIOA->ODR |= (1 << 1);  // green color led on
    timer10_delay(b);

    GPIOA->ODR &= ~(1 << 0);  // red color led off
    GPIOA->ODR &= ~(1 << 1);  // green color led off
    timer10_delay(y);
    b = b - 4;
  }
}

void white_color_led()
{
  /* white color value : R = 255, G = 255, B = 255 */

  int h = 16, r = 255;     /* 255 is common value. white color value take 200%. 200 calculate means 16ms milliseconds on. 4ms off */
  for (h = 16; h <= 255; h = h + 4)
  {
    GPIOA->ODR |= (1 << 0);  // red color led on
    GPIOA->ODR |= (1 << 1);  // green color led on
    GPIOA->ODR |= (1 << 2);  // blue color led on
    timer10_delay(h);

    GPIOA->ODR &= ~(1 << 0);  // red color led off
    GPIOA->ODR &= ~(1 << 1);  // green color led off
    GPIOA->ODR &= ~(1 << 2);  // blue color led off
    timer10_delay(r);
    r = r - 4;
  }

  h = 255;
  for (int w = 16; w <= 255; w = w + 4)
  {
    GPIOA->ODR |= (1 << 0);  // red color led on
    GPIOA->ODR |= (1 << 1);  // green color led on
    GPIOA->ODR |= (1 << 2);  // blue color led on
    timer10_delay(h); 

    GPIOA->ODR &= ~(1 << 0);  // red color led off
    GPIOA->ODR &= ~(1 << 1);  // red color led off
    GPIOA->ODR &= ~(1 << 2);  // red color led off
    timer10_delay(w);
    h = h - 4;
  }
}

void red_color_led(void)
{
  /* red color value : R = 255, G = 0, B = 0 */

  int d = 16, e = 255;    /* 255 is common value. red color value take 200%. 200 calculate means 16ms milliseconds on. 4ms off */
  for (d = 16; d <= 255; d = d + 4)
  {
    GPIOA->ODR |= (1 << 0);  // red color led on
    timer10_delay(d);

    GPIOA->ODR &= ~(1 << 0);  // red color led off
    timer10_delay(e);
    e = e - 4;
  }

  d = 255;
  for (int rd = 16; rd <= 255; rd = rd + 4)
  {
    GPIOA->ODR |= (1 << 0);  // red color led on
    timer10_delay(d);

    GPIOA->ODR &= ~(1 << 0);  // red color led off
    timer10_delay(rd);
    d = d - 4;
  }
}

void green_color_led(void)
{
  /* green color value : R = 0, G = 255, B = 0 */

  int gr = 16, en = 255;  /* 255 is common value. green color value take 200%. 200 calculate means 16ms milliseconds on. 4ms off */
  for (gr = 16; gr <= 255; gr = gr + 4)
  {
    GPIOA->ODR |= (1 << 1);   // green color led on
    timer10_delay(gr);

    GPIOA->ODR &= ~(1 << 1);  // green color led off
    timer10_delay(en);
    en = en - 4;
  }

  gr = 255;
  for (int gn = 16; gn <= 255; gn = gn + 4)
  {
    GPIOA->ODR |= (1 << 1);  // green color led on
    timer10_delay(gr);

    GPIOA->ODR &= ~(1 << 1);  // green color led off
    timer10_delay(gn);
    gr = gr - 4;
  }
}

void cyan_color_led(void)
{
  /* cyan color value : R = 0, G = 255, B = 255  */

  int cy = 16, an = 255;   /* 255 is common value. green color value take 200%. 200 calculate means 16ms milliseconds on. 4ms off */
  for (cy = 16; cy <= 255; cy = cy + 4)
  {
    GPIOA->ODR |= (1 << 1);  // green color led on
    GPIOA->ODR |= (1 << 2);  // blue color led on
    timer10_delay(cy);

    GPIOA->ODR &= ~(1 << 1);  // green color led off
    GPIOA->ODR &= ~(1 << 2);  // blue color led off
    timer10_delay(an);
    an = an - 4;
  }

  cy = 255;
  for (int nc = 16; nc <= 255; nc = nc + 4)
  {
    GPIOA->ODR |= (1 << 1);  // green color led on
    GPIOA->ODR |= (1 << 2);  // blue color led on
    timer10_delay(cy);

    GPIOA->ODR &= ~(1 << 1);  // green color led off
    GPIOA->ODR &= ~(1 << 2);  // blue color led off
    timer10_delay(nc);
    cy = cy - 4;
  }
}
