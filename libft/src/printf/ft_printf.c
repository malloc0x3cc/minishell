/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 03:18:31 by madelwau          #+#    #+#             */
/*   Updated: 2026/03/09 15:07:11 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static size_t	format_string(va_list args, const char fs)
{
	size_t	len;

	len = 0;
	if (fs == '%')
		len += write(1, "%", 1);
	else if (fs == 'c')
		len += ft_printchar(va_arg(args, int));
	else if (fs == 's')
		len += ft_printstr(va_arg(args, char *));
	else if (fs == 'p')
		len += ft_printptr(va_arg(args, void *));
	else if (fs == 'd' || fs == 'i')
		len += ft_printnbr(va_arg(args, int), DEC);
	else if (fs == 'u')
		len += ft_printnbr(va_arg(args, unsigned int), DEC);
	else if (fs == 'x')
		len += ft_printunbr(va_arg(args, unsigned int), HEX_LOWER);
	else if (fs == 'X')
		len += ft_printunbr(va_arg(args, unsigned int), HEX_UPPER);
	return (len);
}

int	ft_printf(const char *s, ...)
{
	va_list	args;
	size_t	len;

	if (!s)
		return (-1);
	va_start(args, s);
	len = 0;
	while (*s)
	{
		if (*s == '%' && !*(s + 1))
			return (-1);
		else if (*s == '%' && *(s + 1))
		{
			len += format_string(args, *(s + 1));
			s += 2;
		}
		else
			len += ft_printchar(*s++);
	}
	return (va_end(args), len);
}
