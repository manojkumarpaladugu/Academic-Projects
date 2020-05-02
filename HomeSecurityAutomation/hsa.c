#include<reg52.h>
#include"declarations.h"
sbit GAS=P1^0;
sbit LDR=P1^3;
sbit PIR=P1^4;
sbit TMP=P1^5;
sbit WND=P1^6;
sbit SMODE_LED=P1^7; //security mode led
sbit BUZ=P2^2;
sbit GAS_LED=P2^3;
sbit TMP_LED=P2^4;
sbit AMODE_LED=P2^5; //automation mode led
sbit INTR_LED=P2^6;  //intruder led
sbit WND_LED=P2^7;
sbit FAN=P3^2;
float f;
char *f2str(float f)
{
	char str[6];
  int i,j;
	i=f*100;
	for(j=4;j>=0;j--)
	{
		if(j==2)
		{
			str[j]='.';
		  continue;
		}
		str[j]=(i%10)+'0';
		i/=10;
	}
	return str;
}
void security(bit flag)
{
	bit flag1=0,flag2=0,flag3=0,flag4=0,flag5=0;
	SMODE_LED=1;
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_str("TURNING ON",1);
	delay_ms(15);
	LCD_cmd(0xc0);
	LCD_str("SECURITY MODE",1);
	delay_ms(250);
	message();
	Serial_Tx_str("Security Turned ON");
	delay_ms(50);
	Serial_Tx(26);
	while(1)
	{
		f=readDS1621();
		if(!PIR && f<=35.0f && GAS && WND)
		{
			if(f<=33.0f)
			{
				FAN=0;
				TMP_LED=0;
			}
			BUZ=0;
			INTR_LED=0;
			GAS_LED=0;
			WND_LED=0;
			flag2=flag3=flag4=flag5=0;
			if(flag1==0)
			  LCD_cmd(0x01);
			LCD_cmd(0x81);
			LCD_str("SECURITY IS ON",0);
			LCD_cmd(0xc0);
			LCD_str("Everything is OK",0);
			flag1=1;
			flag2=0;
			if(!LDR && flag==0)
		  {
			  LCD_cmd(0x01);
			  LCD_cmd(0x80);
		    LCD_str("TURNING OFF",0);
			  LCD_cmd(0xc0);
			  LCD_str("SECURITY MODE",0);
			  delay_ms(1000);
				message();
				Serial_Tx_str("Security Turned OFF");
				delay_ms(50);
				Serial_Tx(26);
			  SMODE_LED=0;
			  return;
		  }
		}
		else
		{
			if(PIR)
			{
				INTR_LED=1;  //intruder led on
				BUZ=1;
				if(flag2==0)
				{
					message();
					Serial_Tx_str("Intruder is in your place.");
					delay_ms(50);
					Serial_Tx(26);
				}
				LCD_cmd(0x01);
				LCD_cmd(0x81);
				LCD_str("INTRUDER IS IN",1);
				LCD_cmd(0xc3);
				LCD_str("YOUR PLACE",1);
				if(f<=33.0f)
				{
					TMP_LED=0;
					FAN=0;
				}
				if(GAS)
				{
					GAS_LED=0;
					BUZ=1;
				}
				if(WND)
				{
					WND_LED=0;
					BUZ=0;
				}
				flag1=0;
		    flag2=1;
				delay_ms(250);
			}
			if(f>35.0f)
			{
				TMP_LED=1;
				FAN=1;
				if(flag3==0)
				{
					message();
					Serial_Tx_str("Temparature is high.");
					delay_ms(50);
					Serial_Tx(26);
				}
				LCD_cmd(0x01);
				LCD_cmd(0x81);
				LCD_str("TEMPERATURE IS",1);
				LCD_cmd(0xc3);
				LCD_str("HIGH",1);
				LCD_cmd(0xc8);
				LCD_str(f2str(f),1);
				if(!PIR)
				{
					INTR_LED=0;
				  BUZ=0;
				}
				if(GAS)
				{
					GAS_LED=0;
					BUZ=1;
				}
				if(WND)
				{
					WND_LED=0;
					BUZ=0;
				}
				flag1=0;
				flag3=1;
				delay_ms(250);
			}
			if(!GAS)
			{
				GAS_LED=1;
				BUZ=1;
				if(flag4==0)
				{
					message();
					Serial_Tx_str("LPG/smoke is detected");
					delay_ms(50);
					Serial_Tx(26);
				}
				LCD_cmd(0x01);
				LCD_cmd(0x84);
				LCD_str("LPG/SMOKE",1);
				LCD_cmd(0xc4);
				LCD_str("DETECTED",1);
				if(!PIR)
				{
					INTR_LED=0;
					BUZ=0;
				}
				if(f<=33.0f)
				{
					TMP_LED=0;
					FAN=0;
				}
				if(WND)
				{
					WND_LED=0;
					BUZ=0;
				}
				flag1=0;
				flag4=1;
				delay_ms(250);
			}
			if(!WND)
			{
				WND_LED=1;
				BUZ=1;
				if(flag5==0)
				{
				  message();
				  Serial_Tx_str("House doors/windows opened");
				  delay_ms(50);
				  Serial_Tx(26);
				}
				LCD_cmd(0x01);
				LCD_cmd(0x82);
				LCD_str("DOORS/WINDOWS",1);
				LCD_cmd(0xc4);
				LCD_str("ARE OPEN",1);
				if(!PIR)
				{
					INTR_LED=0;
					BUZ=0;
				}
				if(f<=33.0f)
				{
					TMP_LED=0;
					FAN=0;
				}
				if(GAS)
				{
					GAS_LED=0;
					BUZ=0;
				}
				flag1=0;
				flag5=1;
				delay_ms(250);
			}
		}
	}
}
main()
{
	unsigned char flag=0;
	BUZ=0;
	FAN=0;
	GAS=1;
	LDR=0;
	PIR=0;
	TMP=0;
	WND=1;
	BUZ=0;
	GAS_LED=0;
	TMP_LED=0;
	INTR_LED=0;
	SMODE_LED=0;
	WND_LED=0;
	LCD_init();
	Serial_init();
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_str("Booting...",1);
	delay_ms(1000);
	LCD_cmd(0x01);
	LCD_cmd(0x80);
	LCD_str("TURNING ON",1);
	LCD_cmd(0xc0);
	LCD_str("AUTOMATION MODE",1);
	delay_ms(1000);
  Serial_Tx_str("ATE0");
	delay_ms(50);
	while(1)
	{
		SMODE_LED=0;
		if(LDR)
		{
			AMODE_LED=0;
			LCD_cmd(0x01);
			LCD_cmd(0x80);
			LCD_str("TURNING OFF",1);
			LCD_cmd(0xc0);
			LCD_str("AUTOMATION MODE",1);
			delay_ms(1000);
			security(0);
			LCD_cmd(0x01);
			LCD_cmd(0x80);
			LCD_str("TURNING ON",1);
			LCD_cmd(0xc0);
			LCD_str("AUTOMATION MODE",1);
			delay_ms(1000);
		}
		if(RI==1)
		{
			if(Serial_Rx()=='R')
				if(Serial_Rx()=='I')
					if(Serial_Rx()=='N')
						if(Serial_Rx()=='G')
						{
						  AMODE_LED=0;
							LCD_cmd(0x01);
							LCD_cmd(0x80);
							LCD_str("TURNING OFF",1);
							LCD_cmd(0xc0);
							LCD_str("AUTOMATION MODE",1);
							delay_ms(1000);
							security(1);
							LCD_cmd(0x01);
							LCD_cmd(0x80);
							LCD_str("TURNING ON",1);
							LCD_cmd(0xc0);
							LCD_str("AUTOMATION MODE",1);
							delay_ms(1000);
		        }				
	  }
		f=readDS1621();
		if(!PIR && GAS && f<=35.0f)
		{
			if(f<=33.0f)
			{
				FAN=0;
				TMP_LED=0;
			}
			AMODE_LED=1;
			GAS_LED=0;
			BUZ=0;
			if(flag==0)
				LCD_cmd(0x01);
			LCD_cmd(0x80);
			LCD_str("AUTOMATION ON",0);
			LCD_cmd(0xc0);
			LCD_str("RELAX YOUR SELF",0);
			flag=1;
			delay_ms(25);
		}
		else
		{
			if(PIR)
			{
				BUZ=1;
				LCD_cmd(0x01);
				LCD_cmd(0x81);
				LCD_str("SOME ONE WANTS",1);
				LCD_cmd(0xc2);
				LCD_str("TO MEET YOU",1);
				if(f<=33.0f)
				{
					TMP_LED=0;
					FAN=0;
				}
				if(GAS)
				{
					GAS_LED=0;
					BUZ=0;
				}
				flag=0;
				delay_ms(100);
			}
			if(f>35.0f)
			{
				TMP_LED=1;
				FAN=1;
			  LCD_cmd(0x01);
				LCD_cmd(0x81);
				LCD_str("TEMPERATURE IS",1);
				LCD_cmd(0xc3);
				LCD_str("HIGH",1);
				LCD_cmd(0xc8);
				LCD_str(f2str(f),1);
				if(!PIR)
				{
					BUZ=0;
				}
				if(GAS)
				{
					GAS_LED=0;
					BUZ=0;
				}
				flag=0;
				delay_ms(100);
			}
			if(!GAS)
			{
				GAS_LED=1;
				BUZ=1;
				LCD_cmd(0x01);
				LCD_cmd(0x82);
				LCD_str("GAS,SMOKE IS",1);
				LCD_cmd(0xc0);
				LCD_str("DETECTED IN HOME",1);
				if(!PIR)
				{
					BUZ=0;
				}
				if(f<=33.0f)
				{
					TMP_LED=0;
					FAN=0;
				}
				flag=0;
				delay_ms(100);
			}
		}
		if(WND)
		{
		  WND_LED=0;
		}
		else
		{
	    WND_LED=1;
		}
	}
}