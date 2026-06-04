/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/25 01:25:12 by madelwau          #+#    #+#             */
/*   Updated: 2026/02/28 16:59:56 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*get_remaining(char *static_buffer)
{
	char	*remaining;
	int		i;

	i = 0;
	while (static_buffer[i] && static_buffer[i] != '\n')
		i++;
	if (!static_buffer[i] || !static_buffer[i + 1])
		return (free(static_buffer), NULL);
	remaining = ft_strdup(static_buffer + i + 1);
	return (free(static_buffer), remaining);
}

static char	*get_line(char *static_buffer)
{
	char	*line;
	size_t	i;
	size_t	len;

	i = 0;
	if (!static_buffer || !static_buffer[0])
		return (NULL);
	while (static_buffer[i] && static_buffer[i] != '\n')
		i++;
	len = i + (static_buffer[i] == '\n');
	line = malloc(sizeof(char) * (len + 1));
	if (!line)
		return (NULL);
	ft_memcpy(line, static_buffer, len);
	line[len] = '\0';
	return (line);
}

static char	*get_fd_input(int fd, char *static_buffer)
{
	static char	temp_read[BUFFER_SIZE + 1];
	ssize_t		readlen;

	readlen = 1;
	while (!ft_strchr(static_buffer, '\n') && readlen > 0)
	{
		readlen = read(fd, temp_read, BUFFER_SIZE);
		if (readlen == -1)
			return (free(static_buffer), NULL);
		if (readlen == 0)
			break ;
		temp_read[readlen] = '\0';
		static_buffer = ft_strjoin(static_buffer, temp_read);
		if (!static_buffer)
			return (NULL);
	}
	return (static_buffer);
}

char	*get_next_line(int fd)
{
	static char	*remaining;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	remaining = get_fd_input(fd, remaining);
	if (!remaining)
		return (NULL);
	line = get_line(remaining);
	if (!line)
	{
		free(remaining);
		remaining = NULL;
		return (NULL);
	}
	remaining = get_remaining(remaining);
	return (line);
}
