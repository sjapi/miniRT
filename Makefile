NAME    = miniRT
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -g3 -fsanitize=address

LGFLAGS = -lreadline -Lmlx -lmlx -lXext -lX11 -lm
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
	main.c

PARSER = \
	parser.c parser_utils.c

UTILS_STRING = \
	ft_strlen.c ft_strcmp.c


GNL = \
	get_next_line.c get_next_line_utils.c


SRCS = \
	$(addprefix $(SRCS_DIR)main/, $(MAIN)) \
	$(addprefix $(SRCS_DIR)get_next_line/, $(GNL)) \
	$(addprefix $(SRCS_DIR)parser/, $(PARSER)) \
	$(addprefix $(SRCS_DIR)utils/string/, $(UTILS_STRING))

OBJS = $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))

all: $(MLX_LIB) $(NAME)

$(MLX_LIB):
	@echo "$(CYAN)⚙️ Building MLX...$(RESET)"
	@$(MAKE) -C $(MLX_DIR)

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	@echo "$(CYAN)[Compiling]$(RESET) $<"
	@$(CC) $(CFLAGS) -I$(INC_DIR) -I$(MLX_DIR) -c $< -o $@

$(NAME): $(OBJS)
	@echo "$(YELLOW)🔧 Linking objects...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LGFLAGS)
	@echo "$(CYAN)✅ Build complete: ./$(NAME)$(RESET)"
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
