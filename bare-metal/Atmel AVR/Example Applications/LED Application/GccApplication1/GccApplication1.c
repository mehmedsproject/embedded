/*
 * GccApplication1.c
 *
 * Created: 29.3.2015 17:23:27
 *  Author: Muhammed
 */ 


#include <avr/io.h>
void prep(void){
	DDRB =0xff; // port b yi çýkýþ olarak ayarla.
	PORTB =0x00; // çýkýþlarý sýfýrla.
}
int main(void)
{
    while(1)
    {
      prep();
	  PORTB =_BV(3); // PORTB.3 ü lojik 1 yap.
	  break;         // burada led sürekli yanacak.
    }
}