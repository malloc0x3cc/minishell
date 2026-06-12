/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:01:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/12 15:23:56 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/inc/libft.h"

# define PROMPT "minishell> "

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_OUTFILE,	/* > */
	TOKEN_INFILE,	/* < */
	TOKEN_APPEND,	/* >> */
	TOKEN_HEREDOC,	/* << */
	TOKEN_PIPE,		/* | */
}	t_token_type;

typedef struct s_cmd
{
	char			**args;
	t_token_type	type;
	struct s_token	*next;
}	t_cmd;

#endif
