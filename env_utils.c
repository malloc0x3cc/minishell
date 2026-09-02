/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 07:31:05 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/02 07:48:53 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_size(char **env)
{
	int	i;

	i = 0;
	while (env && env[i])
		i++;
	return (i);
}

void	free_env(char **env)
{
	int	i;

	if (!env)
		return ;
	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

char	**dup_env(char **env)
{
	char	**new_env;
	int		size;
	int		i;

	size = env_size(env);
	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_env[i] = ft_strdup(env[i]);
		if (!new_env[i])
			return (free_env(new_env), NULL);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

/*
** Ajoute ou remplace une variable au format "KEY=VALUE".
*/
int	set_env_val(char *key_value, char ***env)
{
	char	**new_env;
	int		len;
	int		i;
	int		size;

	len = 0;
	while (key_value[len] && key_value[len] != '=')
		len++;
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key_value, len) == 0 && (*env)[i][len] == '=')
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(key_value);
			return (0);
		}
		i++;
	}
	size = env_size(*env);
	new_env = malloc(sizeof(char *) * (size + 2));
	if (!new_env)
		return (1);
	i = -1;
	while (++i < size)
		new_env[i] = (*env)[i];
	new_env[size] = ft_strdup(key_value);
	new_env[size + 1] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}

/*
** Supprime une variable correspondant a 'key' du tableau d'environnement.
*/
int	unset_env_val(char *key, char ***env)
{
	char	**new_env;
	int		size;
	int		i;
	int		j;
	int		len;

	if (!key || !*env)
		return (0);
	len = ft_strlen(key);
	size = env_size(*env);
	new_env = malloc(sizeof(char *) * (size + 1));
	if (!new_env)
		return (1);
	i = 0;
	j = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], key, len) == 0
			&& ((*env)[i][len] == '=' || (*env)[i][len] == '\0'))
			free((*env)[i]);
		else
			new_env[j++] = (*env)[i];
		i++;
	}
	new_env[j] = NULL;
	free(*env);
	*env = new_env;
	return (0);
}
