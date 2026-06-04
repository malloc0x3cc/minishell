/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_prints.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 18:43:52 by madelwau          #+#    #+#             */
/*   Updated: 2026/01/21 13:45:09 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

size_t	ft_printchar(const char c)
{
	return (write(1, &c, 1));
}

size_t	ft_printstr(const char *s)
{
	if (!s)
		return (ft_printstr("(null)"));
	return (write(1, s, ft_strlen(s)));
}

size_t	ft_printptr(const void *p)
{
	size_t	len;

	len = 0;
	if (!p)
		return (ft_printstr("(nil)"));
	len += ft_printstr("0x");
	len += ft_printunbr((unsigned long)p, HEX_LOWER);
	return (len);
}

size_t	ft_printunbr(unsigned long n, char *base)
{
	size_t			len;
	unsigned long	base_len;

	base_len = ft_strlen(base);
	len = 0;
	if (n >= base_len)
		len += ft_printunbr((n / base_len), base);
	len += ft_printchar(base[n % base_len]);
	return (len);
}

size_t	ft_printnbr(long n, char *base)
{
	size_t	len;

	len = 0;
	if (n < 0)
	{
		len += ft_printchar('-');
		n = -n;
	}
	len += ft_printunbr(n, base);
	return (len);
}
