#ifndef UTILS_H
# define UTILS_H

# include "mlx.h"
# include <stdlib.h>
# include <unistd.h>
# include <math.h>

# define SIZE			500
# define MAX_ITER		200
# define DIVERGE_VALUE	4.0
# define WHITE			0xFFFFFF
# define BLACK			0x000000
# define RED				0xFF0000
# define GREEN			0x00FF00
# define BLUE			0x0000FF

# ifdef __linux__

enum e_keys
{
	KEY_ESC = 65307,
	KEY_UP = 65362,
	KEY_DOWN = 65364,
	KEY_RIGHT = 65363,
	KEY_LEFT = 65361,
	KEY_ZERO = 48,
	KEY_ONE = 49,
	KEY_TWO = 50,
	KEY_THREE = 51,
	KEY_FOUR = 52,
	KEY_FIVE = 53,
	KEY_SIX = 54,
	KEY_Q = 113,
	KEY_W = 119,
	KEY_E = 101,
	KEY_R = 114,
	KEY_T = 116,
	KEY_Y = 121,
	KEY_A = 97,
	KEY_S = 115,
	KEY_D = 100,
	KEY_F = 102,
	KEY_G = 103,
	KEY_H = 104,
	KEY_L = 108,
	MOUSE_LEFT_CLK = 1,
	MOUSE_SCRL_CLK = 2,
	MOUSE_RIGHT_CLK = 3,
	MOUSE_SCRL_UP = 4,
	MOUSE_SCRL_DOWN = 5,
};

# else

enum e_keys
{
	KEY_ESC = 53,
	KEY_UP = 126,
	KEY_DOWN = 125,
	KEY_LEFT = 123,
	KEY_RIGHT = 124,
	KEY_ZERO = 29,
	KEY_ONE = 18,
	KEY_TWO = 19,
	KEY_THREE = 20,
	KEY_FOUR = 21,
	KEY_FIVE = 23,
	KEY_SIX = 22,
	KEY_Q = 12,
	KEY_W = 13,
	KEY_E = 14,
	KEY_R = 15,
	KEY_T = 17,
	KEY_Y = 16,
	KEY_A = 0,
	KEY_S = 1,
	KEY_D = 2,
	KEY_F = 3,
	KEY_G = 5,
	KEY_H = 4,
	KEY_L = 37,
	MOUSE_LEFT_CLK = 1,
	MOUSE_RIGHT_CLK = 2,
	MOUSE_SCRL_CLK = 3,
	MOUSE_SCRL_DOWN = 4,
	MOUSE_SCRL_UP = 5,
};
# endif

typedef enum set_type
{
	julia = '0',
	mandelbrot = '1',
	ship = '2'
}	t_set;

typedef struct complex
{
	float	re;
	float	im;
}	t_complex;

typedef struct window
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*img_buffer;
	void	(*renderer)(struct window *);
	t_set	type;
	int		c_x;
	int		c_y;
	float	zoom;
	float	shift_x;
	float	shift_y;
	int		img_num_rows;
	int		img_endian;
	int		pixel_bits;
	int		color;
}	t_window;


size_t	ft_strlen(const char *c);
float	map(float num, float min, float max, float old_max);
void	calc_next(t_complex *z, t_complex *c, int is_ship);
void	set_pixel_color(t_window *engine, int x, int y, int color);
void	put_pixel_to_img(int row, int col, int iter, t_window *fractol);
void	init_fractol_window(t_window *fractol);
void	create_fractol(t_set set);
void	error_exit(char *msg, t_window *fractol);
int		on_key_event(int key, t_window *fractol);
int		on_mouse_click_event(int key, int x, int y, t_window *fractol);
int		on_mouse_move_event(int x, int y, t_window *fractol);
int		close_app(t_window *fractol);
void	usage(void);
void	render_mandelbrot(t_window *fractol);
void	render_ship(t_window *fractol);
void	render_julia(t_window *fractol);

#endif
