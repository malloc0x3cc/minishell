/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gahubert <gahubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:01:26 by gahubert          #+#    #+#             */
/*   Updated: 2026/06/18 15:57:03 by gahubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	token_redir(t_cmd *cmd)
{
	if (cmd->type == TOKEN_OUTFILE)
		redir_out(cmd);
	else if (cmd->type == TOKEN_INFILE)
		redir_in(cmd);
	else if (cmd->type == TOKEN_APPEND)
		redir_append(cmd);
	else if (cmd->type == TOKEN_PIPE)
		redir_pipe(cmd);
	else if (cmd->type == TOKEN_HEREDOC)
		redir_heredoc(cmd);
	return ;
}

static int	redirin(t_cmd *cmd)
{

}

static int	redirout(t_cmd *cmd)
{

}

static int	append(t_cmd *cmd)
{

}

int	execute(t_cmd *cmd)
{
	pid_t	pid;
	int		i;
	int		status;

	i = 0;
	while (cmd && cmd->next)
	{
		pid = fork();
		if (pid == -1)
			return (1);
		if (pid == 0)
		{
			token_redir(cmd->type);
			exit(0);
		}
		cmd = cmd->next;
	}
	if (!wait_children(pid, &status))
		return (printf("error"), 1);
	return (0);
}
