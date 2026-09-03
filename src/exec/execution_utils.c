/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 15:48:58 by ghub              #+#    #+#             */
/*   Updated: 2026/09/02 20:23:18 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** count_cmds
** ============================================================================
** Compte le nombre de commandes chainees dans la liste (via ->next).
** Sert a savoir quelle taille donner au tableau hd_fds dans execute().
** ============================================================================
*/
int	count_cmds(t_cmd *cmd)
{
	int	n;

	n = 0;
	while (cmd)
	{
		n++;
		cmd = cmd->next;
	}
	return (n);
}

/*
** ============================================================================
** wait_children
** ============================================================================
** Attend tous les enfants et extrait le vrai code de sortie (0-255)
** du dernier processus execute grace aux macros POSIX WEXITSTATUS / WTERMSIG.
** ============================================================================
*/
int	wait_children(pid_t last_pid)
{
	pid_t	pid;
	int		status;
	int		last_status;

	last_status = 0;
	while (1)
	{
		pid = waitpid(-1, &status, 0);
		if (pid == -1)
			break ;
		if (pid == last_pid)
			last_status = status;
	}
	if (WIFEXITED(last_status))
		return (WEXITSTATUS(last_status));
	if (WIFSIGNALED(last_status))
	{
		if (WTERMSIG(last_status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		else if (WTERMSIG(last_status) == SIGINT)
			ft_putchar_fd('\n', STDERR_FILENO);
		return (128 + WTERMSIG(last_status));
	}
	return (1);
}

/*
** ============================================================================
** setup_pipe
** ============================================================================
** Prepare le pipe pour la commande courante :
**   - S'il y a une commande suivante (cmd->next), on cree un vrai pipe.
**   - Sinon (derniere commande de la chaine), la sortie doit aller vers
**     le stdout du shell : on force pipe_fd[1] a STDOUT_FILENO.
** Retourne 0 si OK, 1 si pipe() a echoue.
** ============================================================================
*/
int	setup_pipe(t_cmd *cmd, t_exec *ex)
{
	if (cmd->next)
	{
		if (pipe(ex->pipe_fd) == -1)
			return (perror("pipe"), 1);
	}
	else
		ex->pipe_fd[1] = STDOUT_FILENO;
	return (0);
}

int	exec_empty_cmd_redirs(t_redir *redirs)
{
	int	saved_in;
	int	saved_out;
	int	status;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
		return (1);
	status = apply_redirs(redirs);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (status);
}
