/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gahubert <gahubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:01:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/11 13:06:54 by gahubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/inc/libft.h"
# include <sys/wait.h>

# define PROMPT "minishell> "

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

int	execute(char **args);
int	wait_children(pid_t pid, int *status);

#endif
