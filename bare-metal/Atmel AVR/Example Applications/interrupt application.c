/*
 * GccApplication4.c
 *
 * Created: 30.6.2015 23:44:55
 *  Author: Muhammed
 */ 

#include <avr/interrupt.h>
#include <util/delay.h>

volatile short i=0;

ISR(INT0_vect)
{
	PORTB=PORTB+1;
	_delay_ms(200);
}
ISR(INT1_vect)
{
	PORTB=PORTB-1;
	_delay_ms(200);
}
int main(void)
{
	DDRD=0;
	DDRB=0xFF;
	PORTB=0;
	PORTD=0;
	
	
    while(1)
    {
       SREG = 128;
	   GICR = 0b11000000;
	   MCUCR |= _BV(ISC01) | _BV(ISC11);
	   MCUCR = MCUCR &-_BV(ISC00) | -_BV(ISC10); 
    }
}
