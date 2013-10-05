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

//static const unsigned char vhs[] PROGMEM = {
static const unsigned char vhs[] = {
0x11, 0xf0, 0x20, 0xf4,
0x11, 0x08, 0x20, 0x0c,
0x11, 0x08, 0x20, 0x0c,
0x11, 0x70, 0xe0, 0x77,
0x11, 0x80, 0x20, 0x84,
0x0a, 0x80, 0x20, 0x84,
0x04, 0x78, 0x20, 0x7c,
0x00, 0x00, 0x00, 0x00 };

static volatile unsigned char tick;      // Timer tick, indicating when to update the display

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

  unsigned char loop_count = 0;

  for (tick = 0, last_tick = 255; tick < MATRIX_WIDTH;)
  {
    if (++loop_count >= 255 )
    {
      matrix_timer_callback();
      loop_count = 0;
    }

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
      index = (MATRIX_HEIGHT - 1 - y) * ROW_BYTES + byte;

      if (stage == 3)
      {
        color.red = 0;
        color.green = 0;
      }
      else
      {
//        color.red = (pgm_read_byte(vhs[index]) & mask) ? 255 : 0;
        color.red = (vhs[index] & mask) ? 255 : 0;

        index += ROW_BYTES / 2;  // move from red to green data

//        color.green = (pgm_read_byte(vhs[index]) & mask) ? 255 : 0;
        color.green = (vhs[index] & mask) ? 255 : 0;

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
  unsigned char timer;

  matrix_clear_all();

  timer = timer_set_callback(4, TIMER_REPEAT, &banner_callback);

  while (1)
  {
    wipe(1);  // left to right, normal colours
    wipe(2);  // right to left, inverted colours
  }

  wipe(3);  // left to right, clearing display

  timer_clear_callback(timer);
}

