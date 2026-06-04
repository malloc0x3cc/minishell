/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 15:55:12 by madelwau          #+#    #+#             */
/*   Updated: 2025/11/24 03:28:12 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*s_ptr;

	s_ptr = (char *)s + ft_strlen(s);
	while (s_ptr >= s)
	{
		if (*s_ptr == (char)c)
			return (s_ptr);
		s_ptr--;
	}
	if (!c)
		return (s_ptr);
	return (NULL);
}
