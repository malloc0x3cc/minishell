/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:01:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/15 21:16:20 by madelwau         ###   ########.fr       */
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
	TOKEN_OUTFILE,	/* > */
	TOKEN_INFILE,	/* < */
	TOKEN_APPEND,	/* >> */
	TOKEN_HEREDOC,	/* << */
	TOKEN_PIPE,		/* | */
}	t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	struct s_cmd	*next;
}	t_cmd;

/* lexer */
t_token	*lexer(char *s);
/* lexer_utils */
void	debug_tokens(t_token *t);
t_token	*create_token(char *str, t_token_type type);
void	add_token_back(t_token **head, t_token *new);

#endif
