/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 11:07:22 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/23 19:57:48 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** is_redir
** ============================================================================
** Petite fonction utilitaire qui determine si un caractere correspond a un
** operateur de redirection de flux ('<', '>') ou a un operateur de
** pipeline ('|').
**
** Retourne 1 (vrai) si c'est un operateur, 0 (faux) sinon.
** ============================================================================
*/
static int	is_redir(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/*
** ============================================================================
** handle_redir
** ============================================================================
** Analyse le ou les caracteres d'operateurs au debut de la chaine input
** afin de determiner le type de token de redirection exact a generer.
**
** Distingue les redirections simples des redirections doubles :
**   - '<' suivi de '<' renvoie un TOKEN_HEREDOC, sinon un TOKEN_INFILE.
**   - '>' suivi de '>' renvoie un TOKEN_APPEND, sinon un TOKEN_OUTFILE.
**   - Dans tous les autres cas (notamment '|'), renvoie un TOKEN_PIPE.
** ============================================================================
*/
static t_token_type	handle_redir(char *input)
{
	if (*input == '<')
	{
		if (*(input + 1) && *(input + 1) == '<')
			return (TOKEN_HEREDOC);
		return (TOKEN_INFILE);
	}
	else if (*input == '>')
	{
		if (*(input + 1) && *(input + 1) == '>')
			return (TOKEN_APPEND);
		return (TOKEN_OUTFILE);
	}
	return (TOKEN_PIPE);
}

static int	check_redir_syntax(char *s)
{
	int		count;
	char	c;

	c = *s;
	count = 0;
	while (s[count] == c)
		count++;
	if (count > 2 || (c == '|' && count > 1))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
		ft_putchar_fd(c, 2);
		ft_putstr_fd("'\n", 2);
		return (-1);
	}
	return (count);
}

/*
** ============================================================================
** handle_args
** ============================================================================
** Extrait et isole un argument (TOKEN_WORD) de la chaine d'entree en gérant
** correctement les quotes simples et doubles.
**
** Tant qu'on est a l'interieur d'un quote ouvert (quote != '\0'), les espaces
** et les symboles de redirection perdent leur pouvoir de delimiteur et sont
** traites comme des caracteres ordinaires. Des que le quote est referme ou si
** aucun quote n'est ouvert, on s'arrete au premier espace ou operateur croise.
**
** Affiche un message d'erreur si un quote reste ouvert a la fin du mot.
** Retourne la sous-chaine allouee correspondant a l'argument isole.
** ============================================================================
*/
static char	*handle_args(char **input)
{
	char	*start;
	size_t	word_len;
	char	quote;

	start = *input;
	word_len = 0;
	quote = '\0';
	while (**input)
	{
		if ((**input == '\'' || **input == '"') && quote == '\0')
			quote = **input;
		else if (**input == quote)
			quote = '\0';
		if (quote == '\0' && (ft_isspace(**input) || is_redir(**input)))
			break ;
		word_len++;
		(*input)++;
	}
	if (quote != '\0')
		printf("Error: Open quotes\n");
	return (ft_substr(start, 0, word_len));
}

/*
** ============================================================================
** lexer
** ============================================================================
** Point d'entree de la phase de lexing. Analyse la chaine de caracteres tapee
** par l'utilisateur (s) et la decoupe en une liste chainee de tokens typés.
**
** Fonctionnement :
**   1. Ignore les espaces de separation (whitespaces).
**   2. Si on rencontre un operateur de redirection, on appelle handle_redir()
**      pour determiner son type precise, on cree le token (sur 1 ou 2 chars)
**      et on avance d'autant dans la chaine.
**   3. Sinon, on delegue a handle_args() l'extraction du mot (TOKEN_WORD),
**      qui gere la preservation des espaces a l'interieur des quotes.
**
** Retourne la tete de la liste chainee de tokens extraits.
** ============================================================================
*/
t_token	*lexer(char *s)
{
	t_token			*t;
	t_token_type	type;
	int				len;

	t = NULL;
	while (*s)
	{
		while (*s && ft_isspace(*s))
			s++;
		if (!*s)
			break ;
		if (is_redir(*s))
		{
			len = check_redir_syntax(s);
			if (len == -1)
				return (free_tokens(t), NULL);
			type = handle_redir(s);
			add_token(&t, create_token(ft_strndup(s, len), type));
			s += len;
		}
		else
			add_token(&t, create_token(handle_args(&s), TOKEN_WORD));
	}
	return (t);
}
