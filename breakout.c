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


static uint8_t paddle;   // paddle y location, 2 to 7

static uint8_t ball_count;
static uint8_t ball_x;
static uint8_t ball_y;
static int8_t ball_dx;           // -1,    1
static int8_t ball_dy;           // -1, 0, 1

static uint8_t brick_count;

static const matrix_color_t red    = { 255, 0 };
static const matrix_color_t green  = { 0, 255 };
static const matrix_color_t yellow = { 255, 255 };

static volatile uint8_t tick;


static void tick_callback(void)
{
  ++tick;
}

static void setup_board(void)
{
  uint8_t x, y, i;

  matrix_clear_all();

  i=0;

  for (x=6; x<MATRIX_WIDTH; x+=3)
  {
    for (y=0; y<MATRIX_HEIGHT; ++y)
    {
      i = (i+1) % 3;

      switch (i)
      {
        case 0:
          matrix_set_pixel(x, y, yellow);
          break;

        case 1:
          matrix_set_pixel(x, y, red);
          break;

        case 2:
          matrix_set_pixel(x, y, green);
          break;
      }
    }
  }

  brick_count = MATRIX_WIDTH * 4;
}

static void move_paddle(void)
{
  uint8_t button_state, y;

  button_state = button_get_hat_state();

  switch (button_state)
  {
    case BUTTON_HAT_UP:
      if (paddle < MATRIX_HEIGHT-2)
        ++paddle;
      break;

    case BUTTON_HAT_DOWN:
      if (paddle > 1)
        --paddle;
      break;
  }

  for (y=0; y<MATRIX_HEIGHT; ++y)
  {
    if (y >= paddle-1 && y <= paddle+1)
      matrix_set_pixel(0, y, matrix_color_green);
    else
      matrix_clear_pixel(0, y);
  }
}

static void setup_player(void)
{
  paddle = 4;

  move_paddle();
}

static void wait_for_start(void)
{
  uint8_t last_tick = 0;
  uint8_t blink = 0;
  uint8_t i;

  for (i=0; i<ball_count; ++i)
  {
    matrix_set_pixel(3, i*2+2, matrix_color_yellow);
  }

  while (1)
  {
    if (last_tick != tick)
    {
      move_paddle();

      blink = 1-blink;

      if (blink)
        matrix_set_pixel(3, (ball_count-1)*2+2, matrix_color_yellow);
      else
        matrix_set_pixel(3, (ball_count-1)*2+2, matrix_color_red);

      last_tick = tick;
    }

    if (button_get_aux_state())
      break;
  }

  for (i=0; i<ball_count; ++i)
  {
    if (i < ball_count-1)
      matrix_clear_pixel(3, i*2+2);
    else
      matrix_set_pixel(3, i*2+2, matrix_color_yellow);
  }

  ball_x  = 3;
  ball_y  = (ball_count-1)*2+2;
  ball_dx = -1;
  ball_dy = 0;
}

static uint8_t move_ball(void)
{
  uint8_t cleared_brick=0;
  uint8_t next_x, next_y;
  uint8_t reversed_x = 0;
  uint8_t reversed_y = 0;

  if (ball_x == 1 && ball_dx == -1)  // check for possible last moment paddle contact
  {
  }

  matrix_clear_pixel(ball_x, ball_y);

  ball_x += ball_dx;
  ball_y += ball_dy;

  matrix_set_pixel(ball_x, ball_y, matrix_color_red);

  next_x = ball_x + ball_dx;
  next_y = ball_y + ball_dy;

  if (next_x>=MATRIX_WIDTH)
  {
    reversed_x = 1;
    ball_dx = -1;
    next_x = ball_x + ball_dx;
  }

  if (next_y>=MATRIX_HEIGHT)
  {
    reversed_y = 1;
    ball_dy *= -1;
    next_y = ball_y + ball_dy;
  }

  if (ball_x == 1)  // check for paddle bounce
  {
    if (ball_y == paddle)
    {
      ball_dx = 1;
      ball_dy = 0;
    }
    else if (ball_y == paddle-1)
    {
      ball_dx = 1;
      ball_dy = -1;
    }
    else if (ball_y == paddle+1)
    {
      ball_dx = 1;
      ball_dy = 1;
    }
  }
  else              // check for brick bounce
  {
    if (!matrix_is_pixel_blank(next_x, ball_y))
    {
      matrix_clear_pixel(next_x, ball_y);
      cleared_brick = 1;
      if (!reversed_x)
        ball_dx *= -1;
    }
    else if (!matrix_is_pixel_blank(next_x, next_y))
    {
      matrix_clear_pixel(next_x, next_y);
      cleared_brick = 1;
      if (!reversed_y)
      {
        ball_dy *= -1;

        if (!reversed_x)
          ball_dx *= -1;
      }
    }
  }

  return cleared_brick;
}

static void animate_failure(void)
{
  uint8_t last_tick = tick;
  uint8_t blink = 0;
  uint8_t count = 8;

  while (count > 0)
  {
    if (last_tick != tick)
    {
      --count;
      blink = 1 - blink;

      if (blink)
        matrix_set_pixel(ball_x, ball_y, matrix_color_red);
      else
        matrix_clear_pixel(ball_x, ball_y);

      last_tick = tick;
    }
  }
}

static void play(void)
{
  uint8_t last_tick = tick;

  while (brick_count > 0)
  {
    if (last_tick != tick)
    {
      move_paddle();

      brick_count -= move_ball();

      if (ball_x==0)
      {
        --ball_count;

        animate_failure();

        break;
      }

      last_tick = tick;
    }
  }
}

static void endgame(void)
{
  uint8_t x, y;
  const matrix_color_t *color;

  if (brick_count)
    color = &matrix_color_red;
  else
  {
    color = &matrix_color_green;

    setup_board();
    move_paddle();
    matrix_set_pixel(ball_x, ball_y, *color);
  }

  for (x=0; x<MATRIX_WIDTH; ++x)
  {
    for (y=0; y<MATRIX_HEIGHT; ++y)
    {
      if (!matrix_is_pixel_blank(x, y))
        matrix_set_pixel(x, y, *color);
    }
  }

  while (1)
  {
    if (button_get_aux_state())
      break;
  }
}

void breakout_run(void)
{
  uint8_t timer = timer_set_callback(15, 1, &tick_callback);

  matrix_clear_all();

  matrix_start_scanning();

  ball_count = 3;

  setup_board();

  while (ball_count > 0 && brick_count > 0)
  {
    setup_player();

    wait_for_start();

    play();
  }

  endgame();

  matrix_stop_scanning();

  timer_clear_callback(timer);
}

