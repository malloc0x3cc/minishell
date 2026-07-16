/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 12:05:14 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/16 13:58:49 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	get_expanded_len(char *str, char **env, int last_status)
{
	size_t	len;
	size_t	i;
	int		s_q;
	int		d_q;

	len = 0;
	i = 0;
	s_q = 0;
	d_q = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !d_q)
			s_q = !s_q;
		else if (str[i] == '"' && !s_q)
			d_q = !d_q;
		else if (str[i] == '$' && !s_q && str[i + 1]
			&& (is_var_char(str[i + 1]) || str[i + 1] == '?'))
		{
			len += add_var_len(str, &i, env, last_status);
			continue ;
		}
		(void)((len++) + (i++));
	}
	return (len);
}

static void	sub_var(char *str, char *dest, size_t *i, t_sub_ctx *ctx)
{
	char	*var;
	char	*val;
	size_t	k;

	if (str[*i + 1] == '?')
	{
		insert_status_value(dest, &ctx->j, ctx->status);
		*i += 2;
		return ;
	}
	var = ft_substr(str, *i + 1, get_var_name_len(str + *i + 1));
	val = get_env_value(var, ctx->env);
	k = 0;
	while (val[k])
		dest[(ctx->j)++] = val[k++];
	*i += 1 + get_var_name_len(str + *i + 1);
	free(var);
}

static void	init_ctx(t_sub_ctx *ctx, char **env, int status)
{
	ctx->j = 0;
	ctx->s_q = 0;
	ctx->d_q = 0;
	ctx->env = env;
	ctx->status = status;
}

static char	*substitute_variables(char *str, size_t new_len, char **env,
	int last_status)
{
	char		*dest;
	size_t		i;
	t_sub_ctx	ctx;

	dest = malloc(sizeof(char) * (new_len + 1));
	if (!dest)
		return (NULL);
	i = 0;
	init_ctx(&ctx, env, last_status);
	while (str[i])
	{
		if (str[i] == '\'' && !ctx.d_q)
			ctx.s_q = !ctx.s_q;
		else if (str[i] == '"' && !ctx.s_q)
			ctx.d_q = !ctx.d_q;
		else if (str[i] == '$' && !ctx.s_q && str[i + 1]
			&& (is_var_char(str[i + 1]) || str[i + 1] == '?'))
		{
			sub_var(str, dest, &i, &ctx);
			continue ;
		}
		dest[ctx.j++] = str[i++];
	}
	dest[ctx.j] = '\0';
	return (dest);
}

void	expanser(t_token *tokens, char **env, int last_status)
{
	t_token	*tmp;
	char	*old_str;
	size_t	new_len;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD)
		{
			new_len = get_expanded_len(tmp->str, env, last_status);
			old_str = tmp->str;
			tmp->str = substitute_variables(old_str, new_len, env, last_status);
			free(old_str);
		}
		tmp = tmp->next;
	}
}
