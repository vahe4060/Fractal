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

void	usage(void)
{
	char	*msg;

	msg = "Usage: ./fractol [0-2]\n\
	[0] - julia set\n\
	[1] - mondelbrod set\n\
	[2] - burning ship set\n";
	write(1, msg, ft_strlen(msg));
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
