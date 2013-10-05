/**
 * Matrix interface for VHS badge.
 *
 * Author: Bruce Fletcher
 * License: http://creativecommons.org/licenses/by/3.0/
 */

#define MATRIX_WIDTH 16
#define MATRIX_HEIGHT 8

typedef struct
{
  unsigned char red;
  unsigned char green;
} matrix_color_t;
  

void matrix_init(void);  // call on boot

void matrix_start_scanning(void);  // turn the display on
void matrix_stop_scanning(void);   // and off, essentially.

void matrix_timer_callback(void);  // alternate approach; poll it

void matrix_clear_all(void);

void matrix_clear_pixel(unsigned char x, unsigned char y);

void matrix_set_pixel(unsigned char x, unsigned char y, matrix_color_t color);

matrix_color_t matrix_get_pixel(unsigned char x, unsigned char y);

