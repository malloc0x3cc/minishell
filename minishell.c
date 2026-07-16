/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:00:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/16 13:14:35 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_received_signal = 0;

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

static int	shell_loop(char *input, char **env, int last_status)
{
	t_token	*tokens;
	t_cmd	*cmds;

	add_history(input);
	tokens = lexer(input);
	expanser(tokens, env, last_status);
	remove_quotes(tokens);
	cmds = parser(tokens);
	if (cmds)
	{
		last_status = execute(cmds, env);
		if (g_received_signal == SIGINT)
		{
			write(1, "\n", 1);
			rl_on_new_line();
			rl_replace_line("", 0);
		}
	}
	free_tokens(tokens);
	free_cmds(cmds);
	return (last_status);
}

int	main(int ac, char **av, char **env)
{
	char	*input;
	int		last_status;

	((void) ac, (void) av);
	init_signals();
	last_status = 0;
	while (1)
	{
		g_received_signal = 0;
		input = readline(PROMPT);
		if (!input)
			break ;
		if (*input)
			last_status = shell_loop(input, env, last_status);
		free(input);
	}
	rl_clear_history();
	return (0);
}
