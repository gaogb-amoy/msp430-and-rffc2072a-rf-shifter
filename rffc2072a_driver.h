
#include <msp430g2553.h>

int rffc2072_read(char addr, int* rdata);
int rffc2072_write(char addr, int wdata);

#define ENBL_SET() P1OUT |= BIT5
#define ENBL_CLR() P1OUT &= ~BIT5

#define ENX_SET() P1OUT |= BIT0
#define ENX_CLR() P1OUT &= ~BIT0

#define SCLK_SET() P1OUT |= BIT3
#define SCLK_CLR() P1OUT &= ~BIT3

#define SDATA_SET_OUT() P1DIR |= BIT4
#define SDATA_SET_IN()  P1DIR &= ~BIT4
#define SDATA_SET()     P1OUT |= BIT4
#define SDATA_CLR()     P1OUT &= ~BIT4
#define SDATA_BIT_READ() ((P1IN&BIT4)>>4)

#define LO_MAX 5400ULL
#define REF_FREQ 40ULL
#define FREQ_ONE_MHZ (1000ULL*1000ULL)


#define DELAY_CNT 5

#define RFFC2072_REGS_NUM 32
extern int rffc2072_reg[RFFC2072_REGS_NUM];

void rffc2072_read_default_setting();
void rffc2072_set_default_value();
void rffc2072_set_up();
void rffc2072_disable();
void rffc2072_enable();
void delay_cs(int t);
long long rffc2072_freq_calc(int lo_MHz);
long long rffc2072_freq_set(int lo_MHz);

