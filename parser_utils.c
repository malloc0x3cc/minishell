/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 19:35:18 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/02 19:35:30 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** syntax_error
** ============================================================================
** Affiche le message d'erreur de syntaxe au format standard bash sur stderr.
** ============================================================================
*/
static int	syntax_error(char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putstr_fd(token, 2);
	ft_putendl_fd("'", 2);
	return (2);
}

/*
** ============================================================================
** check_syntax
** ============================================================================
** Parcourt et valide l'enchainement des tokens selon les regles de bash :
**   - Pas de pipe en tete de ligne.
**   - Pas de pipes consecutifs.
**   - Toute redirection doit etre suivie d'un mot valide.
** Retourne 0 si valide, 2 en cas d'erreur de syntaxe.
** ============================================================================
*/
int	check_syntax(t_token *t)
{
	if (!t)
		return (0);
	if (t->type == TOKEN_PIPE)
		return (syntax_error("|"));
	while (t)
	{
		if (t->type == TOKEN_PIPE)
		{
			if (!t->next || t->next->type == TOKEN_PIPE)
				return (syntax_error("|"));
		}
		else if (t->type != TOKEN_WORD)
		{
			if (!t->next)
				return (syntax_error("newline"));
			if (t->next->type != TOKEN_WORD)
				return (syntax_error(t->next->str));
		}
		t = t->next;
	}
	return (0);
}
