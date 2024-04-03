/* ATmega8 with internal 4Mhz clock (6cycle + 64ms) */

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
   DDRC |= (1 << PC2) | (1 << PC1);     // leds for testing

   DDRD &= ~(1 << PD2);    // INT0: input...
   PORTD |= (1 << PD2);    // ...with pullup.

   // level interrupt INT0 (low level)
   MCUCR &= ~((1 << ISC01) | (1 << ISC00));

   // infinite main loop
   while (1)
   {
      // trigger leds for testing
      PORTC ^= (1 << PC1);
      _delay_ms(500);
      PORTC ^= (1 << PC1);

      // enable external interrupt
      GICR |= (1 << INT0);

      // set sleep mode
      set_sleep_mode(SLEEP_MODE_PWR_DOWN);

      // sleep_mode() has a possible race condition
      sleep_enable();
      sei();
      sleep_cpu();
      sleep_disable();

      // waking up...
      // disable external interrupt here, in case the external low pulse is too long
      GICR &= ~(1 << INT0);

      // disable all interrupts
      cli();
   }
}


ISR(INT0_vect)
{
   // ISR might be empty, but is necessary nonetheless
   PORTC ^= (1 << PC2);    // debugging
}
