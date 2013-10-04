/**
 * Matrix interface for VHS badge.
 *
 * Author: Bruce Fletcher
 * License: http://creativecommons.org/licenses/by/3.0/
 */

#include <avr/io.h>
#include "matrix.h"

void matrix_clear_all(void)
{
  // todo: bulk-clear the matrix display
}

void matrix_clear_pixel(unsigned char x, unsigned char y)
{
  // todo: clear a pixel
}

void matrix_set_pixel(unsigned char x, unsigned char y, matrix_color_t color)
{
  // todo: set a pixel
}

