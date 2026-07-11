/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghub <ghub@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:01:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/11 14:25:34 by ghub             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>   
# include <sys/wait.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
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

typedef struct s_exec
{
	int	pipe_fd[2];
	int	in_fd;
	int	*hd_fds;
	int	idx;
}	t_exec;

typedef struct s_fds
{
	int	in_fd;
	int	out_fd;
	int	hd_fd;
}	t_fds;

/*t_token	*lexer(char *s);
void	free_tokens(t_token *t);
void	free_cmds(t_cmd *cmd);
t_token	*create_token(char *str, t_token_type type);
void	add_token(t_token **head, t_token *new);
t_cmd	*parser(t_token *t);
*/
int		execute(t_cmd *cmd, char **env);
int		wait_children(pid_t pid, int *status);
int		count_cmds(t_cmd *cmd);
void	child_exec(t_cmd *cmd, t_fds *fds, char **env);
int		handle_heredocs(t_cmd *cmd, int *hd_fds);
char	**clean_args(char **args);
char	*find_path(char *cmd, char **env);
int		apply_redirs(char **args);

/* DEBUG, REMOVE LATER 
void	debug_tokens(t_token *t);
void	debug_cmds(t_cmd *cmd);
*/
#endif