/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:00:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/15 19:20:45 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*input;
	t_token	*tokens;

	while (1)
	{
		input = readline(PROMPT);
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			tokens = lexer(input);
			debug_tokens(tokens);
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}
