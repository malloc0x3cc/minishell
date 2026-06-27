/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gahubert <gahubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:01:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/23 12:00:00 by gahubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>      /* fork, dup2, execve, pipe, close */
# include <fcntl.h>       /* open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC */
# include <sys/wait.h>    /* waitpid, WIFEXITED, WEXITSTATUS */
# include "libft/inc/libft.h"

# define PROMPT "sixsevenshell> "

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_OUTFILE,    /* >  */
	TOKEN_INFILE,     /* <  */
	TOKEN_APPEND,     /* >> */
	TOKEN_HEREDOC,    /* << */
	TOKEN_PIPE,       /* |  */
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
void	free_tokens(t_token *t);
void	free_cmds(t_cmd *cmd);
t_token	*create_token(char *str, t_token_type type);
void	add_token(t_token **head, t_token *new);

/* parser */
t_cmd	*parser(t_token *t);

/* execution */
int	execute(t_cmd *cmd, char **env);
int		wait_children(pid_t pid, int *status);

/* DEBUG, REMOVE LATER */
void	debug_tokens(t_token *t);
void	debug_cmds(t_cmd *cmd);

#endif