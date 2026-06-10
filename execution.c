/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gahubert <gahubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:01:26 by gahubert          #+#    #+#             */
/*   Updated: 2026/06/09 18:51:38 by gahubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute(/*int infile, */ char **args)
{
	pid_t	pid[3];
	int		i;

	i = 0;
	while (args[i])
	{
		pid[i] = fork();
		if (pid[i] == -1)
			return ;
		if (pid[i] == 0)
		{
			printf("Pid : %d Arg : %s\n", pid[i], args[i]);
		}
		i++;
	}
}
