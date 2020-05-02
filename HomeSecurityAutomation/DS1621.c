#include<reg52.h>
#include"types.h"
#include"i2c_h.h"
extern delay_ms(u16);
float readDS1621(void)
{
	u8 mByte,lByte;
	float temp=0.0;
	static s8 i=1,sign;
	
	i2cStart();
	i2cWrite(0x90);
	i2cAck();
	i2cWrite(0xee); //command to start conversion
	i2cAck();
	i2cStop();
	
	if(i==1)
	{
		delay_ms(750);
		i++;
	}

	i2cStart();
	i2cWrite(0x90);
	i2cAck();
	i2cWrite(0xaa); //command to read temparature
	i2cAck();
	i2cStop();
	
	i2cStart();
	i2cWrite(0x91);
	i2cAck();
	mByte=i2cRead(); //read integer part of temparature
	i2cMasterAck();
	lByte=i2cRead();//read fraction part of temparature
	i2cNoAck();
	i2cStop();
	
	i2cStart();
	i2cWrite(0x90);
	i2cAck();
	i2cWrite(0x22); //command to stop conversion
	i2cAck();
	i2cStop();
	
	sign=1;
	if((mByte & 0x80) && (lByte==0x80))
	{
		mByte=~mByte;
		sign=-1;
	}
	else if((mByte & 0x80) && (lByte!=0x80))
	{
		if(mByte!=128)
		{
			mByte=~mByte+1;
			sign=-1;
		}
	}
	
	temp=(sign*(mByte+((lByte>>7)*0.5)));
	return temp;
}