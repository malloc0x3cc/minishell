/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 11:07:22 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/10 14:00:06 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* debug, remove later */
static void	print_tokens(t_token *t)
{
	int	i;

	i = 0;
	while (t->next)
	{
		printf("=== %d ===\nSTR: %s\nTYPE: %s\nNEXT: %s\n", i++, t->str, t->type, t->next);
		t = t->next;
	}
}

t_token	*lexer(char *input)
{
	t_token	*t;
	size_t	i;
	size_t	wordlen;

	t = NULL;
	i = 0;
	while (*input)
	{
		wordlen = 0;
		// skip spaces
		while (ft_isspace(*input))
			input++;
		if (!*input)
			break ;
		// special tokens
		if (*input == '|' || *input == '<' || *input == '>')
		{
			if (*input == '|')
				t->type = TOKEN_PIPE;
			else if (*input == '<')
				t->type = TOKEN_IN;
			else if (*input == '>')
				t->type = TOKEN_OUT;
			input++;
			i++;
			continue ;
		}
		// text
		else
		{
			while (*input && !ft_isspace(input)
				&& *input != '|' && *input != '<' && *input != '>')
			{
				wordlen++;
				input++;
			}
			t[i].str = ft_substr(input - wordlen, 0, wordlen);
			t[i].type = TOKEN_WORD;
			input += wordlen;
			i++;
		}
	}
	t[i].next = NULL;
	return (t);
}
