/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 11:09:03 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/19 12:11:15 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count_tokens(t_token *t)
{
	size_t	i;

	i = 0;
	while (t && t->type != TOKEN_PIPE)
	{
		if (t->type == TOKEN_WORD)
			i++;
		t = t->next;
	}
	return (i);
}

t_cmd	*parser(t_token *t)
{
	size_t	i;
	t_cmd	*cmd;

	i = 0;
	cmd = malloc(sizeof(t_cmd) * count_tokens(t));
	while (t->next)
	{
		if (t->type == TOKEN_PIPE)
			cmd = cmd->next;
		cmd->args[i++] = ft_strdup(t->str);
	}
	return (cmd);
}
