TARGET = fractol
SOURCES = src/main.c
INCLUDES = includes/
OBJECTS = $(SOURCES:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra #-Werror
MLX = MiniLibX
MLX_FLAGS = -l mlx -framework OpenGL -framework Appkit

all: $(TARGET)

$(TARGET): $(OBJECTS) $(MLX)/libmlx.a
	$(CC) $(CFLAGS) -o $@ $^ $(MLX_FLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -I ./$(MLX) -I ./$(INCLUDES) -o $@ -c $<

$(MLX)/libmlx.a:
	@echo "- making MiniLibX"
	@make -sC $(MLX)
	@echo "- success"

clean:
	@make -sC $(MLX) clean
	@rm -f $(OBJECTS)

fclean: clean
	@rm -f $(TARGET)

re: fclean all

debug: fclean
	@echo "- making debug"
	@make -s $(TARGET) CFLAGS="$(CFLAGS) -g"
	@echo "- success (debug)"

.PHONY: all, clean, fclean, re, debug
