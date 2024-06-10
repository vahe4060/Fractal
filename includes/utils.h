#ifndef UTILS_H
# define UTILS_H

#include "mlx.h"
#include <stdlib.h>

#define WIDTH			500
#define	HEIGHT			500
#define	MAX_ITER		200
#define DIVERGE_VALUE	4.0
#define WHITE			0xFFFFFF
#define BLACK			0x000000
#define RED				0xFF0000
#define GREEN			0x00FF00
#define BLUE			0x0000FF

typedef enum {
	julia = '0', 
	mandelbrot = '1'
} set_t;

typedef struct complex_number
{
	float	re;
	float	im;
} complex_t;

typedef struct	window {
	void	*mlx;
	void	*win;
	void	*img;
	char	*img_buffer;
	void	(*renderer)(void *);
	int		img_num_rows;
	int		img_endian;
	int		pixel_bits;
}	window_t;

size_t  ft_strlen(const char *c)
{
	size_t  size;

	size = 0;
	while (c[size] != '\0')
		++size;
	return (size);
}

#endif
