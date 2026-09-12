/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 19:35:18 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/12 22:01:46 by madelwau         ###   ########.fr       */
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

int	is_all_spaces(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isspace(s[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
** ============================================================================
** clean_str
** ============================================================================
** Alloue une nouvelle chaine et y recopie 'str' en retirant les quotes qui
** ont servi a definir les arguments (quote removal), conformement au
** comportement classique de Bash.
**
** Seuls les quotes exterieurs et effectifs sont supprimes :
** Un simple quote dans des doubles quotes (ex: "hello 'world'") ou inversement
** est preserve car il ne sert pas de delimiteur a ce niveau.
**
** Retourne la chaine "nettoyee" sans les quotes, ou NULL en cas d'echec.
** ============================================================================
*/
char	*clean_str(char *str)
{
	char	*clean;
	char	quote;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	clean = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!clean)
		return (NULL);
	quote = '\0';
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && quote == '\0')
			quote = str[i];
		else if (str[i] == quote)
			quote = '\0';
		else
			clean[j++] = str[i];
		i++;
	}
	clean[j] = '\0';
	return (clean);
}

/*
** ============================================================================
** remove_quotes
** ============================================================================
** Parcourt toute la liste chainee des tokens et applique l'etape de "Quote
** Removal" sur chaque TOKEN_WORD.
**
** Remplace la chaine originale du token par sa version nettoyee (sans les
** quotes protecteurs devenus inutiles apres l'expansion) via clean_str(),
** puis libere proprement l'ancienne chaine de caracteres.
** ============================================================================
*/
void	remove_quotes(t_token *tokens)
{
	t_token	*tmp;
	char	*old_str;
	char	*cleaned;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD && tmp->str)
		{
			cleaned = clean_str(tmp->str);
			if (cleaned)
			{
				old_str = tmp->str;
				tmp->str = cleaned;
				free(old_str);
			}
		}
		tmp = tmp->next;
	}
}
