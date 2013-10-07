/**
 * Button reading functions
 *
 * Author: Bruce Fletcher
 * Licence: http://creativecommons.org/licenses/by/3.0/
 */


#include <avr/io.h>
#include "buttons.h"


void buttons_init()
{
  DDRC  &= ~((1<<DDC1)|(1<<DDC2)|(1<<DDC3));   // hat switch input pins
  DDRC  &= !(1<<DDC0);                         // aux switch input pin
  PORTC |= (1<<PORTC0);                        // enable pull-up for aux button
}

button_hat_state_t button_get_hat_state(void)
{
  switch (PINC & ((1<<PINC1)|(1<<PINC2)|(1<<PINC3)))
  {
    case 2:
      return BUTTON_HAT_UP;
      break;

    case 6:
      return BUTTON_HAT_LEFT;
      break;

    case 8:
      return BUTTON_HAT_RIGHT;
      break;

    case 12:
      return BUTTON_HAT_DOWN;
      break;

    case 14:
      return BUTTON_HAT_CENTER;
      break;
  }

  return BUTTON_HAT_NONE;
}

char button_get_aux_state(void)
{
  return !(PINC & (1<<PINC0));
}

