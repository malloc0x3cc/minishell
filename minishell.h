/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:01:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/10 12:20:06 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/inc/libft.h"

# define PROMPT "sixsevenshell> "

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_IN,		/* < */
	TOKEN_OUT,		/* > */
	TOKEN_APPEND,	/* >> */
	TOKEN_HEREDOC,	/* << */
	TOKEN_PIPE,		/* | */
}	t_token_type;

typedef struct s_token
{
	char			**str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

/* lexer */
t_token	*lexer(char *input);

#endif
