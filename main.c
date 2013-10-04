/**
 * Badges - Maybe we do need them, after all.
 *
 * Author: Bruce Fletcher
 * Licence: http://creativecommons.org/licenses/by/3.0/
 */

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "uart.h"
#include "banner.h"


/**
 * Simple heartbeat indicator.
 */
void toggle_led(void)
{
  PORTB ^= (1 << DDB5);   // toggle that LED, toggle it!
}


int main(void)
{
  timer_init();
  uart_init();

  PORTB &= ~(1 << PORTB5);  // set B5 lo
  DDRB |= (1 << DDB5);      // and make it an output

  sei();  // enable interrupts

  banner_run();

  toggle_led();

  timer_set_callback(100, 1, &toggle_led);

  while (1)
  {
  }
}

