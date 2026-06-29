/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 11:09:03 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/29 08:11:12 by madelwau         ###   ########.fr       */
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

static t_cmd	*create_cmd(t_token *t)
{
	t_cmd	*cmd;
	size_t	size;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	size = count_tokens(t);
	cmd->args = malloc(sizeof(char *) * (size + 1));
	if (!cmd->args)
		return (free(cmd), NULL);
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

static void	add_redir(t_redir **head, char *name, t_token_type type)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->name = ft_strdup(name);
	if (type == TOKEN_INFILE)
		new->type = REDIR_IN;
	else if (type == TOKEN_OUTFILE)
		new->type = REDIR_OUT;
	else if (type == TOKEN_APPEND)
		new->type = REDIR_APPEND;
	else
		new->type = REDIR_HEREDOC;
	new->next = NULL;
	if (!*head)
		*head = new;
	else
	{
		tmp = *head;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_cmd	*parser(t_token *t)
{
	t_cmd	*head;
	t_cmd	*current;
	size_t	i;

	if (!t)
		return (NULL);
	head = create_cmd(t);
	current = head;
	i = 0;
	while (t)
	{
		if (t->type == TOKEN_PIPE)
		{
			current->args[i] = NULL;
			t = t->next;
			current->next = create_cmd(t);
			current = current->next;
			i = 0;
			continue ;
		}
		if (t->type != TOKEN_WORD)
		{
			if (t->next)
			{
				add_redir(&current->redirs, t->next->str, t->type);
				t = t->next->next;
			}
			else
				t = t->next;
			continue ;
		}
		current->args[i++] = ft_strdup(t->str);
		t = t->next;
	}
	current->args[i] = NULL;
	return (head);
}
