/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanser_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 13:18:39 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/16 13:35:08 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char *var, char **env)
{
	int		i;
	size_t	len;

	if (!var || !env)
		return ("");
	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return ("");
}

int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

size_t	get_var_name_len(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '?')
		return (1);
	while (str[i] && is_var_char(str[i]))
		i++;
	return (i);
}

void	insert_status_value(char *dest, size_t *j, int last_status)
{
	char	*status_str;
	size_t	k;

	status_str = ft_itoa(last_status);
	k = 0;
	while (status_str && status_str[k])
	{
		dest[*j] = status_str[k];
		(*j)++;
		k++;
	}
	free(status_str);
}

size_t	add_var_len(char *str, size_t *i, char **env, int status)
{
	char	*var;
	char	*status_str;
	size_t	len;

	if (str[*i + 1] == '?')
	{
		status_str = ft_itoa(status);
		len = ft_strlen(status_str);
		free(status_str);
		*i += 2;
		return (len);
	}
	var = ft_substr(str, *i + 1, get_var_name_len(str + *i + 1));
	len = ft_strlen(get_env_value(var, env));
	*i += 1 + get_var_name_len(str + *i + 1);
	free(var);
	return (len);
}
