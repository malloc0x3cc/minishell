/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 16:36:21 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/19 11:55:36 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* remove later */
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

t_token	*create_token(char *str, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = str;
	new->type = type;
	new->next = NULL;
	return (new);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}
