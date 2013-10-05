/**
 * Matrix interface for VHS badge.
 *
 * Author: Bruce Fletcher
 * License: http://creativecommons.org/licenses/by/3.0/
 */

#include <string.h>
#include <avr/io.h>
#include "timer.h"
#include "matrix.h"


matrix_color_t matrix_buffer[MATRIX_WIDTH][MATRIX_HEIGHT];

static unsigned char timer;


void matrix_init(void)
{
  DDRB |= (1 << DDB0)      // column select high bits
       |  (1 << DDB1)
       |  (1 << DDB2);
  DDRD |= (1 << DDD7)      // column select low bit
       |  (1 << DDD6)      // row data
       |  (1 << DDD5);     // row clock
}

void matrix_timer_callback(void)
{
  static unsigned char column;

  unsigned char row, scan_column;
  matrix_color_t *c;

  ++column;
  column %= MATRIX_WIDTH;

  c = &matrix_buffer[column][0];

  scan_column = MATRIX_WIDTH - 1 - column;
  PORTD = (PORTD & 0b01111111) | ((scan_column & 1) << PORTD7);
  PORTB = (PORTB & 0b11111000) | (scan_column >> 1);

  for (row=0; row<MATRIX_HEIGHT; ++row)
  {
    if (c[row].red)
      PORTD |= (1<<PORTD6);
    else
      PORTD &= ~(1<<PORTD6);

    PORTD |= (1<<PORTD5);
    asm("nop");
    PORTD &= ~(1<<PORTD5);
    asm("nop");

    if (c[row].green)
      PORTD |= (1<<PORTD6);
    else
      PORTD &= ~(1<<PORTD6);

    PORTD |= (1<<PORTD5);
    asm("nop");
    PORTD &= ~(1<<PORTD5);
    asm("nop");
  }
}

void matrix_start_scanning(void)
{
  timer = timer_set_callback(1, 1, &matrix_timer_callback);
}

void matrix_stop_scanning(void)
{
  timer_clear_callback(timer);
}

void matrix_clear_all(void)
{
  memset(&matrix_buffer, 0, sizeof(matrix_buffer));
}

void matrix_clear_pixel(unsigned char x, unsigned char y)
{
  matrix_buffer[x][y].red   = 0;
  matrix_buffer[x][y].green = 0;
}

void matrix_set_pixel(unsigned char x, unsigned char y, matrix_color_t color)
{
  matrix_buffer[x][y] = color;
}

matrix_color_t matrix_get_pixel(unsigned char x, unsigned char y)
{
  return matrix_buffer[x][y];
}

