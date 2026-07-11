# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ghub <ghub@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/04 12:58:29 by madelwau          #+#    #+#              #
#    Updated: 2026/07/11 14:29:49 by ghub             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL	:=	/bin/sh

NAME	:= $(strip minishell)

CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror -I. -MMD -MP

LFT_DIR	:= libft
LFT		:= $(LFT_DIR)/libft.a

LIBS	:= -L$(LFT_DIR) -lft -lreadline

SRCS	:= minishell.c args_utils.c execution.c execution_children.c execution_utils.c find_path.c heredoc.c redirection.c
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

.PHONY: all clean fclean re
-include $(DEPS)
