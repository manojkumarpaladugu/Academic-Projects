#include<reg52.h>
extern delay_ms(unsigned int);
void Serial_init()
{
	SCON=0x50;
	TMOD=0x20;
	TH1=0xFD;
	RI=0;
	TI=0;
	TR1=1;
}
void Serial_Tx(char ch)
{
	TI=0;
	SBUF=ch;
	while(TI==0);
	TI=0;
}
void Serial_Tx_str(char *p)
{
	while(*p)
	{
		Serial_Tx(*p++);
	}
}
char Serial_Rx()
{
	char ch;
	RI=0;
	while(RI==0);
	ch=SBUF;
	RI=0;
	return ch;
}
void message( void )
{
	Serial_Tx_str("AT+CMGF=1");
	delay_ms(50);
	Serial_Tx_str("AT+CMGS=\"+919581031137\"");
	delay_ms(50);
}