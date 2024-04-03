/*
 * GccApplication7.c
 *
 * Created: 13.7.2015 16:14:13
 *  Author: Muhammed
 */ 


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	
	DDRD = 0x00;
	DDRB = 0x0E;
	
	TCCR2 = 0x69;
	TCCR1A = 0xA1;
	TCCR1B = 0x0A;
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x7D;
	OCR1BH=0x00;
	OCR1BL=0x7D;
	
    while(1)
    {
       
	   
		if (bit_is_set(PIND,0))
		{
			_delay_ms(250);
			OCR2 = 63;
			OCR1A = 63;
			OCR1B = 63;
		}
		else if (bit_is_set(PIND,1))
		{
			_delay_ms(250);
			OCR2 = 127;
			OCR1A = 127;
			OCR1B = 127;
		}
		else if (bit_is_set(PIND,2))
		{
			_delay_ms(250);
			OCR2 = 200;
			OCR1A = 200;
			OCR1B = 200;
		}
		
    }
}