/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:00:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/23 10:40:35 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;

	while (1)
	{
		input = readline(PROMPT);
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			tokens = lexer(input);
			cmds = parser(tokens);
			debug_tokens(tokens);
			debug_cmds(cmds);
			free_tokens(tokens);
			free_cmds(cmds);
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}
