/**
 * Interrupt-driven timer for callback functions.
 *
 * Timer periods/delays for callbacks are measured in 100ths of a second.
 *
 * Author: Bruce Fletcher
 * Licence: http://creativecommons.org/licenses/by/3.0/
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"


// F_CPU is apparently 8MHz on the badge board
#define TIMER_SCALE        64    // Each timer count is this many F_CPU cycles
#define COUNTS_PER_LOOP   256    // We're using an 8 bit timer, so it counts down from 255 then loops
#define TICKS_PER_SECOND  100    // Our arbitrary tick length
#define COUNTS_PER_TICK   ((F_CPU/TIMER_SCALE)/TICKS_PER_SECOND - 1)
#define LOOPS_PER_TICK    (COUNTS_PER_TICK/COUNTS_PER_LOOP)
#define LOOP_REMAINDER    (COUNTS_PER_TICK - LOOPS_PER_TICK*COUNTS_PER_LOOP)


typedef struct
{
  unsigned int interval;
  unsigned int state;
  void (*callback)(void);
  char repeat;
} callback_t;

/**
 * Each callback gets its own entry in this table.
 */
static callback_t callback_list[TIMER_MAX_CALLBACKS];

/**
 * Loop counter; probably a little over 4 loops per 1/100th of a second tick.
 */
static unsigned char timer_loop_count;

/**
 * Called on system startup to initialize timer.
 */
void timer_init()
{
  TCCR0B |= (1 << CS01) | (1 << CS00);    // Configure timer for F_CPU / 64

  timer_loop_count = LOOPS_PER_TICK+1;

  TIMSK0 |= (1 << TOIE0);  // Enable timer overflow interrupt
}

/**
 * Stuff a callback request into the callback_list.
 *
 * This function is guaranteed to succeed, but may wipe out
 * an existing callback if there's no room in the table.
 *
 * Returns a reference to use in stopping callbacks.
 */
unsigned char timer_set_callback(unsigned int period, timer_config_t repeat, void (*callback)(void))
{
  unsigned char reference;
  callback_t *c;

  for (reference=0; reference < TIMER_MAX_CALLBACKS-1; ++reference)
  {
    if (callback_list[reference].interval == 0)
      break;
  }

  c = &callback_list[reference];

  // This sequence should be interrupt safe:
  c->interval = 0;
  c->state    = period;
  c->callback = callback;
  c->repeat   = repeat;
  c->interval = period;

  return reference;
}

void timer_clear_callback(unsigned char reference)
{
  callback_list[reference].interval=0;
}

/**
 * Interrupt service routine for timer.
 */
ISR(TIMER0_OVF_vect)
{
  unsigned char i;
  callback_t *c;

  if (--timer_loop_count == 0)
  {
    timer_loop_count = LOOPS_PER_TICK+1;

    TCNT0 += 255 - LOOP_REMAINDER;

    for (i=0; i < TIMER_MAX_CALLBACKS; ++i)
    {
      c = &callback_list[i];

      if (c->interval == 0)
        continue;

      if (--c->state == 0)
      {
        (c->callback)();

        if (c->repeat == 0)
          c->interval = 0;
         else
          c->state = c->interval;
      }
    }
  }
}

