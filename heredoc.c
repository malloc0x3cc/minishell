/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gahubert <gahubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/16 14:43:36 by gahubert          #+#    #+#             */
/*   Updated: 2026/06/16 17:22:36 by gahubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static char **read_heredoc(int fd, char *delimiter)
{
	char *input;

	while(input != delimiter)
	{
		input = readline()
	}
}

static char *expand_heredoc()
{

}

void	write_heredoc()
{
	char	**heredoc;

	heredoc = read_heredoc();

	return ;
}

