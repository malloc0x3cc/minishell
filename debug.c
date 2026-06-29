/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/29 07:29:02 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/29 07:29:13 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* remove this file later */

#include "minishell.h"

void	debug_tokens(t_token *t)
{
	int	i;

	i = 0;
	while (t)
	{
		printf("=== %d ===\nSTR: %s\nLEN: %ld\nTYPE: %d\nNEXT: %p\n", i++, t->str, ft_strlen(t->str), t->type, t->next);
		t = t->next;
	}
}

void	debug_cmds(t_cmd *cmd)
{
	int	i;
	int	c;

	c = 0;
	while (cmd)
	{
		printf("--- COMMAND %d ---\n", c++);
		i = 0;
		while (cmd->args && cmd->args[i])
		{
			printf("  args[%d]: [%s]\n", i, cmd->args[i]);
			i++;
		}
		cmd = cmd->next;
	}
}
