#include"initial.h"
#include<msp430g2553.h>
void clk_init()
{
   DCOCTL = CALDCO_1MHZ;// DCOCTL = DCO0+DCO1+DCO2;  //
   BCSCTL1 = CALBC1_1MHZ;//BCSCTL1 = RSEL3 +  RSEL2 + RSEL1 + RSEL0; 
   BCSCTL2 = 0;
   BCSCTL3 = 0;
}

void clk_init_ext()
{
   DCOCTL = DCO0+DCO1+DCO2;  //
   BCSCTL1 = RSEL2+RSEL1+RSEL0; 
   //BCSCTL2 = 0;
  // BCSCTL3 = 0;
   
}

void timerA_init()
{
   TACTL |= TASSEL1 + ID1 + ID0 + MC0 + TACLR;  //
   CCTL0 = CCIE;
   CCR0 = 61598; //250ms
   TACCTL1 = 0;
   TACCR1 = 0;
   TACCTL2 = 0;
   TACCR2 = 0;
}

void timerA_init_ext()
{
   TACTL |= TASSEL1 + ID1 + ID0 + MC0 + TACLR;  //SMCLK; 0DIV; UP MODE 
   CCTL0 = CCIE;
   CCR0 = 51960; //250ms
   TACCTL1 = 0;
   TACCR1 = 0;
   TACCTL2 = 0;
   TACCR2 = 0;
}

void delay_cycles(unsigned int cyc)
{
  unsigned int i = cyc;
  while(i-->=1);
}