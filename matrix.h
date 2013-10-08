/**
 * Matrix interface for VHS badge.
 *
 * Author: Bruce Fletcher
 * License: http://creativecommons.org/licenses/by/3.0/
 */


#include <stdint.h>

#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 8

typedef struct
{
  uint8_t red;
  uint8_t green;
} matrix_color_t;

extern const matrix_color_t matrix_color_red;
extern const matrix_color_t matrix_color_green;
extern const matrix_color_t matrix_color_yellow;

void matrix_init(void);  // call on boot

void matrix_start_scanning(void);  // turn the display on
void matrix_stop_scanning(void);   // and off, essentially.

void matrix_timer_callback(void);  // alternate approach; poll it

void matrix_clear_all(void);

void matrix_clear_pixel(uint8_t x, uint8_t y);

void matrix_set_pixel(uint8_t x, uint8_t y, matrix_color_t color);

matrix_color_t matrix_get_pixel(uint8_t x, uint8_t y);

uint8_t matrix_is_pixel_blank(uint8_t x, uint8_t y);

