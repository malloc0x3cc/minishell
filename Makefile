# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/04 12:58:29 by madelwau          #+#    #+#              #
#    Updated: 2026/09/02 19:37:58 by madelwau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL	:=	/bin/sh

NAME	:= $(strip minishell)

CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror -I. -MMD -MP

LFT_DIR	:= libft
LFT		:= $(LFT_DIR)/libft.a

LIBS	:= -L$(LFT_DIR) -lft -lreadline

SRCS	:= minishell.c find_path.c heredoc.c redirection.c \
			execution.c execution_children.c execution_utils.c \
			env_utils.c lexer.c lexer_utils.c parser.c parser_utils.c \
			expanser.c expanser_utils.c signal.c signal_handlers.c execution_parent.c \
			builtin_cd.c builtin_dispatcher.c builtin_echo_pwd_env.c \
			builtin_exit.c builtin_export_unset.c
MODE	:= mandatory

OBJ_DIR	:= .obj/
OBJS	:= $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))
DEPS	:= $(addprefix $(OBJ_DIR), $(SRCS:.c=.d))

all: $(NAME)

$(LFT):
	@$(MAKE) -C $(LFT_DIR)

$(NAME): $(LFT) $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	@printf '$(COL_Y)[DONE] %s ($(MODE))$(COL_0)\n' "$(NAME)"

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -I$(LFT_DIR) -I. -c $< -o $@
	@printf "$(COL_G)[OK]$(COL_0) %s -> %s\n" "$<" "$@"

clean:
	@rm -rf $(OBJ_DIR)
	@printf "$(COL_R)[RM]$(COL_0) %s/%s\n" "$(shell basename $(CURDIR))" "$(OBJ_DIR)"

fclean: clean
	@$(MAKE) fclean -C $(LFT_DIR)
	@rm -f $(NAME)
	@printf "$(COL_R)[RM]$(COL_0) $(COL_B)%s$(COL_0)\n" "$(NAME)"

re: fclean all

val: all
	@valgrind --suppressions=valgrindignore.supp --leak-check=full --show-leak-kinds=all ./$(NAME)

.PHONY: all clean fclean re val
-include $(DEPS)

# Colors
COL_R	:= \033[1;31m
COL_G	:= \033[1;32m
COL_B	:= \033[1;34m
COL_Y	:= \033[1;33m
COL_0	:= \033[0m
