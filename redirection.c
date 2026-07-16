/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/07 15:52:25 by ghub              #+#    #+#             */
/*   Updated: 2026/07/15 18:20:41 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	open_redir_fd(t_redir *redir)
{
	if (redir->type == REDIR_OUT)
		return (open(redir->name, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	if (redir->type == REDIR_APPEND)
		return (open(redir->name, O_WRONLY | O_CREAT | O_APPEND, 0644));
	if (redir->type == REDIR_IN)
		return (open(redir->name, O_RDONLY));
	return (-1);
}

int	apply_redirs(t_redir *redir)
{
	int	fd;
	int	target;

	while (redir)
	{
		fd = open_redir_fd(redir);
		if (fd == -1)
			return (perror(redir->name), 1);
		target = STDIN_FILENO;
		if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
			target = STDOUT_FILENO;
		if (dup2(fd, target) == -1)
			return (close(fd), perror("dup2"), 1);
		close(fd);
		redir = redir->next;
	}
	return (0);
}
