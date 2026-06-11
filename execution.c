/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gahubert <gahubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:01:26 by gahubert          #+#    #+#             */
/*   Updated: 2026/06/11 16:43:25 by gahubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(char **scmd)
{
	pid_t	pid;
	int		i;
	int		status;

	i = 0;
	while (scmd[i])
	{
		pid = fork();
		if (pid == -1)
			return (1);
		if (pid == 0)
		{
			printf("%s\n", scmd[i]);
			exit(0);
		}
		if (!wait_children(pid, &status))
			return (printf("error"), 1);
		i++;
	}
	return (0);
}
