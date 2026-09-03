/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:46:41 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/03 07:18:07 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
** ============================================================================
** builtin_exit
** ============================================================================
** Valide les arguments d'exit.
** - S'il y a trop d'arguments : ne quitte pas, renvoie 1 (ou 2 selon ton setup).
** - En cas d'erreur de format : affiche l'erreur et declenche la sortie a 2.
** - Sinon : programme la sortie avec le code fourni modulo 256.
** ============================================================================
*/
int	builtin_exit(char **args)
{
	int	exit_code;

	if (isatty(STDIN_FILENO))
		ft_putendl_fd("exit", STDERR_FILENO);
	if (!args[1])
		return (-256);
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		return (-256 - 2);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		return (2);
	}
	exit_code = (unsigned char)ft_atoi(args[1]);
	return (-256 - exit_code);
}
