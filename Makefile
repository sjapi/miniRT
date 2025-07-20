NAME    = miniRT
SRCS_DIR = src/
OBJS_DIR = objs/
INC_DIR  = includes/
MLX_DIR  = mlx
MLX_LIB  = $(MLX_DIR)/libmlx.a

GREEN = \033[1;32m
YELLOW = \033[1;33m
CYAN = \033[1;36m
RED = \033[1;31m
PURPLE = \033[1;35m
RESET = \033[0m

MAIN = \
	main.c optimization.c \
	free.c init.c

PARSER = \
	parser.c parser_utils.c parser_tools.c \
	parse_elements.c parse_objects.c \
	parse_model.c 
UTILS = \
	vector1.c ambient.c ft_atof.c ft_atoi.c \
	ft_memcpy.c color1.c clamp.c ft_calloc.c \
	ft_bzero.c ft_abs.c time.c utils.c \
	ft_strlen.c ft_strcmp.c vector2.c \
	color2.c

GNL = \
	get_next_line.c get_next_line_utils.c

RENDERER = \
	render.c img.c ray.c \
	hit.c plane.c sphere.c model.c \
	cylinder.c cone.c skybox.c \
	checkerboard.c info.c specular.c diffuse.c \
	compute_color.c


CONTROLS = \
	mouse.c keys.c obj.c \
	cam.c lights.c helper.c

SRCS = \
	$(addprefix $(SRCS_DIR)main/, $(MAIN)) \
	$(addprefix $(SRCS_DIR)get_next_line/, $(GNL)) \
	$(addprefix $(SRCS_DIR)parser/, $(PARSER)) \
	$(addprefix $(SRCS_DIR)renderer/, $(RENDERER)) \
	$(addprefix $(SRCS_DIR)utils/, $(UTILS)) \
	$(addprefix $(SRCS_DIR)controls/, $(CONTROLS))

OBJS = $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))

CC      = cc
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
	LGFLAGS = -Lmlx -lmlx -framework OpenGL -framework AppKit -lm 
	SANITIZE = # sanitize disabled on macOS (optional)
else
	LGFLAGS = -Lmlx -lmlx -lXext -lX11 -lm
	SANITIZE = -fsanitize=address
endif


all: $(MLX_LIB) $(NAME)

$(MLX_LIB):
	@echo "$(RED)⚙️ Building MLX...$(RESET)"
	@$(MAKE) -C $(MLX_DIR)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	@echo "$(RED)[Compiling]$(RESET) $<"
	@$(CC) $(CFLAGS) -I$(INC_DIR) -I$(MLX_DIR) -c $< -o $@

$(NAME): $(OBJS)
	@echo "$(YELLOW)🔧 Linking objects...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LGFLAGS)
	@echo "$(RED)✅ Build complete: ./$(NAME)$(RESET)"
	@sleep 0.07
	@echo "\033[38;5;226m███╗░░░███╗██╗███╗░░██╗██╗██████╗░████████╗\033[0m"
	@sleep 0.07
	@echo "\033[38;5;220m████╗░████║██║████╗░██║██║██╔══██╗╚══██╔══╝\033[0m"
	@sleep 0.07
	@echo "\033[38;5;214m██╔████╔██║██║██╔██╗██║██║██████╔╝░░░██║░░░\033[0m"
	@sleep 0.07
	@echo "\033[38;5;208m██║╚██╔╝██║██║██║╚████║██║██╔══██╗░░░██║░░░\033[0m"
	@sleep 0.07
	@echo "\033[38;5;202m██║░╚═╝░██║██║██║░╚███║██║██║░░██║░░░██║░░░\033[0m"
	@sleep 0.07
	@echo "\033[38;5;196m╚═╝░░░░░╚═╝╚═╝╚═╝░░╚══╝╚═╝╚═╝░░╚═╝░░░╚═╝░░░\033[0m"
	@sleep 0.07
	@echo "\033[38;5;202m      🔥 MADE BY: Arseniy & Hakob 🔥\033[0m"

clean:
	@$(MAKE) -C $(MLX_DIR) clean
	@rm -rf $(OBJS_DIR)
	@echo "$(RED)🧹 Objects cleaned!$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)🔥 $(NAME) removed!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
