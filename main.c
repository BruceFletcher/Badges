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
#include "uart.h"
#include "banner.h"
#include "breakout.h"


int main(void)
{
  CLKPR = (1<<CLKPCE);
  CLKPR = 0;  // no prescaling

  timer_init();
  matrix_init();

  sei();  // enable interrupts

  banner_run();

/*
  breakout_run();
*/

  while (1);
}

