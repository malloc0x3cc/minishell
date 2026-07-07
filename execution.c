/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:01:26 by gahubert          #+#    #+#             */
/*   Updated: 2026/07/07 09:59:51 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Cherche la commande dans PATH et retourne le chemin complet.
** Ex: "ls" -> "/bin/ls"
** Retourne NULL si non trouvé. A libérer après usage.
*/
static char	*find_path(char *cmd, char **env)
{
	char	**paths;
	char	*tmp;
	char	*full;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full, X_OK) == 0)
		{
			ft_free_tab(paths);
			return (full);
		}
		free(full);
		i++;
	}
	ft_free_tab(paths);
	return (NULL);
}

/*
** FIX: adapted to t_redir
*/
static int	apply_redirs(t_redir *redir)
{
	int	fd;

	while (redir)
	{
		if (redir->type == REDIR_OUT)
			fd = open(redir->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND)
			fd = open(redir->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == REDIR_IN)
			fd = open(redir->name, O_RDONLY);
		else
			fd = -1; // TODO: heredoc
		if (fd == -1)
			return (perror(redir->name), 1);
		if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (close(fd), perror("dup2"), 1);
		}
		else if (redir->type == REDIR_IN)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
				return (close(fd), perror("dup2"), 1);
		}
		close(fd);
		redir = redir->next;
	}
	return (0);
}

/*
** FIX: clean_args() not needed thanks to parsing
*/
static void	child_exec(t_cmd *cmd, int in_fd, int out_fd, char **env)
{
	char	*path;

	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			(perror("dup2"), exit(1));
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			(perror("dup2"), exit(1));
		close(out_fd);
	}
	if (apply_redirs(cmd->redirs) != 0)
		exit(1);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	path = find_path(cmd->args[0], env);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putchar_fd('\n', 2);
		exit(127);
	}
	execve(path, cmd->args, env);
	perror(path);
	free(path);
	exit(1);
}

/*
** Attend tous les enfants et retourne le code de sortie du dernier.
*/
static int	wait_children(pid_t last_pid, int *status)
{
	pid_t	pid;

	while (1)
	{
		pid = waitpid(-1, status, 0);
		if (pid == -1)
			break ;
	}
	(void)last_pid;
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	return (1);
}

/*
** Fonction principale d'exécution.
** Gère les pipes entre les t_cmd chainés via ->next.
** Chaque paire de cmds reliées partage un pipe.
*/
int	execute(t_cmd *cmd, char **env)
{
	int		pipe_fd[2];
	int		in_fd;
	pid_t	pid;
	int		status;

	in_fd = STDIN_FILENO;
	pid = 0;
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(pipe_fd) == -1)
				return (perror("pipe"), 1);
		}
		else
			pipe_fd[1] = STDOUT_FILENO;
		pid = fork();
		if (pid == -1)
			return (perror("fork"), 1);
		if (pid == 0)
		{
			if (cmd->next)
				close(pipe_fd[0]);
			child_exec(cmd, in_fd, pipe_fd[1], env);
		}
		if (cmd->next)
			close(pipe_fd[1]);
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		in_fd = (cmd->next) ? pipe_fd[0] : STDIN_FILENO;
		cmd = cmd->next;
	}
	return (wait_children(pid, &status));
}
