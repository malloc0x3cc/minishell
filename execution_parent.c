/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_parent.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:40:04 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/02 07:40:06 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	restore_std(int saved_in, int saved_out)
{
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (1);
}

int	exec_single_parent_builtin(t_cmd *cmd, char ***env, int hd_fd)
{
	int	saved_in;
	int	saved_out;
	int	status;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (saved_in == -1 || saved_out == -1)
		return (1);
	if (hd_fd != -1)
	{
		dup2(hd_fd, STDIN_FILENO);
		close(hd_fd);
	}
	if (apply_redirs(cmd->redirs) != 0)
	{
		restore_std(saved_in, saved_out);
		return (1);
	}
	status = exec_builtin(cmd, env);
	restore_std(saved_in, saved_out);
	return (status);
}
