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
#include "matrix.h"
#include "buttons.h"
#include "uart.h"
#include "banner.h"
#include "breakout.h"


int main()
{
  CLKPR = (1<<CLKPCE);
  CLKPR = 0;  // no prescaling

  timer_init();
  matrix_init();
  buttons_init();

  sei();  // enable interrupts

  while (1)
  {
    banner_run();

    breakout_run();
  }
}

