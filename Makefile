TARGET = fractol
SOURCES = src/main.c
INCLUDES = includes/utils.h
OBJECTS = $(SOURCES:.c=.o)
INCLUDES_DIR = includes/

CC = gcc
CFLAGS = -Wall -Wextra -Werror
MLX = mlx
MLX_FLAGS = -lmlx -lm -framework OpenGL -framework Appkit

all: $(TARGET)

$(TARGET): $(OBJECTS) $(MLX)/libmlx.a
	$(CC) $(CFLAGS) -o $@ $^ $(MLX_FLAGS)

%.o: %.c $(INCLUDES)
	$(CC) $(CFLAGS) -I ./$(MLX) -I ./$(INCLUDES_DIR) -o $@ -c $<

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
