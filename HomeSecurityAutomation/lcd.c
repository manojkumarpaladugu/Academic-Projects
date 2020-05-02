#include<reg52.h>
#define lcd P0
sbit RS=P2^0;
sbit EN=P2^1;
extern delay_ms(unsigned int);
void LCD_write(char dat)
{
	lcd=dat;
	EN=1;
	delay_ms(2);
	EN=0;
}
void LCD_char(char ch,char t)
{
	RS=1;
	if(t==1)
		delay_ms(25);
	LCD_write(ch);
}
void LCD_cmd(char cmd)
{
	RS=0;
	LCD_write(cmd);
}
void LCD_str(char *p,char t)
{
	while(*p)
		LCD_char(*p++,t);
}
void LCD_init(void)
{
	LCD_cmd(0x38);
	LCD_cmd(0x06);
	LCD_cmd(0x0c);
}