/**
 * Button reading functions
 *
 * Author: Bruce Fletcher
 * Licence: http://creativecommons.org/licenses/by/3.0/
 */


typedef enum
{
  BUTTON_HAT_NONE = 0,
  BUTTON_HAT_UP,
  BUTTON_HAT_DOWN,
  BUTTON_HAT_RIGHT,
  BUTTON_HAT_LEFT,
  BUTTON_HAT_CENTER
} button_hat_state_t;


void buttons_init();

button_hat_state_t button_get_hat_state(void);

inline char button_get_aux_state(void);

