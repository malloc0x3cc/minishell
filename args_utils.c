/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghub <ghub@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 14:07:35 by ghub              #+#    #+#             */
/*   Updated: 2026/07/11 14:07:41 by ghub             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** is_redir_token
** ============================================================================
** Renvoie 1 si la chaine str est un token de redirection reconnu
** (">", ">>", "<" ou "<<"), 0 sinon.
**
** Cette fonction factorise la condition a rallonge repetee 2 fois dans
** l'ancienne version de clean_args() (une fois par passe). Elle ne
** verifie que le token lui-meme : c'est a l'appelant de verifier en plus
** qu'un argument suivant existe (args[i + 1]) avant de sauter les 2
** elements, car un token de redirection sans cible n'est pas cense
** etre saute ici.
** ============================================================================
*/
static int	is_redir_token(char *str)
{
	return (ft_strcmp(str, ">") == 0 || ft_strcmp(str, ">>") == 0
		|| ft_strcmp(str, "<") == 0 || ft_strcmp(str, "<<") == 0);
}

/*
** ============================================================================
** count_clean_args
** ============================================================================
** Premiere passe sur args : compte le nombre d'elements "utiles" qui
** resteront une fois les tokens de redirection et leurs cibles retires
** (ex: "<<", "EOF", ">", "out.txt" ne comptent pour rien, seuls "cat" et
** "-n" sont comptes dans l'exemple du commentaire de clean_args).
**
** Ce compte sert uniquement a determiner la taille exacte du tableau a
** allouer dans clean_args(), afin d'eviter tout gaspillage ou
** depassement memoire.
** ============================================================================
*/
static int	count_clean_args(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args[i])
	{
		if (is_redir_token(args[i]) && args[i + 1])
			i += 2;
		else
			(count++, i++);
	}
	return (count);
}

/*
** ============================================================================
** fill_clean_args
** ============================================================================
** Deuxieme passe sur args : remplit le tableau clean (deja alloue a la
** bonne taille par clean_args()) en ne recopiant que les pointeurs des
** elements utiles, dans le meme ordre que dans args.
**
** Les tokens de redirection et leurs cibles sont sautes exactement comme
** dans count_clean_args(), pour garantir que les deux passes restent
** coherentes entre elles (meme nombre d'elements retenus).
**
** Termine le tableau clean par un NULL final, requis par execve().
** ============================================================================
*/
static void	fill_clean_args(char **args, char **clean)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if (is_redir_token(args[i]) && args[i + 1])
			i += 2;
		else
			clean[j++] = args[i++];
	}
	clean[j] = NULL;
}

/*
** ============================================================================
** clean_args
** ============================================================================
** Construit une copie de args debarrassee de tous les tokens de
** redirection ET de leurs cibles, pour obtenir le tableau final a passer
** a execve().
** Ex: ["cat", "<<", "EOF", "-n", ">", "out.txt", NULL]
**     -> ["cat", "-n", NULL]
**
** Fonctionnement en deux passes, chacune deleguee a sa propre fonction
** pour respecter la norme 42 (fonctions courtes, une seule
** responsabilite) :
**   1. count_clean_args() : compte combien d'elements "utiles" il reste
**      (hors tokens de redirection et leurs arguments).
**   2. On alloue un tableau de la bonne taille.
**   3. fill_clean_args() : recopie uniquement les elements utiles dans
**      ce tableau, termine par NULL.
**
** Retourne NULL si l'allocation echoue.
** ============================================================================
*/
char	**clean_args(char **args)
{
	char	**clean;
	int		count;

	count = count_clean_args(args);
	clean = malloc(sizeof(char *) * (count + 1));
	if (!clean)
		return (NULL);
	fill_clean_args(args, clean);
	return (clean);
}