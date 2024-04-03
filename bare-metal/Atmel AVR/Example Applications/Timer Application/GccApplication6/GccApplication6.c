/*
 * GccApplication6.c
 *
 * Created: 4.7.2015 17:29:54
 *  Author: Muhammed
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	
	DDRD = 0xff;
	DDRB = 0x08;
	PORTB= 0x00;
	TCCR2 = 0x69;
	
    while(1)
    {
       
	   PORTD ^= OCR2;
		if (bit_is_set(PINB,0))
		{
			_delay_ms(250);
			OCR2 = 63;
		}
		else if (bit_is_set(PINB,1))
		{
			_delay_ms(250);
			OCR2 = 127;
		}
		else if (bit_is_set(PINB,2))
		{
			_delay_ms(250);
			OCR2 = 200;
		}
		
    }
}