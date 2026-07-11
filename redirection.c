/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghub <ghub@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 15:52:25 by ghub              #+#    #+#             */
/*   Updated: 2026/07/07 22:00:49 by ghub             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** redir_out
** ============================================================================
** Gere la redirection ">" : ouvre le fichier cible en ecriture, l'ecrase
** s'il existe deja (O_TRUNC), le cree sinon (O_CREAT), puis redirige
** stdout dessus via dup2.
** Retourne 0 si OK, 1 en cas d'erreur (open ou dup2 echoue).
** ============================================================================
*/
static int	redir_out(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (perror(file), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), perror("dup2"), 1);
	close(fd);
	return (0);
}

/*
** ============================================================================
** redir_append
** ============================================================================
** Gere la redirection ">>" : ouvre le fichier cible en ecriture, ajoute
** a la suite s'il existe deja (O_APPEND), le cree sinon (O_CREAT), puis
** redirige stdout dessus via dup2.
** Retourne 0 si OK, 1 en cas d'erreur (open ou dup2 echoue).
** ============================================================================
*/
static int	redir_append(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (perror(file), 1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), perror("dup2"), 1);
	close(fd);
	return (0);
}

/*
** ============================================================================
** redir_in
** ============================================================================
** Gere la redirection "<" : ouvre le fichier cible en lecture seule, puis
** redirige stdin dessus via dup2.
** Retourne 0 si OK, 1 en cas d'erreur (open ou dup2 echoue).
** ============================================================================
*/
static int	redir_in(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return (perror(file), 1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), perror("dup2"), 1);
	close(fd);
	return (0);
}

/*
** ============================================================================
** redir_token
** ============================================================================
** Identifie le type de redirection au token args[i] et delegue au bon
** handler (redir_out / redir_append / redir_in).
** Retourne :
**   2  si une redirection a ete reconnue et appliquee avec succes
**      (nombre de tokens consommes : l'operateur + sa cible)
**   -1 si une redirection a ete reconnue mais a echoue (open/dup2)
**   0  si args[i] n'est pas un token de redirection
** ============================================================================
*/
static int	redir_token(char **args, int i)
{
	int	err;

	if (ft_strcmp(args[i], ">") == 0 && args[i + 1])
	{
		err = redir_out(args[i + 1]);
		if (err)
			return (-1);
		return (2);
	}
	if (ft_strcmp(args[i], ">>") == 0 && args[i + 1])
	{
		err = redir_append(args[i + 1]);
		if (err)
			return (-1);
		return (2);
	}
	if (ft_strcmp(args[i], "<") == 0 && args[i + 1])
	{
		err = redir_in(args[i + 1]) ? -1 : 2;
		return (err);
	}
	return (0);
}

/*
** ============================================================================
** apply_redirs
** ============================================================================
** Parcourt args et applique chaque redirection classique rencontree en
** deleguant a redir_token(). step vaut 2 si une redirection a ete
** consommee, 0 si args[i] n'en est pas une (auquel cas on avance de 1).
** Retourne 0 si tout s'est bien passe, 1 des qu'une redirection echoue.
** ============================================================================
*/
int	apply_redirs(char **args)
{
	int	i;
	int	step;

	i = 0;
	while (args[i])
	{
		step = redir_token(args, i);
		if (step == -1)
			return (1);
		if (step == 0)
			step = 1;
		i += step;
	}
	return (0);
}
