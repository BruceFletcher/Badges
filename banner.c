/**
 * Display a banner image on the matrix display, with a little animation.
 *
 * Author: Bruce Fletcher
 * Licence: http://creativecommons.org/licenses/by/3.0/
 */

#include <avr/pgmspace.h>
#include "timer.h"
#include "matrix.h"

#define ROW_BYTES ((MATRIX_WIDTH/8)*2)  // /8 because bitmask, *2 because red & green

static const unsigned char vhs[] PROGMEM = {
  0x11, 0x78,  // row 1 red bitmask
  0x11, 0x04,  // row 1 green bitmask
  0x11, 0x04,  // row 2 red bitmask
  0x11, 0x38,  // row 2 green bitmask
  0x11, 0x40, 0xa4, 0x00, 0x43, 0xc0, 0x00, 0x02,  // row 3, 4
  0x07, 0xa2, 0x00, 0x62, 0x00, 0x6e, 0x03, 0xb2,  // row 5, 6
  0x04, 0x22, 0x04, 0x22, 0x03, 0xe0, 0x00, 0x00   // row 7, 8
};

static volatile char tick;      // Timer tick, indicating when to update the display

static void banner_callback(void)
{
  ++tick;
}

/**
 * Perform an animated wipe left-to-right or right-to-left.
 */
static void wipe(unsigned char stage)
{
  unsigned char last_tick;
  unsigned char x, y;
  unsigned char byte, bit;
  unsigned char index, mask;
  matrix_color_t color;

  for (tick = 0, last_tick = 255; tick < MATRIX_WIDTH;)
  {
    if (last_tick == tick)
      continue;

    last_tick = tick;

    if (stage == 2)
     x = MATRIX_WIDTH - 1 - last_tick;  // wipe right to left
    else
      x = last_tick;                    // wipe left to right

    byte = x / 8;
    bit  = x % 8;
    mask = 1 << bit;

    for (y=0; y<MATRIX_HEIGHT; ++y)
    {
      index = y * ROW_BYTES + byte;

      if (stage == 3)
      {
        color.red = 0;
        color.green = 0;
      }
      else
      {
        color.red = (pgm_read_byte(vhs[index]) & mask) ? 255 : 0;

        index += ROW_BYTES / 2;  // move from red to green data

        color.green = (pgm_read_byte(vhs[index]) & mask) ? 255 : 0;

        if (stage == 2)
        {
          color.red   = 255 - color.red;
          color.green = 255 - color.green;
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
  unsigned char timer;

  matrix_clear_all();

  timer = timer_set_callback(2, TIMER_REPEAT, &banner_callback);

  wipe(1);  // left to right, normal colours
  wipe(2);  // right to left, inverted colours
  wipe(3);  // left to right, clearing display

  timer_clear_callback(timer);
}

