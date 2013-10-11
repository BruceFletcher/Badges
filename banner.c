/**
 * Display a banner image on the matrix display, with a little animation.
 *
 * Author: Bruce Fletcher
 * Licence: http://creativecommons.org/licenses/by/3.0/
 */

#include <avr/pgmspace.h>
#include "timer.h"
#include "buttons.h"
#include "matrix.h"

#define ROW_BYTES ((MATRIX_WIDTH/8)*2)  // /8 because bitmask, *2 because red & green

static const uint8_t  vhs[] PROGMEM = {
0x11, 0xf0, 0x20, 0xf4,
0x11, 0x08, 0x20, 0x0c,
0x11, 0x08, 0x20, 0x0c,
0x11, 0x70, 0xe0, 0x77,
0x11, 0x80, 0x20, 0x84,
0x0a, 0x80, 0x20, 0x84,
0x04, 0x78, 0x20, 0x7c,
0x00, 0x00, 0x00, 0x00 };

static volatile uint8_t  tick;             // Timer tick, indicating when to update the display
static volatile uint8_t  button_pressed;   // Did anyone hit the aux button yet?


static void banner_callback(void)
{
  ++tick;
}

/**
 * Perform an animated wipe left-to-right or right-to-left.
 */
static void wipe(uint8_t stage)
{
  uint8_t  last_tick;
  uint8_t  x, y;
  uint8_t  byte, bit;
  uint8_t  index, mask;
  matrix_color_t color;

  for (tick = 0, last_tick = 255; tick < MATRIX_WIDTH;)
  {
    if (last_tick == tick)
      continue;

    last_tick = tick;

    if (button_get_aux_state())
      button_pressed = 1;

    if (stage == 2)
     x = MATRIX_WIDTH - 1 - last_tick;  // wipe right to left
    else
      x = last_tick;                    // wipe left to right

    byte = x / 8;
    bit  = x % 8;
    mask = 1 << bit;

    for (y=0; y<MATRIX_HEIGHT; ++y)
    {
      index = (MATRIX_HEIGHT - 1 - y) * ROW_BYTES + byte;

      if (stage == 3)
      {
        color.red = 0;
        color.green = 0;
      }
      else
      {
        color.red = (pgm_read_byte(&vhs[index]) & mask) ? 255 : 0;

        index += ROW_BYTES / 2;  // move from red to green data

        color.green = (pgm_read_byte(&vhs[index]) & mask) ? 255 : 0;

        if (stage == 2)
        {
          // time to cycle some colors:

          if (color.red)
          {
            if (color.green)
              color.red = 0;    // yellow -> green
            else
              color.green = 255;  // red -> yellow
          }
          else
          {
            if (color.green)
            {
              color.red = 255;    // green -> red
              color.green = 0;
            }
          }
        }
      }

      matrix_set_pixel(x, y, color);
    }
  }
}

/**
 * Animate a simple banner on the display.
 */
void banner_run(void)
{
  uint8_t  timer;

  matrix_clear_all();

  timer = timer_set_callback(4, TIMER_REPEAT, &banner_callback);

  button_pressed = 0;

  button_debounce_aux(25);

  matrix_start_scanning();

  while (1)
  {
    wipe(1);  // left to right, normal colours
    wipe(2);  // right to left, inverted colours

    if (button_pressed)
      break;
  }

  wipe(3);  // left to right, clearing display

  matrix_stop_scanning();

  timer_clear_callback(timer);
}

