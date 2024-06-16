/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vyepremy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 22:32:25 by vyepremy          #+#    #+#             */
/*   Updated: 2024/06/16 22:32:26 by vyepremy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

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

void	error_exit(char *msg, t_window *fractol)
{
	write(1, "Error: ", 7);
	write(1, msg, ft_strlen(msg));
	close_app(fractol);
}
