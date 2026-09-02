/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:01:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/02 20:17:15 by madelwau         ###   ########.fr       */
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

# define PROMPT "sixseven$HELL> "

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_OUTFILE,	// >
	TOKEN_INFILE,	// <
	TOKEN_APPEND,	// >>
	TOKEN_HEREDOC,	// <<
	TOKEN_PIPE,		// |
}	t_token_type;

typedef struct s_token
{
	char			*str;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef enum e_redir_type
{
	REDIR_OUT,		// >
	REDIR_IN,		// <
	REDIR_APPEND,	// >>
	REDIR_HEREDOC,	// <<
}	t_redir_type;

typedef struct s_redir
{
	char			*name;
	t_redir_type	type;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
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

typedef struct s_sub_ctx
{
	size_t	j;
	int		s_q;
	int		d_q;
	char	**env;
	int		status;
}	t_sub_ctx;

/* lexer */
t_token	*lexer(char *s);
/* lexer_utils */
void	free_tokens(t_token *t);
void	free_cmds(t_cmd *cmd);
t_token	*create_token(char *str, t_token_type type);
void	add_token(t_token **head, t_token *new);
/* parser */
t_cmd	*parser(t_token *t);
int		check_syntax(t_token *t);
int		is_all_spaces(char *s);
/* expanser */
void	expanser(t_token *tokens, char **env, int last_status);
/* expanser_utils */
char	*get_env_value(char *var, char **env);
int		is_var_char(char c);
size_t	get_var_name_len(char *str);
void	insert_status_value(char *dest, size_t *j, int last_status);
size_t	add_var_len(char *str, size_t *i, char **env, int status);
/* signal */
void	init_signals(void);
void	set_signals_for_exec(void);
void	reset_signals_for_child(void);
void	set_signals_for_heredoc(void);
/* signal handlers */
void	handle_sigint_prompt(int sig);
void	handle_sigint_exec(int sig);
void	handle_sigint_heredoc(int sig);
/* env utils */
int		env_size(char **env);
void	free_env(char **env);
char	**dup_env(char **env);
int		set_env_val(char *key_value, char ***env);
int		unset_env_val(char *key, char ***env);
/* builtin dispatcher & execution */
int		is_parent_builtin(char *cmd);
int		is_builtin(char *cmd);
int		exec_builtin(t_cmd *cmd, char ***env);
int		exec_single_parent_builtin(t_cmd *cmd, char ***env, int hd_fd);
/* builtins */
int		builtin_echo(char **args);
int		builtin_pwd(void);
int		builtin_env(char **env);
int		builtin_cd(char **args, char ***env);
int		builtin_export(char **args, char ***env);
int		builtin_unset(char **args, char ***env);
int		builtin_exit(char **args);
/* execution */
int		execute(t_cmd *cmd, char ***env);
int		wait_children(pid_t pid);
int		count_cmds(t_cmd *cmd);
void	child_exec(t_cmd *cmd, t_fds *fds, char **env);
int		handle_heredocs(t_cmd *cmd, int *hd_fds);
char	*find_path(char *cmd, char **env);
int		apply_redirs(t_redir *redir);
int		setup_pipe(t_cmd *cmd, t_exec *ex);

/* Return code */
extern int	g_received_signal;

#endif
