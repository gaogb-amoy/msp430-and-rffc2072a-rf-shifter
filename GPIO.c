#include "GPIO.h"
#include<msp430g2553.h>

void GPIO_init()
{
  P1DIR |= BIT0 + BIT3 + BIT4 + BIT5;
  P1OUT &= ~( BIT3 + BIT4 );
  P1OUT |= (BIT0 + BIT5);
}

