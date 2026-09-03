/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatcher.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:39:28 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/02 07:39:30 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_parent_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (is_parent_builtin(cmd)
		|| ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "env") == 0);
}

int	exec_builtin(t_cmd *cmd, char ***env)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (builtin_echo(cmd->args));
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(cmd->args[0], "env") == 0)
		return (builtin_env(*env));
	if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (builtin_cd(cmd->args, env));
	if (ft_strcmp(cmd->args[0], "export") == 0)
		return (builtin_export(cmd->args, env));
	if (ft_strcmp(cmd->args[0], "unset") == 0)
		return (builtin_unset(cmd->args, env));
	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (builtin_exit(cmd->args));
	return (1);
}
