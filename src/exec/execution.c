/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:01:26 by gahubert          #+#    #+#             */
/*   Updated: 2026/09/10 00:10:16 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** fork_and_exec
** ============================================================================
** Fork le processus pour la commande courante.
** Dans l'enfant : ferme le bout de lecture du pipe s'il existe, puis
** delegue tout le travail (redirections, heredoc, execve) a child_exec(),
** qui ne retourne jamais (exit direct).
** Dans le parent : retourne simplement le pid de l'enfant cree.
** Retourne -1 si fork() a echoue.
** ============================================================================
*/
static pid_t	fork_and_exec(
	t_cmd *cmd, char **env, t_exec *ex, t_token *tokens)
{
	pid_t	pid;
	t_fds	fds;

	pid = fork();
	if (pid == -1)
		return (perror("fork"), -1);
	if (pid == 0)
	{
		if (cmd->next)
			close(ex->pipe_fd[0]);
		fds.in_fd = ex->in_fd;
		fds.out_fd = ex->pipe_fd[1];
		fds.hd_fd = ex->hd_fds[ex->idx];
		fds.hd_fds_all = ex->hd_fds;
		fds.tokens = tokens;
		child_exec(cmd, &fds, env);
	}
	return (pid);
}

/*
** ============================================================================
** close_and_advance
** ============================================================================
** Cote parent uniquement : ferme les fds devenus inutiles pour cette
** iteration (bout ecriture du pipe, ancien in_fd, fd de heredoc), puis
** fait avancer l'etat pour la commande suivante :
**   - in_fd devient le bout lecture du pipe qu'on vient de creer
**     (ou STDIN_FILENO s'il n'y a pas de commande suivante).
**   - idx est incremente pour pointer sur le hd_fds de la prochaine
**     commande.
** ============================================================================
*/
static void	close_and_advance(t_cmd *cmd, t_exec *ex)
{
	if (cmd->next)
		close(ex->pipe_fd[1]);
	if (ex->in_fd != STDIN_FILENO)
		close(ex->in_fd);
	if (ex->hd_fds[ex->idx] != -1)
		close(ex->hd_fds[ex->idx]);
	ex->in_fd = STDIN_FILENO;
	if (cmd->next)
		ex->in_fd = ex->pipe_fd[0];
	ex->idx++;
}

/*
** ============================================================================
** run_pipeline
** ============================================================================
** Boucle sur toute la chaine de commandes : pour chacune, prepare le
** pipe, fork+exec, puis ferme/avance l'etat cote parent.
** Retourne le pid du DERNIER enfant cree (utilise ensuite par
** wait_children pour recuperer son code de sortie), ou -1 si une etape
** a echoue en cours de route.
** ============================================================================
*/
static pid_t	run_pipeline(
	t_cmd *cmd, char **env, t_exec *ex, t_token *tokens)
{
	pid_t	pid;

	pid = 0;
	while (cmd)
	{
		if (setup_pipe(cmd, ex) == 1)
			return (-1);
		pid = fork_and_exec(cmd, env, ex, tokens);
		if (pid == -1)
			return (-1);
		close_and_advance(cmd, ex);
		cmd = cmd->next;
	}
	return (pid);
}

/*
** ============================================================================
** run_and_wait
** ============================================================================
** Initialise l'etat d'execution, lance le pipeline et attend les enfants.
** ============================================================================
*/
static int	run_and_wait(t_cmd *cmd, char **env, t_exec *ex, t_token *tokens)
{
	pid_t	pid;
	int		status;

	set_signals_for_exec();
	ex->in_fd = STDIN_FILENO;
	ex->idx = 0;
	pid = run_pipeline(cmd, env, ex, tokens);
	free(ex->hd_fds);
	if (pid == -1)
		return (init_signals(), 1);
	status = wait_children(pid);
	init_signals();
	return (status);
}

/*
** ============================================================================
** execute
** ============================================================================
** Point d'entree principal de l'execution d'un pipeline.
**   1. Alloue le tableau hd_fds (un slot par commande) et pre-lit tous
**      les heredocs de la chaine AVANT tout fork (handle_heredocs).
**   2. Initialise l'etat partage t_exec (in_fd de depart = stdin, idx = 0).
**   3. Delegue toute la boucle de fork/pipe a run_pipeline().
**   4. Libere hd_fds, puis attend tous les enfants et retourne le code
**      de sortie du dernier via wait_children().
** ============================================================================
*/
int	execute(t_cmd *cmd, char ***env, t_token *tokens)
{
	t_exec	ex;
	int		hd_ret;
	int		status;

	ex.hd_fds = malloc(sizeof(int) * count_cmds(cmd));
	if (!ex.hd_fds)
		return (1);
	hd_ret = handle_heredocs(cmd, ex.hd_fds);
	if (hd_ret != 0)
	{
		free(ex.hd_fds);
		if (hd_ret == 130)
			return (130);
		return (1);
	}
	if (!cmd->next && (!cmd->args || !cmd->args[0] || !cmd->args[0][0]))
		return (free(ex.hd_fds), exec_empty_cmd_redirs(cmd->redirs));
	if (!cmd->next && cmd->args && *cmd->args && is_parent_builtin(*cmd->args))
	{
		status = exec_single_parent_builtin(cmd, env, ex.hd_fds[0]);
		free(ex.hd_fds);
		return (status);
	}
	return (run_and_wait(cmd, *env, &ex, tokens));
}
