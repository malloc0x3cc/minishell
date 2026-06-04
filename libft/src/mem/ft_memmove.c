/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 11:59:49 by madelwau          #+#    #+#             */
/*   Updated: 2025/11/24 02:47:02 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*src_ptr;
	unsigned char	*dst_ptr;

	i = 0;
	dst_ptr = (unsigned char *)dest;
	src_ptr = (unsigned char *)src;
	if (dest < src)
	{
		while (i < n)
		{
			dst_ptr[i] = src_ptr[i];
			i++;
		}
		return (dest);
	}
	while (i < n)
	{
		dst_ptr[n - i - 1] = src_ptr[n - i - 1];
		i++;
	}
	return (dest);
}
