/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/15 16:36:21 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/10 00:08:07 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** free_tokens
** ============================================================================
** Libere proprement et recursivement toute la liste chainee de tokens
** generee par le lexer.
**
** Pour chaque maillon, on prend soin de liberer d'abord la chaine de
** caracteres allouee (t->str) avant de liberer la structure t elle-meme,
** garantissant l'absence de fuite memoire (memory leaks) lors du nettoyage
** de la phase de lexing.
** ============================================================================
*/
void	free_tokens(t_token *t)
{
	t_token	*tmp;

	while (t)
	{
		tmp = t->next;
		free(t->str);
		free(t);
		t = tmp;
	}
}

/*
** ============================================================================
** free_cmds
** ============================================================================
** Nettoie et libere l'integralite de la structure de commandes (t_cmd)
** ainsi que toutes ses dependances dynamiques :
**   1. Le tableau d'arguments (cmd->args), en liberant chaque string puis
**      le tableau de pointeurs lui-meme.
**   2. La liste chainee des redirections (cmd->redirs), en liberant le nom
**      de chaque fichier (redir->name) et le maillon.
**   3. La structure de commande principale.
**
** Cette fonction est cruciale pour nettoyer l'arbre de commande a chaque
** fin de boucle du shell.
** ============================================================================
*/
void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp_cmd;
	t_redir	*tmp_redir;
	int		i;

	while (cmd)
	{
		tmp_cmd = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
				free(cmd->args[i++]);
			free(cmd->args);
		}
		while (cmd->redirs)
		{
			tmp_redir = cmd->redirs->next;
			free(cmd->redirs->name);
			free(cmd->redirs);
			cmd->redirs = tmp_redir;
		}
		free(cmd);
		cmd = tmp_cmd;
	}
}

/*
** ============================================================================
** create_token
** ============================================================================
** Alloue et initialise un nouveau maillon de type t_token.
**
** Associe la chaine str recue (qui correspond au lexeme extrait) et son
** type de token associe (TOKEN_WORD, TOKEN_PIPE, etc.) au nouveau maillon,
** puis initialise le pointeur ->next a NULL.
**
** Retourne le pointeur vers le token alloue, ou NULL en cas d'echec de
** malloc().
** ============================================================================
*/
t_token	*create_token(char *str, t_token_type type)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	if (!new)
		return (free(str), NULL);
	new->str = str;
	new->type = type;
	new->next = NULL;
	return (new);
}

/*
** ============================================================================
** add_token
** ============================================================================
** Ajoute un token fraichement cree a la fin de la liste chainee de tokens.
**
** Si la liste est vide (*head == NULL), le nouveau token en devient la
** racine. Sinon, on parcourt la liste jusqu'au dernier element pour y
** greffer le nouveau maillon.
** ============================================================================
*/
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

int	check_redir_syntax(char *s)
{
	int		count;
	char	c;

	c = *s;
	count = 0;
	while (s[count] == c)
		count++;
	if (count > 2 || (c == '|' && count > 1))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		ft_putchar_fd(c, 2);
		ft_putendl_fd("'", 2);
		return (-1);
	}
	return (count);
}
