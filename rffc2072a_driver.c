#include<msp430g2553.h>
#include"rffc2072a_driver.h"


/*
   addr: 7bits register num
   rdata : read back number
*/

int rffc2072_read(char addr, int* rdata)
{
    int i =0,j;
	*rdata = 0;

    SDATA_SET_OUT();
	SCLK_CLR();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_SET();
	for(j=0;j<DELAY_CNT;j++)
			;

	
	
	SCLK_CLR();
	ENX_CLR();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_SET();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_CLR();
	for(j=0;j<DELAY_CNT;j++)
			;
	
	SDATA_SET();
	SCLK_SET();
	for(j=0;j<DELAY_CNT;j++)
			;
	for(i=6;i>=0;i--)
	{
        SCLK_CLR();
		if((addr>>i) & 0x1)
			SDATA_SET();
		else
			SDATA_CLR();
		SCLK_SET();
		for(j=0;j<DELAY_CNT;j++)
			;
	}
	SDATA_SET_IN();
	SCLK_CLR();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_SET();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_CLR();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_SET();
	for(j=0;j<DELAY_CNT;j++)
			;
	for(i=15;i>=0;i--)
	{
	    SCLK_CLR();
		*rdata += (SDATA_BIT_READ()<<i);
		SCLK_SET();
		for(j=0;j<DELAY_CNT;j++)
			;
	}
	ENX_SET();
	SCLK_CLR();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_SET();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_CLR();
	SDATA_SET_OUT();

	return 1;
		
}

/*
    addr : register number being writed
    wdata : number being writed into
*/
int rffc2072_write(char addr, int wdata)
{
    int i,j;
	SDATA_SET_OUT();

	SCLK_CLR();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_SET();
	for(j=0;j<DELAY_CNT;j++)
			;

	
    ENX_CLR();
	SCLK_CLR();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_SET();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_CLR();
	for(j=0;j<DELAY_CNT;j++)
			;
	SDATA_CLR();
	SCLK_SET();
	for(j=0;j<DELAY_CNT;j++)
			;
	for(i=6;i>=0;i--)
	{
        SCLK_CLR();
		if((addr>>i) & 0x1)
			SDATA_SET();
		else 
			SDATA_CLR();
		SCLK_SET();
		for(j=0;j<DELAY_CNT;j++)
                  ;
	}
	for(i=15;i>=0;i--)
	{
	    SCLK_CLR();
		if((wdata>>i) & 0x1)
			SDATA_SET();
		else 
			SDATA_CLR();
		SCLK_SET();
		for(j=0;j<DELAY_CNT;j++)
			;
	}
	ENX_SET();
	SCLK_CLR();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_SET();
	for(j=0;j<DELAY_CNT;j++)
			;
	SCLK_CLR();

	return 1;
	
}

int rffc2072_reg[RFFC2072_REGS_NUM]=
	{
	    0xbefa,   /* 00 */	
	    0x4064,   /* 01 */	
	    0x9055,   /* 02 */	
	    0x2d02,   /* 03 */	
	    0xacbf,   /* 04 */	
	    0xacbf,   /* 05 */	
	    0x0028,   /* 06 */	
	    0x0028,   /* 07 */	
	    0xff00,   /* 08 */	
	    0x8220,   /* 09 */	
        0x0202,   /* 0A */	
        0x7E00,   /* 0B */	
        0x1a94,   /* 0C */	
        0xd89d,   /* 0D */	
        0x8900,   /* 0E */	
        0x1e84,   /* 0F */	
        0x89d8,   /* 10 */	
        0x9d00,   /* 11 */	
        0x2a20,   /* 12 */	
        0x0000,   /* 13 */	
        0x0000,   /* 14 */	
        0x0000,   /* 15 */	
        0x0000,   /* 16 */	
        0x4900,   /* 17 */	
        0x0281,   /* 18 */	
        0xf00f,   /* 19 */	
        0x0000,   /* 1A */	
        0x0000,   /* 1B */	
        0xc840,   /* 1C */	
        0x1000,   /* 1D */	
        0x0005,   /* 1E */ 
        };
		
void rffc2072_read_default_setting()
{
    int i,j;
	for(i=0;i<RFFC2072_REGS_NUM;i++)
	{
		rffc2072_read((char)i,&rffc2072_reg[i]);
		for(j=0;j<10;j++)
			;
	}
}

void rffc2072_set_default_value()
{
    char i;
	for(i=0;i<RFFC2072_REGS_NUM-1;i++)
	{
	    rffc2072_write(i,rffc2072_reg[i]);
	}
}

