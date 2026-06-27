/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gahubert <gahubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:01:26 by gahubert          #+#    #+#             */
/*   Updated: 2026/06/23 12:00:00 by gahubert         ###   ########.fr       */
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
			ft_free_tab(paths);  /* libft : free char**  */
			return (full);
		}
		free(full);
		i++;
	}
	ft_free_tab(paths);
	return (NULL);
}

/*
** Applique les redirections présentes dans args.
** Parcourt args, détecte ">", ">>", "<" et ouvre les fds correspondants.
** Retourne 0 si OK, 1 si erreur.
*/
static int	apply_redirs(char **args)
{
	int	i;
	int	fd;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 && args[i + 1])
		{
			fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (perror(args[i + 1]), 1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (close(fd), perror("dup2"), 1);
			close(fd);
			i += 2;
		}
		else if (ft_strcmp(args[i], ">>") == 0 && args[i + 1])
		{
			fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (perror(args[i + 1]), 1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (close(fd), perror("dup2"), 1);
			close(fd);
			i += 2;
		}
		else if (ft_strcmp(args[i], "<") == 0 && args[i + 1])
		{
			fd = open(args[i + 1], O_RDONLY);
			if (fd == -1)
				return (perror(args[i + 1]), 1);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (close(fd), perror("dup2"), 1);
			close(fd);
			i += 2;
		}
		else
			i++;
	}
	return (0);
}

/*
** Construit un args nettoyé sans les tokens de redirection ni leurs cibles.
** Ex: ["ls", ">", "out.txt", NULL] -> ["ls", NULL]
** Retourne NULL si malloc échoue.
*/
static char	**clean_args(char **args)
{
	char	**clean;
	int		i;
	int		j;
	int		count;

	count = 0;
	i = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0
				|| ft_strcmp(args[i], "<") == 0) && args[i + 1])
			i += 2;
		else
			(count++, i++);
	}
	clean = malloc(sizeof(char *) * (count + 1));
	if (!clean)
		return (NULL);
	i = 0;
	j = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0
				|| ft_strcmp(args[i], "<") == 0) && args[i + 1])
			i += 2;
		else
			clean[j++] = args[i++];
	}
	clean[j] = NULL;
	return (clean);
}

/*
** Processus enfant : applique les redirections, cherche la commande
** dans PATH et lance execve. Exit avec le bon code d'erreur.
*/
static void	child_exec(t_cmd *cmd, int in_fd, int out_fd, char **env)
{
	char	*path;
	char	**exec_args;

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
	if (apply_redirs(cmd->args) != 0)
		exit(1);
	exec_args = clean_args(cmd->args);
	if (!exec_args || !exec_args[0])
		exit(0);
	path = find_path(exec_args[0], env);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(exec_args[0], 2);
		ft_putchar_fd('\n', 2);
		exit(127);
	}
	execve(path, exec_args, env);
	perror(path);   /* execve a échoué */
	free(path);
	exit(1);
}

/*
** Attend tous les enfants et retourne le code de sortie du dernier.
*/
int	wait_children(pid_t last_pid, int *status)
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