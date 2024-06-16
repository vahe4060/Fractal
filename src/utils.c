/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vyepremy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 22:32:44 by vyepremy          #+#    #+#             */
/*   Updated: 2024/06/16 22:32:46 by vyepremy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

size_t	ft_strlen(const char *c)
{
	size_t	size;

	size = 0;
	while (c[size] != '\0')
		++size;
	return (size);
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
	else if (set == ship)
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
