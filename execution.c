/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gahubert <gahubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:01:26 by gahubert          #+#    #+#             */
/*   Updated: 2026/06/11 13:07:30 by gahubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute(/*int infile, */ char **args)
{
	pid_t	pid[9999];
	int		i;
	int		status;

	i = 0;
	while (args[i])
	{
		pid[i] = fork();
		if (pid[i] == -1)
			return (1);
		if (pid[i] == 0)
		{
			printf("Pid : %d Arg : %s\n", pid[i], args[i]);
			exit(0);
		}
		if (!wait_children(pid[i], &status))
			return (printf("error"), 1);
		i++;
	}
	return (0);
}
