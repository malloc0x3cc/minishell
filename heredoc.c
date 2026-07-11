/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghub <ghub@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 14:43:36 by gahubert          #+#    #+#             */
/*   Updated: 2026/07/11 14:18:08 by ghub             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** read_heredoc
** ============================================================================
** Lit un heredoc "<< delim" DANS LE PROCESSUS PARENT, avant le fork.
**
** Pourquoi dans le parent et pas dans l'enfant ?
**   Si on lisait le heredoc dans child_exec() (donc apres le fork), et qu'il
**   y a plusieurs commandes pipees ayant chacune un heredoc
**   (ex: "cat << A | grep x << B"), on se retrouverait avec plusieurs
**   processus enfants qui essaient de lire au clavier EN MEME TEMPS :
**   comportement imprevisible et casse. En lisant tout en amont dans le
**   parent, un seul heredoc est lu a la fois, dans l'ordre, avant meme
**   qu'aucun fork n'ait lieu.
**
** Fonctionnement :
**   1. On cree un pipe : pipe_fd[0] = lecture, pipe_fd[1] = ecriture.
**   2. On boucle : on lit une ligne au clavier avec readline("> ").
**      - Si l'utilisateur tape Ctrl+D (EOF), readline retourne NULL -> stop.
**      - Si la ligne tapee est exactement egale au delimiteur -> stop.
**      - Sinon on ecrit la ligne + un saut de ligne dans pipe_fd[1].
**   3. On ferme le bout ecriture du pipe (plus besoin d'ecrire).
**   4. On retourne pipe_fd[0] : ce fd, une fois lu, redonnera tout le texte
**      tape par l'utilisateur, comme si c'etait un fichier.
**
** Retourne le fd de lecture du pipe, ou -1 en cas d'erreur (pipe() echoue).
** ============================================================================
*/
static int	read_heredoc(char *delim)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

/*
** ============================================================================
** read_cmd_heredocs
** ============================================================================
** Parcourt les args d'UNE SEULE commande a la recherche de tokens "<<",
** et lit chaque heredoc rencontre via read_heredoc().
**
** Si une commande a plusieurs "<<" (ex: "cat << A << B"), seul le DERNIER
** compte : on ferme le fd precedent des qu'un nouveau heredoc est lu, ce
** qui reproduit le comportement standard de bash. Le resultat final est
** stocke dans *hd_fd (initialise a -1 si la commande n'a aucun heredoc).
**
** Retourne 0 si tout s'est bien passe, 1 si read_heredoc() a echoue
** (pipe() a echoue quelque part).
** ============================================================================
*/
static int	read_cmd_heredocs(t_cmd *cmd, int *hd_fd)
{
	int	j;

	*hd_fd = -1;
	j = 0;
	while (cmd->args[j])
	{
		if (ft_strcmp(cmd->args[j], "<<") == 0 && cmd->args[j + 1])
		{
			if (*hd_fd != -1)
				close(*hd_fd);
			*hd_fd = read_heredoc(cmd->args[j + 1]);
			if (*hd_fd == -1)
				return (1);
			j += 2;
		}
		else
			j++;
	}
	return (0);
}

/*
** ============================================================================
** close_heredocs
** ============================================================================
** Ferme tous les fds de heredoc deja ouverts et stockes dans hd_fds[0..count[.
**
** Utilisee uniquement en cas d'echec dans handle_heredocs() : si la lecture
** du heredoc de la commande N echoue, les heredocs des commandes 0 a N-1
** ont deja ete lus et leurs fds sont ouverts. Sans ce nettoyage, ils
** resteraient ouverts pour toute la duree de vie du shell (fuite de fds).
** ============================================================================
*/
static void	close_heredocs(int *hd_fds, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (hd_fds[i] != -1)
			close(hd_fds[i]);
		i++;
	}
}

/*
** ============================================================================
** handle_heredocs
** ============================================================================
** Parcourt TOUTE la liste de commandes AVANT le fork, et pour chaque
** commande, delegue a read_cmd_heredocs() la lecture de son (dernier)
** heredoc eventuel, stocke dans hd_fds[i] (i = index de la commande dans
** la chaine). Une commande sans heredoc obtient hd_fds[i] == -1.
**
** En cas d'echec sur la commande i, close_heredocs() ferme tous les fds
** deja ouverts pour les commandes 0 a i-1, afin de ne rien laisser fuir,
** puis la fonction retourne 1 immediatement.
**
** Retourne 0 si tout s'est bien passe, 1 si une erreur est survenue.
** ============================================================================
*/
int	handle_heredocs(t_cmd *cmd, int *hd_fds)
{
	int	i;

	i = 0;
	while (cmd)
	{
		if (read_cmd_heredocs(cmd, &hd_fds[i]) != 0)
		{
			close_heredocs(hd_fds, i);
			return (1);
		}
		i++;
		cmd = cmd->next;
	}
	return (0);
}
