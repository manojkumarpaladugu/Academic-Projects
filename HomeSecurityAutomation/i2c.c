#include<reg52.h>
#include"i2c_h.h"
sbit SCL=P1^2;
sbit SDA=P1^1;
extern void delay_ms(u16);
void i2cStart(void)
{
  SCL=1;
	SDA=1;
	SDA=0;
}
void i2cStop(void)
{
  SCL=0;
	SDA=0;
	SCL=1;
	SDA=1;
}
void i2cWrite(u8 dat)
{
	u8 i;
  for(i=0;i<8;i++)
	{
		SCL=0;
    SDA=(dat<<i) & 0x80?1:0;
		SCL=1;
	}
}
u8 i2cRead(void)
{
  u8 dat=0,i;
	for(i=0;i<8;i++)
	{
		SCL=0;
	  SCL=1;
		if(SDA)
		  dat|=0x80>>i;
	}
	return dat;
}
void i2cAck(void)
{
  SCL=0;
	SDA=1;
	SCL=1;
	while(SDA);
}
void i2cNoAck(void)
{
  SCL=0;
	SDA=1;
	SCL=1;
}
void i2cMasterAck(void)
{
	SCL=0;
	SDA=0;
	SCL=1;
	SCL=0;
	SDA=1;
}