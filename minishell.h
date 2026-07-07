/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:01:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/07 09:59:55 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>      // fork, dup2, execve, pipe, close
# include <fcntl.h>       // open, O_RDONLY, O_WRONLY, O_CREAT, O_TRUNC
# include <sys/wait.h>    // waitpid, WIFEXITED, WEXITSTATUS
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/inc/libft.h"

# define PROMPT "sixsevenshell> "

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_OUTFILE,	// >
	TOKEN_INFILE,	// <
	TOKEN_APPEND,	// >>
	TOKEN_HEREDOC,	// <<
	TOKEN_PIPE,		// |
}	t_token_type;

typedef enum e_redir_type
{
	REDIR_OUT,	// >
	REDIR_IN,	// <
	REDIR_APPEND,	// >>
	REDIR_HEREDOC,	// <<
}	t_redir_type;

typedef struct s_redir
{
	char			*name;
	t_redir_type	type;
	struct s_redir	*next;
}	t_redir;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
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
int		execute(t_cmd *cmd, char **env);

/* DEBUG, REMOVE LATER */
void	debug_tokens(t_token *t);
void	debug_cmds(t_cmd *cmd);

#endif
