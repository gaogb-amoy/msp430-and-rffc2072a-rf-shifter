#include  <msp430g2553.h>
#include "initial.h"
#include "UART.h"
#include "rffc2072a_driver.h"
#include "GPIO.h"

//#define DEBUG

#define INIT_LO_MHz 230

char uart_rxd = 0;
char uart_rx_addr;
int  uart_rx_data;
int  write_fsm = 0;
int  read_fsm = 0;
int  set_freq_fsm = 0;
int  uart_rx_lo_mhz;

int  uart_rx_int_flag = 0;

int rffc2072_read_data;

void main(void)
{

#ifdef DEBUG
    int test_temp = 0;
    char addr=0;
    int * rdata;
    int i;
#endif
    
    WDTCTL = WDTPW + WDTHOLD;                 

    long long freq;
    clk_init();
    GPIO_init();
    UART_init();
	rffc2072_set_up();
	delay_cs(1000);
	freq = rffc2072_freq_set(INIT_LO_MHz);
    _EINT();
    while(1)
    {
        if(uart_rx_int_flag)
        {
			if((uart_rxd=='#') && ((write_fsm == 0)|(read_fsm == 0)))
			{
			    write_fsm++;
				read_fsm++;
				set_freq_fsm ++;
			}
			else if((uart_rxd=='#') && ((write_fsm == 1)|(read_fsm == 1)))
			{
			    write_fsm++;
				read_fsm++;
				set_freq_fsm ++;
			}
			else if((uart_rxd== 0x0) && (write_fsm == 2))
			{
			    write_fsm ++;
				read_fsm = 0;
				set_freq_fsm = 0;
			}
			else if(write_fsm==3)
			{
			    write_fsm ++;
				uart_rx_addr = uart_rxd;
			}
			else if(write_fsm == 4)
			{
			    write_fsm++;
				uart_rx_data = (uart_rxd<<8);
			}
			else if(write_fsm == 5)
			{
			    write_fsm ++;
				uart_rx_data += uart_rxd;
			}
			else if((write_fsm == 6) && (uart_rxd == '\r'))
			{
			    write_fsm++;
			}
			else if((write_fsm == 7) && (uart_rxd == '\n'))
			{
			    write_fsm = 0;
				if(uart_rx_addr<RFFC2072_REGS_NUM)
				{
                    rffc2072_reg[uart_rx_addr] = uart_rx_data;
					rffc2072_write(uart_rx_addr,rffc2072_reg[uart_rx_addr]);
				    //rffc2072_read(uart_rx_addr,&rffc2072_reg[uart_rx_addr]);
				    UART_send_string("O");
				}
				else
					UART_send_string("wrong write addr number\r\n");
			}
			else if((uart_rxd== 0x1) && (read_fsm == 2))
			{
			    write_fsm = 0;
				set_freq_fsm = 0;
				read_fsm ++;
			}
			else if(read_fsm==3)
			{
                read_fsm ++;
				uart_rx_addr = uart_rxd;
			}
			else if((uart_rxd == '\r') && (read_fsm == 4))
			{
			    read_fsm ++;
			}
			else if((uart_rxd =='\n') && (read_fsm == 5))
			{
			    read_fsm = 0;
				if(uart_rx_addr<RFFC2072_REGS_NUM)
				{
                    rffc2072_read(uart_rx_addr, &rffc2072_reg[uart_rx_addr]);
					UART_tx_rgst_data(uart_rx_addr, rffc2072_reg[uart_rx_addr]);
				}
				else
					UART_send_string("wrong read addr number\r\n");
			}
			else if((uart_rxd == 0x2) && (set_freq_fsm == 2))
			{
			    set_freq_fsm++;
				read_fsm = 0;
				write_fsm = 0;
			}
			else if(set_freq_fsm == 3)
			{
			    set_freq_fsm ++;
				uart_rx_lo_mhz = uart_rxd<<8;
			}
			else if(set_freq_fsm == 4)
			{
			    set_freq_fsm ++;
				uart_rx_lo_mhz += uart_rxd;
			}
			else if((uart_rxd == '\r') && (set_freq_fsm == 5))
			{
			    set_freq_fsm ++;
			}
			else if((uart_rxd == '\n') && (set_freq_fsm == 6))
			{
			    set_freq_fsm = 0;
				rffc2072_freq_set(uart_rx_lo_mhz);
				UART_send_string("O");
			}
			else
			{
			    UART_send_string("wrong word\r\n");
				write_fsm = 0;
				read_fsm =0;
				set_freq_fsm = 0;
				#ifdef DEBUG
                    for(addr=0;addr<RFFC2072_REGS_NUM;addr++)
                    {
                        rffc2072_read(addr,&rffc2072_reg[addr]);
		                UART_tx_rgst_data(addr,rffc2072_reg[addr]);
                    }
				#endif
			}

            uart_rx_int_flag = 0;
			
        }

#ifdef DEBUG

#else
		//rffc2072_read(0x1f,&test_temp);
		//UART_tx_rgst_data(0x1f,test_temp);
#endif    
    }
}


#pragma vector = TIMER0_A0_VECTOR
__interrupt void TimerA(void)
{

}

#pragma vector = PORT1_VECTOR
__interrupt void Port_ISR(void)
{
  P1IFG &= ~BIT2;
} 

#pragma vector=USCIAB0RX_VECTOR 
__interrupt void USCI0RX_ISR(void)
{
    IFG2 &= ~UCA0RXIFG;
    uart_rxd = UCA0RXBUF;
    uart_rx_int_flag = 1;
}