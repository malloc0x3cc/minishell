/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 19:00:14 by madelwau          #+#    #+#             */
/*   Updated: 2026/05/04 19:20:37 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	ft_atof(const char *nptr)
{
	double	n;
	double	pow;
	int		sign;

	n = 0;
	sign = 1;
	pow = 1.0;
	while (ft_isspace(*nptr))
		nptr++;
	if ((*nptr == '+' || *nptr == '-') && *nptr++ == '-')
		sign = -1;
	while (ft_isdigit(*nptr))
		n = (n * 10) + (*nptr++ - '0');
	if (*nptr == '.')
	{
		while (ft_isdigit(*++nptr))
		{
			n = n * 10 + (*nptr++ - '0');
			pow *= 10;
		}
	}
	return (n / pow * sign);
}
