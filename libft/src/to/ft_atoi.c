/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 17:06:25 by madelwau          #+#    #+#             */
/*   Updated: 2026/03/17 21:29:01 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int		n;
	int		sign;

	n = 0;
	sign = 1;
	while (ft_isspace(*nptr))
		nptr++;
	if ((*nptr == '+' || *nptr == '-') && *nptr++ == '-')
		sign = -1;
	while (ft_isdigit(*nptr))
		n = (n * 10) + (*nptr++ - '0');
	return (n * sign);
}
