/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 11:07:22 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/19 11:54:47 by madelwau         ###   ########.fr       */
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

t_token	*lexer(char *s)
{
	t_token			*t;
	t_token_type	type;

	t = NULL;
	while (*s)
	{
		while (*s && ft_isspace(*s))
			s++;
		if (!*s)
			break ;
		if (is_redir(*s))
		{
			type = handle_redir(s);
			if (type == TOKEN_APPEND || type == TOKEN_HEREDOC)
			{
				add_token(&t, create_token(ft_strndup(s, 2), type));
				s += 2;
			}
			else
				(add_token(&t, create_token(ft_strndup(s, 1), type)), s++);
		}
		else
			add_token(&t, create_token(handle_args(&s), TOKEN_WORD));
	}
	return (t);
}
