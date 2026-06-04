/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 17:33:56 by madelwau          #+#    #+#             */
/*   Updated: 2025/11/23 23:52:04 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	len_s1;
	size_t	len_s2;
	char	*s;

	len_s1 = ft_strlen(s1);
	len_s2 = ft_strlen(s2);
	s = malloc((len_s1 + len_s2 + 1) * sizeof(char));
	if (!s)
		return (NULL);
	ft_strlcpy(s, s1, len_s1 + 1);
	ft_strlcpy((s + len_s1), s2, len_s2 + 1);
	return (s);
}
