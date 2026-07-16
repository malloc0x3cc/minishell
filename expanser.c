/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 12:05:14 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/16 12:05:15 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_env_value(char *var, char **env)
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

static int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

static size_t	get_var_name_len(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '?')
		return (1);
	while (str[i] && is_var_char(str[i]))
		i++;
	return (i);
}

static size_t	get_expanded_len(char *str, char **env)
{
	size_t	len;
	size_t	i;
	int		s_quote;
	int		d_quote;
	char	*var_name;
	char	*status_str;

	len = 0;
	i = 0;
	s_quote = 0;
	d_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !d_quote)
			(s_quote = !s_quote, len++, i++);
		else if (str[i] == '"' && !s_quote)
			(d_quote = !d_quote, len++, i++);
		else if (str[i] == '$' && !s_quote && str[i + 1]
			&& (is_var_char(str[i + 1]) || str[i + 1] == '?'))
		{
			if (str[i + 1] == '?')
			{
				status_str = ft_itoa(g_status);
				len += ft_strlen(status_str);
				free(status_str);
				i += 2;
			}
			else
			{
				var_name = ft_substr(str, i + 1, get_var_name_len(str + i + 1));
				len += ft_strlen(get_env_value(var_name, env));
				i += 1 + get_var_name_len(str + i + 1);
				free(var_name);
			}
		}
		else
			(len++, i++);
	}
	return (len);
}

static void	insert_var_value(char *dest, size_t *j, char *var_name, char **env)
{
	char	*value;
	size_t	k;

	value = get_env_value(var_name, env);
	k = 0;
	while (value[k])
	{
		dest[*j] = value[k];
		(*j)++;
		k++;
	}
}

static void	insert_status_value(char *dest, size_t *j)
{
	char	*status_str;
	size_t	k;

	status_str = ft_itoa(g_status);
	k = 0;
	while (status_str && status_str[k])
	{
		dest[*j] = status_str[k];
		(*j)++;
		k++;
	}
	free(status_str);
}

static char	*substitute_variables(char *str, size_t new_len, char **env)
{
	char	*dest;
	size_t	i;
	size_t	j;
	int		s_quote;
	int		d_quote;
	char	*var_name;

	dest = malloc(sizeof(char) * (new_len + 1));
	if (!dest)
		return (NULL);
	i = 0;
	j = 0;
	s_quote = 0;
	d_quote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !d_quote)
		{
			s_quote = !s_quote;
			dest[j++] = str[i++];
		}
		else if (str[i] == '"' && !s_quote)
		{
			d_quote = !d_quote;
			dest[j++] = str[i++];
		}
		else if (str[i] == '$' && !s_quote && str[i + 1]
			&& (is_var_char(str[i + 1]) || str[i + 1] == '?'))
		{
			if (str[i + 1] == '?')
			{
				insert_status_value(dest, &j);
				i += 2;
			}
			else
			{
				var_name = ft_substr(str, i + 1, get_var_name_len(str + i + 1));
				insert_var_value(dest, &j, var_name, env);
				i += 1 + get_var_name_len(str + i + 1);
				free(var_name);
			}
		}
		else
			dest[j++] = str[i++];
	}
	dest[j] = '\0';
	return (dest);
}

void	expanser(t_token *tokens, char **env)
{
	t_token	*tmp;
	char	*old_str;
	size_t	new_len;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD)
		{
			new_len = get_expanded_len(tmp->str, env);
			old_str = tmp->str;
			tmp->str = substitute_variables(old_str, new_len, env);
			free(old_str);
		}
		tmp = tmp->next;
	}
}
