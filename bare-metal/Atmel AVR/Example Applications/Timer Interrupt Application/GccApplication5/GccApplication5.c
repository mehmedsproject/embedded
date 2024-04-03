/*
 * GccApplication5.c
 *
 * Created: 3.7.2015 01:27:30
 *  Author: Muhammed
 */ 
#define F_CPU 1000000UL
#include <avr/interrupt.h>
#include <avr/io.h>
int i=0;
int main(void)
{
	
	DDRB = 0x01;
	
	TCCR2 = (1 << WGM21); // SET CTC BIT
	OCR2 = 156.25; // Output compare register - the total timer ticks value
	TIMSK = (1 << OCIE2); // timer/counter interrupt mask register (timsk) - timer/counter output compare match interrupt enable (ocie2) will be 1
	sei();
	TCCR2 = (1 << CS22); //timer resolution selected as clc/64 prescalar
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}

ISR(TIMER2_COMP_vect)
{
	
	i++;
	if (i > 65)
	{
		PORTB ^= (1 << PINB0);
		i=0;
	}

}