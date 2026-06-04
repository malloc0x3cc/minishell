/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 18:37:24 by madelwau          #+#    #+#             */
/*   Updated: 2025/11/23 23:39:10 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static size_t	nbr_size(long n)
{
	size_t	i;

	i = 1;
	if (n < 0)
	{
		n = -n;
		i++;
	}
	while (n > 9)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static void	ft_get_digits(char *s, long n, int *i)
{
	if (n > 9)
		ft_get_digits(s, n / 10, i);
	s[(*i)++] = (n % 10) + '0';
}

char	*ft_itoa(int n)
{
	int		i;
	long	nb;
	char	*s;

	i = 0;
	nb = (long)n;
	s = malloc((nbr_size(nb) + 1) * sizeof(char));
	if (!s)
		return (NULL);
	if (nb < 0)
	{
		s[i++] = '-';
		nb = -nb;
	}
	ft_get_digits(s, nb, &i);
	s[i] = '\0';
	return (s);
}
