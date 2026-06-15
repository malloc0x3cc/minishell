/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 11:07:22 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/15 16:21:18 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* debug, remove later */
static void	print_tokens(t_cmd *t)
{
	int	i;

	i = 0;
	while (t->next)
	{
		printf("=== %d ===\nSTR: %s\nTYPE: %s\nNEXT: %s\n", i++, t->str, t->type, t->next);
		t = t->next;
	}
}

t_cmd	*lexer(char *input)
{
	t_cmd	*t;
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
				t->type = TOKEN_INFILE;
			else if (*input == '>')
				t->type = TOKEN_OUTFILE;
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
			t[i].args = ft_substr(input - wordlen, 0, wordlen);
			t[i].type = TOKEN_WORD;
			input += wordlen;
			i++;
		}
	}
	t[i].next = NULL;
	return (t);
}
