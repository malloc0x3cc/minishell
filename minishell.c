/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ghub <ghub@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:00:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/11 14:29:29 by ghub             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_all(char **arr)
{
	int i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

int	main(int argc, char **argv, char **envp)
{
	char	*input;
	char	**args;
	t_cmd	*cmd;

	(void)argc;
	(void)argv;
	while (1)
	{
		input = readline(PROMPT);
		if (!input)
			break ;
		if (*input)
		{
			add_history(input);
			args = ft_split(input, ' ');
			cmd = malloc(sizeof(t_cmd));
			if (cmd)
			{
				cmd->args = args;
				cmd->next = NULL;
				execute(cmd, envp);
				free(cmd);
			}
			free(input);
			free_all(args);
		}
	}
	rl_clear_history();
	return (0);
}
