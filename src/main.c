/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vyepremy <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 21:37:08 by vyepremy          #+#    #+#             */
/*   Updated: 2024/06/01 22:40:18 by vyepremy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <unistd.h>
#include <stdlib.h>

void	map(float num, float max, float old_max)
{
	return (-max + 2 * max * (num - old_max / 2) / old_max);
}

void	calc_next(complex_t *z, complex_t *c)
{
	complex_t	z_next;

	z_next.im = 2 * z.im * z.re + c.im;
	z_next.re = z.re * z.re - z.im * z.im + c.re;
	z.im = z_next.im;
	z.re = z_next.re;
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

int	on_key_event(window_t *fractol)
{
	return (1);
}

int	on_mouse_event(window_t *fractol)
{
	return (1);
}

int	render_mandelbrot(window_t *fractol)
{
	int			iter;
	complex_t	z;
	complex_t	c;

	x = -1;
	while (++x < HEIGHT)
	{
		y = -1;
		while (++y < WIDTH)
		{
			c = (complex_t) {.re = map(x, 2, WIDTH), .im = map(y, 2, HEIGHT)};
			z = (complex_t) {.im = 0, .re = 0};
			iter = -1;
			while (++iter < MAX_IT && z.im * z.im + z.re * z.re < DIVERGE_VALUE)
				calc_next(&z, &c);
			put_pixel();
		}
	}
	mlx_put_image_to_window(fractol->mlx, fractol->win, fractol->img, 0, 0);
	return (1);
}

int	render_julia(window_t *fractol)
{
	return (1);
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
		fractol.renderer = render_julia;
	fractol.renderer(&fractol);
	mlx_key_hook(fractol.win, on_key_event, &fractol);
	mlx_mouse_hook(fractol.win, on_mouse_event, &fractol);
	mlx_hook(fractol.win, 17, 0, close_app, &fractol);
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
