#include <msp430g2553.h>
#include "UART.h"
#include "initial.h"

void UART_tx_rgst_data(char addr, int rdata)
{
    while( !(UCA0TXIFG&IFG2) );//waiting for tx buffer empty
    UCA0TXBUF = '#';
    while( !(UCA0TXIFG&IFG2) );
    UCA0TXBUF = '#';
    while( !(UCA0TXIFG&IFG2) );
    UCA0TXBUF = addr;
    while( !(UCA0TXIFG&IFG2) );
    UCA0TXBUF = (rdata>>8) & 0xff;
    while( !(UCA0TXIFG&IFG2) );
    UCA0TXBUF = rdata & 0xff;
    while( !(UCA0TXIFG&IFG2) );
    UCA0TXBUF = '\r';
    while( !(UCA0TXIFG&IFG2) );
    UCA0TXBUF = '\n';
    return;
}


void UART_send_string(char * str)
{
    char *temp = str;
	while(*temp != '\0')
	{
	    while(!(UCA0TXIFG&IFG2));
		UCA0TXBUF = *temp;
		temp++;
	}
}


void UART_send_addr_data(char addr,int rdata)
{
    while( !(UCA0TXIFG&IFG2) );//waiting for tx buffer empty
    UCA0TXBUF = addr;
    while( !(UCA0TXIFG&IFG2) );
    UCA0TXBUF = (rdata>>8) & 0xff;
    while( !(UCA0TXIFG&IFG2) );
    UCA0TXBUF = rdata & 0xff;
}

void UART_init()
{
 
  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2;
 // UCA0CTL0 |= UCMSB;
  UCA0CTL1 |= UCSSEL0 + UCSSEL1; //select SMCLK for BURD rate gen
  UCA0BR0 = 102;//175;//160;
  UCA0BR1 = 0;
  UCA0MCTL = UCBRS1 + UCBRS0;
  UCA0CTL1 &= ~UCSWRST;
  IE2 |= UCA0RXIE;
}