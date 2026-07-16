/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:00:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/16 11:59:49 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_status = 0;

static char	*clean_str(char *str)
{
	char	*clean;
	char	quote;
	int		i;
	int		j;

	clean = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!clean)
		return (NULL);
	quote = '\0';
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && quote == '\0')
			quote = str[i];
		else if (str[i] == quote)
			quote = '\0';
		else
			clean[j++] = str[i];
		i++;
	}
	clean[j] = '\0';
	return (clean);
}

static void	remove_quotes(t_token *tokens)
{
	t_token	*tmp;
	char	*old_str;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD)
		{
			old_str = tmp->str;
			tmp->str = clean_str(old_str);
			free(old_str);
		}
		tmp = tmp->next;
	}
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	t_token	*tokens;
	t_cmd	*cmds;

	((void) ac, (void) av);
	while (1)
	{
		input = readline(PROMPT);
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			tokens = lexer(input);
			expanser(tokens, env);
			remove_quotes(tokens);
			cmds = parser(tokens);
			if (cmds)
				g_status = execute(cmds, env);
			free_tokens(tokens);
			free_cmds(cmds);
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}
