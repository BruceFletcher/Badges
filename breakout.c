/**
 * A breakout game for the VHS badge
 *
 * Author: Bruce Fletcher
 * Licence: http://creativecommons.org/licenses/by/3.0/
 */

#include "matrix.h"
#include "timer.h"
#include "breakout.h"

static unsigned int tick;

static void tick_callback(void)
{
  ++tick;
}

static void setup_board(void)
{
  matrix_color_t color;

  matrix_clear_all();

  color.red   = 255;
  color.green = 255;

  matrix_set_pixel(1, 1, color);
}

static void setup_player(void)
{
}

static void wait_for_start(void)
{
}

static void play(void)
{
}

static char endgame(void)
{
  while (1)
  {
  }

  return 0;
}

void breakout_run(void)
{
  unsigned char timer = timer_set_callback(4, 1, &tick_callback);

  while (1)
  {
    setup_board();

    setup_player();

    wait_for_start();

    play();

    if (endgame())
      break;
  }

  timer_clear_callback(timer);
}

