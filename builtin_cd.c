/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:46:24 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/02 07:46:25 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_pwd_vars(char ***env, char *old_pwd)
{
	char	new_pwd[1024];
	char	*str;

	if (old_pwd[0])
	{
		str = ft_strjoin("OLDPWD=", old_pwd);
		if (str)
		{
			set_env_val(str, env);
			free(str);
		}
	}
	if (getcwd(new_pwd, sizeof(new_pwd)))
	{
		str = ft_strjoin("PWD=", new_pwd);
		if (str)
		{
			set_env_val(str, env);
			free(str);
		}
	}
}

static char	*get_target_path(char **args, char **env)
{
	char	*target;

	if (!args[1])
	{
		target = get_env_value("HOME", env);
		if (!target || !*target)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), NULL);
		return (target);
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		target = get_env_value("OLDPWD", env);
		if (!target || !*target)
			return (ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2), NULL);
		ft_putendl_fd(target, STDOUT_FILENO);
		return (target);
	}
	return (args[1]);
}

int	builtin_cd(char **args, char ***env)
{
	char	old_pwd[1024];
	char	*target;

	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		old_pwd[0] = '\0';
	target = get_target_path(args, *env);
	if (!target)
		return (1);
	if (chdir(target) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(target);
		return (1);
	}
	update_pwd_vars(env, old_pwd);
	return (0);
}
