/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 11:07:22 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/15 17:24:46 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_redir(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static t_token_type	handle_redir(char *input)
{
	if (*input == '<')
		return (TOKEN_INFILE);
	else if (*input == '>')
		return (TOKEN_OUTFILE);
	return (TOKEN_PIPE);
}

static char	*handle_args(char **input)
{
	char	*start;
	size_t	word_len;

	start = *input;
	word_len = 0;
	while (**input && !ft_isspace(**input) && !is_redir(**input))
	{
		word_len++;
		(*input)++;
	}
	return (ft_substr(start, 0, word_len));
}

t_token	*lexer(char *input)
{
	t_token	*head;
	char	*word;

	head = NULL;
	while (*input)
	{
		while (*input && ft_isspace(*input))
			input++;
		if (!*input)
			break ;
		if (is_redir(*input))
		{
			add_token_back(&head, create_token(ft_substr(input, 0, 1), handle_redir(input)));
			input++;
		}
		else
		{
			word = handle_args(&input);
			add_token_back(&head, create_token(word, TOKEN_WORD));
		}
	}
	return (head);
}
