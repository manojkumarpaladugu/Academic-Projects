void delay_ms(unsigned int time)
{
	unsigned int i;
	for(;time>0;time--)
		for(i=122;i>0;i--);
}