void rffc2072_set_up()
{
    rffc2072_set_default_value();

	/*
	set ENBL and MODE to be configured via 3-wire IF
    set SIPIN and MODE bit
	*/
	rffc2072_reg[0x15] |= (1<<15) + (1<<13);
	rffc2072_write(0x15,rffc2072_reg[0x15]);

	/*
	set reset bit
	
    rffc2072_reg[0x15] |= 1<<1;
	rffc2072_write(0x15,rffc2072_reg[0x15]);
    delay_cs(1000);
    */
	/*
	set ENBL and MODE to be configured via 3-wire IF
    set SIPIN and MODE bit
	*/
	rffc2072_reg[0x15] |= (1<<15) + (1<<13);
	rffc2072_write(0x15,rffc2072_reg[0x15]);


	/*
    GPOs are active at all time and send LOCK flag to GPO4
    set GATE and LOCK BIT
	*/
	rffc2072_reg[0x16] |= (1<<1) + (1<<0);
	rffc2072_write(0x16,rffc2072_reg[0x16]);

    rffc2072_enable();

}


void rffc2072_disable()
{
    //clear ENBL BIT
	rffc2072_reg[0x15] &= ~(1<<14);
	rffc2072_write(0x15,rffc2072_reg[0x15]);
	
}

void rffc2072_enable()
{
    //set ENBL BIT
    rffc2072_reg[0x15] |= (1<<14);
	rffc2072_write(0x15,rffc2072_reg[0x15]);
}

void delay_cs(int t)
{
    int i;
	while(t>0)
	{
		for(i=0;i<100;i++)
			;
		t--;
	}
}

long long rffc2072_freq_calc(int lo_MHz)
{
    char lodiv;	
	int fvco;	
	char fbkdiv;
	int n;	
	long long tune_freq_hz;	
    long long tmp1, tmp2;
	int p1nmsb;	
	char p1nlsb;		
	/* Calculate n_lo */	
	char n_lo = 0;	
	int x = LO_MAX / lo_MHz;	
	while ((x > 1) && (n_lo < 5)) 
	{		
	    n_lo++;		
		x >>= 1;	
	}	
	lodiv = 1 << n_lo;	
	fvco = lodiv * lo_MHz;	
	
	/* higher divider and charge pump current required above	 
	* 3.2GHz. Programming guide says these values (fbkdiv, n,	 
	* maybe pump?) can be changed back after enable in order to	 
	* improve phase noise, since the VCO will already be stable	 
	* and will be unaffected. */
	
	if (fvco > 3200) 
	{		
	    fbkdiv = 4;		
		rffc2072_reg[0x0] &= ~(7);
		rffc2072_reg[0x0] |= 3;
		rffc2072_write(0x0,rffc2072_reg[0x0]);
	} 
	else 
	{		
	    fbkdiv = 2;	
		rffc2072_reg[0x0] &= ~(7);
		rffc2072_reg[0x0] |= 2;
		rffc2072_write(0x0,rffc2072_reg[0x0]);
	}
	
	long long tmp_n = ((long long)fvco << 29ULL) / (fbkdiv*REF_FREQ) ;	
	n = tmp_n >> 29ULL;	
	p1nmsb = (tmp_n >> 13ULL) & 0xffff;	
	p1nlsb =  (tmp_n>> 5ULL)  & 0xff;
	
	tmp1 = (REF_FREQ * (tmp_n>> 5ULL) * fbkdiv * FREQ_ONE_MHZ);
	tmp2 = (lodiv * (1ULL << 24ULL));	
    tune_freq_hz = tmp1 / tmp2;
	/* Path 2 */
    //p2_freq1
	rffc2072_reg[0x0f] &= ~(0x3fff<<2);
	rffc2072_reg[0x0f] |= (n<<7) + (n_lo<<4) + ((fbkdiv>>1)<<2);
    rffc2072_write(0x0f,rffc2072_reg[0x0f]);
	
    //p2_freq2
    rffc2072_reg[0x10] = p1nmsb;
	rffc2072_write(0x10,rffc2072_reg[0x10]);

	//p2_freq3
	rffc2072_reg[0x11] = (p1nlsb<<8);
	rffc2072_write(0x11,rffc2072_reg[0x11]);
		
	return tune_freq_hz;
}

long long rffc2072_freq_set(int lo_MHz)
{
    long long set_freq;

	rffc2072_disable();
	set_freq = rffc2072_freq_calc(lo_MHz);
	rffc2072_enable();

	return set_freq;
}
