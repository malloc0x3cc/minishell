/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gahubert <gahubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:01:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/18 15:20:52 by gahubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/inc/libft.h"
# include "sys/wait.h"

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

int	execute(char **args);
int	wait_children(pid_t pid, int *status);

#endif
