/*
 * GccApplication1.c
 *
 * Created: 29.3.2015 17:23:27
 *  Author: Muhammed
 */ 


#include <avr/io.h>
void prep(void){
	DDRB =0xff; // port b yi ��k�� olarak ayarla.
	PORTB =0x00; // ��k��lar� s�f�rla.
}
int main(void)
{
    while(1)
    {
      prep();
	  PORTB =_BV(3); // PORTB.3 � lojik 1 yap.
	  break;         // burada led s�rekli yanacak.
    }
}