/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 15:48:58 by ghub              #+#    #+#             */
/*   Updated: 2026/09/02 08:03:36 by madelwau         ###   ########.fr       */
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
** Attend la fin de TOUS les processus enfants lances (utile pour les
** pipelines a plusieurs commandes, ou chaque commande est un enfant).
** waitpid(-1, ...) attend n'importe quel enfant, un par un, jusqu'a ce
** qu'il n'y en ait plus (retour -1, typiquement ECHILD).
**
** BUG CORRIGE : l'ancienne version gardait le *status du DERNIER enfant
** REAPE, c'est-a-dire celui qui termine en dernier dans le temps reel.
** Or l'ordre de fin des processus ne correspond pas forcement a l'ordre
** des commandes dans le pipeline (ex: "sleep 1 | false" : "false" finit
** quasi instantanement, "sleep" finit 1 seconde plus tard, alors que le
** code de sortie du PIPELINE doit etre celui de "false", la derniere
** commande, comme le fait bash).
**
** On boucle toujours sur TOUS les enfants pour n'en laisser aucun en
** zombie, mais on ne memorise le status que lorsque le pid reape
** correspond exactement a last_pid (le pid de la derniere commande du
** pipeline, transmis par run_pipeline()).
**
** Retourne le code de sortie de last_pid : converti via WEXITSTATUS() si
** le processus s'est termine normalement, ou 128 + numero de signal s'il
** a ete tue par un signal (convention shell standard), 1 par defaut.
** ============================================================================
*/
int	wait_children(pid_t last_pid, int *status)
{
	pid_t	pid;
	int		tmp_status;

	*status = 0;
	while (1)
	{
		pid = waitpid(-1, &tmp_status, 0);
		if (pid == -1)
			break ;
		if (pid == last_pid)
			*status = tmp_status;
	}
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	if (WIFSIGNALED(*status))
	{
		if (WTERMSIG(*status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		return (128 + WTERMSIG(*status));
	}
	return (1);
}
