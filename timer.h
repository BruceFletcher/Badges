/**
 * Interrupt-driven timer for callback functions.
 *
 * Timer periods for callbacks are measured in 100ths of a second.
 *
 * Author: Bruce Fletcher
 * Licence: http://creativecommons.org/licenses/by/3.0/
 */

/* Number of concurrent timers supported: */
#define TIMER_MAX_CALLBACKS 4

typedef enum
{
  TIMER_ONE_SHOT = 0,
  TIMER_REPEAT = 1
} timer_config_t;

/**
 * Called on system startup to initialize timer.
 */
void timer_init();

/**
 * Originate a callback.
 *
 * Note that this will always succeed, and may wipe out an existing timer
 * entry if TIMER_MAX_CALLBACKS callbacks are currently active.
 *
 * Returns a reference to use in stopping callbacks.
 */
unsigned char timer_set_callback(unsigned int interval, timer_config_t repeat, void (*callback)(void));

/**
 * Stop a callback.
 */
void timer_clear_callback(unsigned char reference);

