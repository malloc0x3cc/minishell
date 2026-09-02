/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_unset.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:49:09 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/02 07:49:12 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Valide le nom de variable selon les regles bash :
** - Doit commencer par une lettre ou '_'
** - Suite de lettres, chiffres ou '_' jusqu'au '=' ou '\0'
*/
static int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!is_var_char(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
** Affiche l'environnement au format export si aucun argument n'est donne.
*/
static void	print_export_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		ft_putstr_fd("declare -x ", STDOUT_FILENO);
		ft_putendl_fd(env[i], STDOUT_FILENO);
		i++;
	}
}

int	builtin_export(char **args, char ***env)
{
	int	i;
	int	status;

	status = 0;
	if (!args[1])
		return (print_export_env(*env), 0);
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else
			set_env_val(args[i], env);
		i++;
	}
	return (status);
}

int	builtin_unset(char **args, char ***env)
{
	int	i;
	int	status;

	status = 0;
	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]) || ft_strchr(args[i], '='))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else
			unset_env_val(args[i], env);
		i++;
	}
	return (status);
}
