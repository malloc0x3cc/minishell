# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/05 21:13:28 by madelwau          #+#    #+#              #
#    Updated: 2026/05/06 13:30:22 by madelwau         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL			= /bin/sh

NAME			= libft.a

CC				= cc
CFLAGS			= -Wall -Wextra -Werror -Iinc -MMD -MP
AR				= ar
ARFLAGS			= rcs
OBJ_DIR			= .obj/

STR_SRCS	= ft_split.c ft_strchr.c ft_strcmp.c ft_strdup.c ft_striteri.c \
              ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c \
              ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c
MEM_SRCS	= ft_bzero.c ft_calloc.c ft_memchr.c ft_memcmp.c ft_memcpy.c \
              ft_memmove.c ft_memset.c
IS_SRCS		= ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c ft_islower.c \
              ft_isprint.c ft_isspace.c ft_isupper.c
LST_SRCS	= ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c \
              ft_lstiter.c ft_lstlast.c ft_lstmap.c ft_lstnew.c ft_lstsize.c
PUT_SRCS	= ft_putchar_fd.c ft_putchar.c ft_putendl_fd.c ft_putendl.c \
              ft_putnbr_fd.c ft_putnbr.c ft_putstr_fd.c ft_putstr.c
TO_SRCS		= ft_atof.c ft_atoi.c ft_itoa.c ft_tolower.c ft_toupper.c
PRINTF_SRCS	= ft_printf.c ft_printf_utils.c
GNL_SRCS	= get_next_line.c
MLX_SRCS	= put_pixel.c

ALL_SRCS	= $(STR_SRCS) $(MEM_SRCS) $(IS_SRCS) $(LST_SRCS) $(PUT_SRCS) \
				$(TO_SRCS) $(PRINTF_SRCS) $(GNL_SRCS) $(MLX_SRCS)

OBJS	= $(addprefix $(OBJ_DIR), $(ALL_SRCS:.c=.o))
DEPS	= $(OBJS:.o=.d)

VPATH	= src/str:src/mem:src/is:src/lst:src/put:src/to:src/printf:src/gnl:src/mlx

all: $(NAME)

$(NAME): $(OBJS)
	@$(AR) $(ARFLAGS) $(NAME) $(OBJS)
	@printf "$(COL_Y)[DONE] $(NAME)$(COL_0)\n"

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@ \
		&& printf "$(COL_G)[OK]$(COL_0) %-42s -> %s\n" "$<" "$@"

clean:
	@rm -rf $(OBJ_DIR);
	@printf "$(COL_R)[RM]$(COL_0) $(OBJ_DIR)\n"

fclean: clean
	@rm -f $(NAME)
	@ printf "$(COL_R)[RM]$(COL_0) $(COL_B)$(NAME)$(COL_0)\n"

re: fclean all

.PHONY: all clean fclean re
-include $(DEPS)

# Colors
COL_R   = \033[1;31m
COL_G   =\033[1;32m
COL_B   =\033[1;34m
COL_Y   =\033[1;33m
COL_0   =\033[0m
