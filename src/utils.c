#include "utils.h"

size_t	ft_strlen(const char *c)
{
	size_t	size;

	size = 0;
	while (c[size] != '\0')
		++size;
	return (size);
}

float	map(float num, float min, float max, float old_max)
{
	int	old_min;

	old_min = 0;
	return ((max - min) * (num - old_min) / (old_max - old_min) + min);
}

void	calc_next(t_complex *z, t_complex *c, int is_ship)
{
	t_complex	z_next;

	z_next.im = 2 * z->im * z->re + c->im;
	z_next.re = z->re * z->re - z->im * z->im + c->re;
	if (is_ship)
	{
		z->im = fabs(z_next.im);
		z->re = fabs(z_next.re);
	}
	else
	{
		z->im = z_next.im;
		z->re = z_next.re;
	}
}

void	set_pixel_color(t_window *engine, int x, int y, int color)
{
	int	line_len;
	int	pixel_bits;
	int	offset;

	if (x < 0 || x >= SIZE || y < 0 || y >= SIZE)
		return ;
	line_len = engine->img_num_rows;
	pixel_bits = engine->pixel_bits;
	offset = (y * line_len) + ((pixel_bits / 8) * x);
	*(unsigned int *)(engine->img_buffer + offset) = color;
}

void	put_pixel_to_img(int row, int col, int iter, t_window *fractol)
{
	int	idx;
	int	color;

	idx = row * fractol->img_num_rows + col * 4;
	color = map(iter, BLACK, fractol->color, MAX_ITER);
	if (fractol->img_endian == 1)
	{
		fractol->img_buffer[idx] = color >> 24;
		fractol->img_buffer[idx + 1] = (color >> 16) & 0xFF;
		fractol->img_buffer[idx + 2] = (color >> 8) & 0xFF;
		fractol->img_buffer[idx + 3] = color & 0xFF;
		return ;
	}
	fractol->img_buffer[idx + 3] = color >> 24;
	fractol->img_buffer[idx + 2] = (color >> 16) & 0xFF;
	fractol->img_buffer[idx + 1] = (color >> 8) & 0xFF;
	fractol->img_buffer[idx] = color & 0xFF;
}
