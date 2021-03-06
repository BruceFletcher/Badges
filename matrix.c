/**
 * Matrix interface for VHS badge.
 *
 * Author: Bruce Fletcher
 * License: http://creativecommons.org/licenses/by/3.0/
 */

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "matrix.h"


#define SCAN_PERIOD  100   // Adjust to minimize flicker, scanning light polution



const matrix_color_t matrix_color_blank  = { 0, 0 };
const matrix_color_t matrix_color_red    = { 255, 0 };
const matrix_color_t matrix_color_green  = { 0, 255 };
const matrix_color_t matrix_color_yellow = { 255, 255 };


matrix_color_t matrix_buffer[MATRIX_WIDTH][MATRIX_HEIGHT];


void matrix_init(void)
{
  TCCR2B |= (1 << CS22);   // /64 prescale on timer 2, used for scanning display

  DDRB |= (1 << DDB0)      // column select high bits
       |  (1 << DDB1)
       |  (1 << DDB2);
  DDRD |= (1 << DDD7)      // column select low bit
       |  (1 << DDD6)      // row data
       |  (1 << DDD5);     // row clock
}

ISR(TIMER2_OVF_vect)
{
  static uint8_t column;

  uint8_t row, scan_column;
  matrix_color_t *c;

  TCNT2 = 255 - SCAN_PERIOD + TCNT2;   // Reset timer scanning period

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
  TCNT2 = 255 - SCAN_PERIOD;   // Initial timer overflow value
  TIMSK2 |= (1 << TOIE2);      // Enable timer overflow interrupt
}

void matrix_stop_scanning(void)
{
  TIMSK2 &= ~(1 << TOIE2);  // Disable timer overflow interrupt
}

void matrix_clear_all(void)
{
  memset(&matrix_buffer, 0, sizeof(matrix_buffer));
}

void matrix_clear_pixel(uint8_t x, uint8_t y)
{
  if (x < MATRIX_WIDTH && y < MATRIX_HEIGHT)
    matrix_buffer[x][y] = matrix_color_blank;
}

void matrix_set_pixel(uint8_t x, uint8_t y, matrix_color_t color)
{
  if (x < MATRIX_WIDTH && y < MATRIX_HEIGHT)
    matrix_buffer[x][y] = color;
}

matrix_color_t matrix_get_pixel(uint8_t x, uint8_t y)
{
  if (x < MATRIX_WIDTH && y < MATRIX_HEIGHT)
    return matrix_buffer[x][y];

  return matrix_color_blank;
}

uint8_t matrix_is_pixel_blank(uint8_t x, uint8_t y)
{
  if (x < MATRIX_WIDTH && y < MATRIX_HEIGHT)
  {
    if (matrix_buffer[x][y].red == 0 && matrix_buffer[x][y].green == 0)
      return 1;
  }

  return 0;
}

