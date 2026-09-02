/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 18:31:51 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/02 07:54:09 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** get_path_value
** ============================================================================
** Parcourt le tableau d'environnement env a la recherche de la variable
** "PATH=" et renvoie un pointeur vers la partie qui suit le signe "="
** (c'est-a-dire directement la liste des repertoires separes par ':').
**
** Si aucune variable PATH n'est presente dans l'environnement, renvoie
** NULL. C'est un cas volontairement gere en amont dans find_path(),
** car sans PATH on ne peut tenter aucune resolution de chemin relatif
** (l'utilisateur devra alors taper un chemin absolu/relatif lui-meme).
**
** Cette fonction ne fait qu'une seule chose : localiser et extraire la
** valeur de PATH. Elle ne s'occupe ni du split ni de la recherche du
** binaire, ce qui est delegue respectivement a ft_split() et a
** search_in_paths().
** ============================================================================
*/
static char	*get_path_value(char **env)
{
	int	i;

	i = 0;
	while (env && env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env || !env[i])
		return (NULL);
	return (env[i] + 5);
}

/*
** ============================================================================
** build_full_path
** ============================================================================
** Construit le chemin complet candidat "dir/cmd" a partir d'un repertoire
** issu de PATH et du nom de la commande recherchee.
**
** La concatenation se fait en 2 temps (dir + "/" puis + cmd) car
** ft_strjoin() ne prend que 2 arguments : on libere le resultat
** intermediaire tmp des qu'il n'est plus utile, pour ne pas fuir de
** memoire a chaque iteration de la boucle dans search_in_paths().
**
** Renvoie une chaine allouee que l'appelant doit liberer (soit apres
** l'avoir testee avec access(), soit apres l'avoir renvoyee a son propre
** appelant si le test reussit).
** ============================================================================
*/
static char	*build_full_path(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

/*
** ============================================================================
** search_in_paths
** ============================================================================
** Parcourt le tableau paths (deja splitte sur ':') et teste, pour chaque
** repertoire, si "dir/cmd" existe et est executable via access(X_OK).
**
** Des qu'un chemin valide est trouve, le tableau paths est libere avec
** ft_free_tab() (on n'en a plus besoin) et le chemin complet est renvoye
** a l'appelant, qui devient responsable de sa liberation.
**
** Si aucun repertoire ne contient d'executable correspondant, paths est
** libere et NULL est renvoye : find_path() saura alors qu'il s'agit d'une
** commande introuvable.
** ============================================================================
*/
static char	*search_in_paths(char **paths, char *cmd)
{
	int		i;
	char	*full;

	i = 0;
	while (paths && paths[i])
	{
		full = build_full_path(paths[i], cmd);
		if (full && access(full, F_OK) == 0)
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
** ============================================================================
** find_path
** ============================================================================
** Cherche le chemin complet et executable correspondant a la commande cmd,
** en s'appuyant sur la variable PATH de l'environnement env.
**
** Etapes :
**   1. get_path_value()  : recupere la valeur de PATH (liste de repertoires
**                          separes par ':'), ou NULL si PATH est absent.
**   2. ft_split()         : decoupe cette valeur en un tableau de chemins
**                          individuels.
**   3. search_in_paths()  : teste chaque repertoire et renvoie le premier
**                          chemin "dir/cmd" qui soit executable.
**
** Cette fonction est volontairement reduite a un role d'orchestrateur,
** pour respecter la norme 42 : chaque etape (extraction de PATH,
** construction d'un chemin candidat, parcours des chemins) est isolee
** dans sa propre fonction, ce qui rend chacune d'elles courte et testable
** independamment.
**
** Renvoie le chemin complet alloue (a liberer par l'appelant) si trouve,
** NULL sinon (PATH absent, ft_split() echoue, ou commande introuvable
** dans aucun repertoire de PATH).
** ============================================================================
*/
char	*find_path(char *cmd, char **env)
{
	char	*path_value;
	char	**paths;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_value = get_path_value(env);
	if (!path_value)
		return (NULL);
	paths = ft_split(path_value, ':');
	if (!paths)
		return (NULL);
	return (search_in_paths(paths, cmd));
}
