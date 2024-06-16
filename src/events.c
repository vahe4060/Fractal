/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vyepremy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 22:32:15 by vyepremy          #+#    #+#             */
/*   Updated: 2024/06/16 22:32:17 by vyepremy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

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

int	close_app(t_window *fractol)
{
	if (fractol->img)
		mlx_destroy_image(fractol->mlx, fractol->img);
	if (fractol->win)
		mlx_destroy_window(fractol->mlx, fractol->win);
	exit(0);
}
