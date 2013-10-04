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
  

void matrix_clear_all(void);

void matrix_clear_pixel(unsigned char x, unsigned char y);

void matrix_set_pixel(unsigned char x, unsigned char y, matrix_color_t color);

