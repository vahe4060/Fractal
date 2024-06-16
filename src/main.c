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
#include <math.h>

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

void	usage(void)
{
	char	*msg;

	msg = "Usage: ./fractol [0-1]\n\
	[0] - julia set\n\
	[1] - mondelbrod set\n";
	write(1, msg, ft_strlen(msg));
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

int	close_app(t_window *fractol)
{
	if (fractol->img)
		mlx_destroy_image(fractol->mlx, fractol->img);
	if (fractol->win)
		mlx_destroy_window(fractol->mlx, fractol->win);
	exit(0);
}

void	error_exit(char *msg, t_window *fractol)
{
	write(1, "Error: ", 7);
	write(1, msg, ft_strlen(msg));
	close_app(fractol);
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

int	on_key_event(int key, t_window *fractol)
{
	if (key == KEY_ESC)
		close_app(fractol);
	else if (key == KEY_LEFT)
		fractol->shift_x -= 0.2;
	else if (key == KEY_RIGHT)
		fractol->shift_x += 0.2;
	else if (key == KEY_UP)
		fractol->shift_y -= 0.2;
	else if (key == KEY_DOWN)
		fractol->shift_y += 0.2;
	else if (key == KEY_Q)
		fractol->color += 0x000200;
	else if (key == KEY_W)
		fractol->color += 0x000030;
	else if (key == KEY_E)
		fractol->color += 0x001000;
	fractol->renderer(fractol);
	mlx_do_sync(fractol->mlx);
	return (EXIT_SUCCESS);
}

int	on_mouse_click_event(int key, int x, int y, t_window *fractol)
{
	if (key == MOUSE_SCRL_UP)
	{
		fractol->shift_x += (x / fractol->zoom - x / (fractol->zoom / 1.25));
		fractol->shift_y += (y / fractol->zoom - y / (fractol->zoom / 1.25));
		fractol->zoom /= 1.25;
	}
	else if (key == MOUSE_SCRL_DOWN)
	{
		fractol->shift_x += (x / fractol->zoom - x / (fractol->zoom * 1.25));
		fractol->shift_y += (y / fractol->zoom - y / (fractol->zoom * 1.25));
		fractol->zoom *= 1.25;
	}
	fractol->renderer(fractol);
	return (0);
}

int	on_mouse_move_event(int x, int y, t_window *fractol)
{
	if (fractol->type != julia)
		return (0);
	fractol->c_x = x;
	fractol->c_y = y;
	fractol->renderer(fractol);
	return (0);
}

void	render_mandelbrot(t_window *fractol)
{
	int			it;
	int			i;
	int			j;
	t_complex	z;
	t_complex	c;

	i = -1;
	mlx_clear_window(fractol->mlx, fractol->win);
	while (++i < SIZE)
	{
		j = -1;
		c.re = (i / fractol->zoom) + fractol->shift_x;
		while (++j < SIZE)
		{
			z = (t_complex){.im = 0.0, .re = 0.0};
			c.im = (j / fractol->zoom) + fractol->shift_y;
			it = -1;
			while (z.im * z.im + z.re * z.re < DIVERGE_VALUE && ++it < MAX_ITER)
				calc_next(&z, &c, 0);
			set_pixel_color(fractol, i, j, (it * fractol->color));
		}
	}
	mlx_put_image_to_window(fractol->mlx, fractol->win, fractol->img, 0, 0);
}

void	render_ship(t_window *fractol)
{
	int			it;
	int			i;
	int			j;
	t_complex	z;
	t_complex	c;

	i = -1;
	mlx_clear_window(fractol->mlx, fractol->win);
	while (++i < SIZE)
	{
		j = -1;
		c.re = (i / fractol->zoom) + fractol->shift_x;
		while (++j < SIZE)
		{
			z = (t_complex){.im = 0.0, .re = 0.0};
			c.im = (j / fractol->zoom) + fractol->shift_y;
			it = -1;
			while (z.im * z.im + z.re * z.re < DIVERGE_VALUE && ++it < MAX_ITER)
				calc_next(&z, &c, 1);
			set_pixel_color(fractol, i, j, (it * fractol->color));
		}
	}
	mlx_put_image_to_window(fractol->mlx, fractol->win, fractol->img, 0, 0);
}

void	render_julia(t_window *fractol)
{
	int			it;
	int			i;
	int			j;
	t_complex	z;
	t_complex	c;

	mlx_clear_window(fractol->mlx, fractol->win);
	i = -1;
	while (++i < SIZE)
	{
		c.re = (fractol->c_x / fractol->zoom) + fractol->shift_x;
		j = -1;
		while (++j < SIZE)
		{
			c.im = (fractol->c_y / fractol->zoom) + fractol->shift_y;
			z.re = i / fractol->zoom + fractol->shift_x;
			z.im = j / fractol->zoom + fractol->shift_y;
			it = -1;
			while (z.im * z.im + z.re * z.re < DIVERGE_VALUE && ++it < MAX_ITER)
				calc_next(&z, &c, 0);
			set_pixel_color(fractol, i, j, (it * fractol->color));
		}
	}
	mlx_put_image_to_window(fractol->mlx, fractol->win, fractol->img, 0, 0);
}

void	init_fractol_window(t_window *fractol)
{
	fractol->mlx = mlx_init();
	if (!fractol->mlx)
		error_exit("mlx init failed\n", fractol);
	fractol->win = mlx_new_window(fractol->mlx, SIZE, SIZE, "fract-ol");
	if (!fractol->win)
		error_exit("can't create mlx window\n", fractol);
	fractol->img = mlx_new_image(fractol->mlx, SIZE, SIZE);
	if (fractol->img)
		fractol->img_buffer = mlx_get_data_addr(fractol->img,
				&(fractol->pixel_bits),
				&(fractol->img_num_rows),
				&(fractol->img_endian));
	if (!fractol->img || !fractol->img_buffer)
		error_exit("can't create mlx image\n", fractol);
	fractol->zoom = SIZE / 4;
	fractol->shift_x = -2.0;
	fractol->shift_y = -2.0;
	fractol->color = RED;
}

void	create_fractol(t_set set)
{
	t_window	fractol;

	init_fractol_window(&fractol);
	fractol.renderer = render_mandelbrot;
	fractol.type = mandelbrot;
	if (set == julia)
	{
		fractol.renderer = render_julia;
		fractol.type = julia;
	}
	if (set == ship)
	{
		fractol.renderer = render_ship;
		fractol.type = ship;
	}
	mlx_key_hook(fractol.win, on_key_event, &fractol);
	mlx_mouse_hook(fractol.win, on_mouse_click_event, &fractol);
	mlx_hook(fractol.win, 6, 1L << 6, on_mouse_move_event, &fractol);
	mlx_hook(fractol.win, 17, 0, close_app, &fractol);
	fractol.renderer(&fractol);
	mlx_loop(fractol.mlx);
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		usage();
	else if (ft_strlen(argv[1]) != 1)
		usage();
	else if (argv[1][0] != '0' && argv[1][0] != '1' && argv[1][0] != '2')
		usage();
	else
		create_fractol(argv[1][0]);
	return (0);
}
