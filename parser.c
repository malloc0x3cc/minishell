/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 11:09:03 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/16 14:14:11 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** count_tokens
** ============================================================================
** Compte le nombre d'arguments simples (TOKEN_WORD) associes a une commande
** courante, en s'arretant au premier pipe croise (qui delimite la commande
** suivante).
**
** Ce pre-calcul permet au parser d'allouer exactement la bonne taille pour
** le tableau cmd->args, s'epargnant ainsi des reallocations successives.
** ============================================================================
*/
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

/*
** ============================================================================
** create_cmd
** ============================================================================
** Alloue une structure t_cmd pour representer une commande de notre pipeline
** et reserve l'espace memoire necessaire pour son tableau d'arguments (args).
**
** La taille du tableau est determinee par count_tokens(). On initialise
** egalement la liste des redirections associees (redirs) et le pointeur vers
** la commande suivante (next) a NULL.
**
** Retourne la commande allouee, ou NULL en cas d'echec de allocation.
** ============================================================================
*/
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

/*
** ============================================================================
** add_redir
** ============================================================================
** Alloue et ajoute un nouvel element de redirection (t_redir) a la fin de
** la liste des redirections de la commande en cours (head).
**
** Traduit le type de token du lexer vers le type interne associe :
**   - TOKEN_INFILE   -> REDIR_IN
**   - TOKEN_OUTFILE  -> REDIR_OUT
**   - TOKEN_APPEND   -> REDIR_APPEND
**   - TOKEN_HEREDOC  -> REDIR_HEREDOC
** Conserve une copie du nom du fichier ou du delimiteur grace a ft_strdup().
** ============================================================================
*/
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

/*
** ============================================================================
** handle_token
** ============================================================================
** Traite individuellement un token au sein de la boucle du parser :
**   1. TOKEN_PIPE : Termine la commande actuelle (en ajoutant la sentinelle NULL
**      a la fin du tableau d'arguments), cree la structure de la commande
**      suivante, et réinitialise l'index des arguments.
**   2. Redirection (<, >, <<, >>) : Ajoute une redirection a la liste de la
**      commande actuelle en recuperant le nom du fichier associe dans le
**      token suivant, puis avance le pointeur de lecture au-dela de ces deux
**      tokens.
**   3. TOKEN_WORD : Duplique le mot pour l'ajouter a la liste d'arguments de
**      la commande actuelle.
** ============================================================================
*/
static void	handle_token(t_token **t, t_cmd *current, size_t *i)
{
	if ((*t)->type == TOKEN_PIPE)
	{
		current->args[*i] = NULL;
		*t = (*t)->next;
		current->next = create_cmd(*t);
		*i = 0;
	}
	else if ((*t)->type != TOKEN_WORD)
	{
		if ((*t)->next)
		{
			add_redir(&current->redirs, (*t)->next->str, (*t)->type);
			*t = (*t)->next->next;
		}
		else
			*t = (*t)->next;
	}
	else
	{
		current->args[(*i)++] = ft_strdup((*t)->str);
		*t = (*t)->next;
	}
}

/*
** ============================================================================
** parser
** ============================================================================
** Transforme la liste plate de tokens issue du lexer/expanser en un arbre de
** syntaxe simplifie (pipeline de structures t_cmd).
**
** Chaque commande de la liste possedera son propre tableau d'arguments purges
** de leurs redirections, et sa propre liste chainee de redirections de fichiers.
**
** Retourne la tete de la liste chainee des commandes pretes a l'execution.
** ============================================================================
*/
t_cmd	*parser(t_token *t)
{
	t_cmd	*head;
	t_cmd	*current;
	size_t	i;

	if (!t)
		return (NULL);
	head = create_cmd(t);
	if (!head)
		return (NULL);
	current = head;
	i = 0;
	while (t)
	{
		if (t->type == TOKEN_PIPE)
		{
			handle_token(&t, current, &i);
			current = current->next;
		}
		else
			handle_token(&t, current, &i);
	}
	current->args[i] = NULL;
	return (head);
}
