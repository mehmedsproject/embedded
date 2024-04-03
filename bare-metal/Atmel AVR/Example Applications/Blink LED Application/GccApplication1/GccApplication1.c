/*
 * GccApplication1.c
 *
 * Created: 29.3.2015 18:01:23
 *  Author: Muhammed
 */ 


#include <avr/io.h>
#include <util/delay.h>

void prep(void){
	DDRB = 0xff;
	PORTB = 0x00;
}

int main(void)
{
	while(1)
	{
		prep();
		PORTB = _BV(2);
		_delay_ms(300);
		PORTB = !_BV(2);
		_delay_ms(300);
	}
}