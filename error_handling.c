/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gahubert <gahubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 13:02:57 by gahubert          #+#    #+#             */
/*   Updated: 2026/06/11 13:04:53 by gahubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_children(pid_t pid, int *status)
{
	if (waitpid(pid, status, 0) == -1)
	{
		perror("error");
		return (0);
	}
	return (1);
}
