/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vyepremy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 21:37:08 by vyepremy          #+#    #+#             */
/*   Updated: 2024/06/05 00:00:38 by vyepremy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <unistd.h>
#include <stdlib.h>

float	map(float num, float min, float max, float old_min, float old_max)
{
    return (max - min) * (num - old_min) / (old_max - old_min) + min;
}

void	calc_next(complex_t *z, complex_t *c)
{
	complex_t	z_next;

	z_next.im = 2 * z->im * z->re + c->im;
	z_next.re = z->re * z->re - z->im * z->im + c->re;
	z->im = z_next.im;
	z->re = z_next.re;
}

void    usage()
{
	char	*msg;

	msg = "Usage: ./fractol [0-1]\n\
	[0] - julia set\n\
	[1] - mondelbrod set\n";
    write(1, msg, ft_strlen(msg));
}

int	close_app(window_t *fractol)
{
	if (fractol->img)
		mlx_destroy_image(fractol->mlx, fractol->img);
	if (fractol->win)
		mlx_destroy_window(fractol->mlx, fractol->win);
	if (fractol->mlx)
		free(fractol->mlx);
	return (EXIT_FAILURE);
}

void	mlx_put_pixel_to_img(int row, int col, int iter, window_t *fractol)
{
	int	idx;
	int	color;

	idx = row * fractol->img_num_rows + col * 4;
	color = map(iter, RED, WHITE, 0, MAX_ITER);
	if (fractol->img_endian == 1)
	{
		fractol->img_buffer[idx] = color >> 24;
		fractol->img_buffer[idx + 1] = (color >> 16) & 0xFF;
		fractol->img_buffer[idx + 2] = (color >> 8) & 0xFF;
		fractol->img_buffer[idx + 3] = color && 0xFF;
		return ;
	}
	fractol->img_buffer[idx + 3] = color >> 24;
	fractol->img_buffer[idx + 2] = (color >> 16) & 0xFF;
	fractol->img_buffer[idx + 1] = (color >> 8) & 0xFF;
	fractol->img_buffer[idx] = color && 0xFF;
}

int	on_key_event(window_t *fractol)
{
	return (1);
}

int	on_mouse_event(window_t *fractol)
{
	return (1);
}

void	render_mandelbrot(window_t *fractol)
{
	int			it;
	int			i;
	int			j;
	complex_t	z;
	complex_t	c;

	i = -1;
	while (++i < HEIGHT)
	{
		j = -1;
		while (++j < WIDTH)
		{
			z = (complex_t) {.im = 0.0, .re = 0.0};
			c.im = map(i, -2, 2, 0, HEIGHT);
			c.re = map(j, -2, 2, 0, WIDTH);
			it = -1;
			while (++it < MAX_ITER && z.im * z.im + z.re * z.re < DIVERGE_VALUE)
				calc_next(&z, &c);
			mlx_put_pixel_to_img(i, j, it, fractol);
		}
	}
	mlx_put_image_to_window(fractol->mlx, fractol->win, fractol->img, 0, 0);
}

int	create_fractol(set_t set)
{
    window_t	fractol;

    fractol.mlx = mlx_init();
	if (!fractol.mlx)
		return (close_app(&fractol));
    fractol.win = mlx_new_window(fractol.mlx, WIDTH, HEIGHT, "fract-ol");
	if (!fractol.win)
		return (close_app(&fractol));
	fractol.img = mlx_new_image(fractol.mlx, WIDTH, HEIGHT);
	if (!fractol.img)
		return (close_app(&fractol));
	fractol.renderer = render_mandelbrot;
	if (set == julia)
		fractol.renderer = render_mandelbrot;
	fractol.img_buffer = mlx_get_data_addr(fractol.img, &(fractol.pixel_bits),
			&(fractol.img_num_rows), &(fractol.img_endian));
	if (!fractol.img_buffer)
		return (close_app(&fractol));
	mlx_key_hook(fractol.win, on_key_event, &fractol);
	mlx_mouse_hook(fractol.win, on_mouse_event, &fractol);
	mlx_hook(fractol.win, 17, 0, close_app, &fractol);
	fractol.renderer(&fractol);
    mlx_loop(fractol.mlx);
	return (EXIT_SUCCESS);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		usage();
	else if (ft_strlen(argv[1]) != 1)
		usage();
	else if(argv[1][0] != '0' && argv[1][0] != '1')
		usage();
	else
		create_fractol(argv[1][0]);
	return (0);
}
