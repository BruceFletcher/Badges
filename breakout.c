/**
 * A breakout game for the VHS badge
 *
 * Author: Bruce Fletcher
 * Licence: http://creativecommons.org/licenses/by/3.0/
 */

#include "matrix.h"
#include "timer.h"
#include "buttons.h"
#include "breakout.h"


static unsigned char paddle;   // paddle y location, 2 to 7

#if 0
static unsigned char ball_x;
static unsigned char ball_y;
static char ball_dx;           // -1,    1
static char ball_dy;           // -1, 0, 1
#endif

static const matrix_color_t red    = { 255, 0 };
static const matrix_color_t green  = { 0, 255 };
static const matrix_color_t yellow = { 255, 255 };

static volatile unsigned char tick;

static void tick_callback(void)
{
  ++tick;
}

static void setup_board(void)
{
  unsigned char x, y;

  matrix_clear_all();

  for (x=6; x<MATRIX_WIDTH; x+=3)
  {
    for (y=0; y<MATRIX_HEIGHT; y+=2)
    {
      matrix_set_pixel(x, y, yellow);

      if (y < MATRIX_HEIGHT-1)
      {
        if ((x+y) & 3)
          matrix_set_pixel(x, y+1, red);
        else
          matrix_set_pixel(x, y+1, green);
      }
    }
  }
}

static void setup_player(void)
{
  paddle = 4;
}

static void wait_for_start(void)
{
}

static void play(void)
{
}

static void endgame(void)
{
  while (1)
  {
    if (button_get_aux_state())
      break;
  }
}

void breakout_run(void)
{
  unsigned char timer = timer_set_callback(4, 1, &tick_callback);

  matrix_clear_all();

  matrix_start_scanning();

  setup_board();

  setup_player();

  wait_for_start();

  play();

  endgame();

  matrix_stop_scanning();

  timer_clear_callback(timer);
}

