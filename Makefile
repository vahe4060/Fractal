TARGET = fractol
SOURCES = 	src/main.c \
			src/events.c \
			src/utils.c
INCLUDES = includes/utils.h
OBJECTS = $(SOURCES:.c=.o)
INCLUDES_DIR = includes/

CC = cc
CFLAGS = -Wall -Wextra -g
OS = $(shell uname | tr A-Z a-z)
MLX_DIR = mlx_$(OS)
ifeq ($(OS), linux)
	MLX_FLAGS = -lmlx -lm -lz -lXext -lX11 -L $(MLX_DIR)
else ifeq ($(OS), macos)
		MLX_FLAGS = -lmlx -lm -framework OpenGL -framework Appkit
else
	@echo "Unsupported platform $(OS), supported linux and macos only"
	$(eval $(exit 0))
endif


all: $(TARGET)

$(TARGET): $(OBJECTS) $(MLX_DIR)/libmlx.a
	@$(CC) $(CFLAGS) -o $@ $^ $(MLX_FLAGS)

%.o: %.c $(INCLUDES)
	@$(CC) $(CFLAGS) -I ./$(MLX_DIR) -I ./$(INCLUDES_DIR) -o $@ -c $<

$(MLX_DIR)/libmlx.a:
	@echo "making MiniLibX"
	@make -sC $(MLX_DIR)

clean:
	@make -sC $(MLX_DIR) clean
	@rm -f $(OBJECTS)

fclean: clean
	@rm -f $(TARGET)

re: fclean all

debug: fclean
	@echo "making debug"
	CFLAGS="$(CFLAGS) -g"
	@make -s

.PHONY: all, clean, fclean, re, debug
