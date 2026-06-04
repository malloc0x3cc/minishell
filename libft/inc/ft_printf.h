/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 03:23:21 by madelwau          #+#    #+#             */
/*   Updated: 2026/03/09 15:18:59 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stddef.h>
# include <stdarg.h>
# include <stdbool.h>

# define DEC "0123456789"
# define HEX_LOWER "0123456789abcdef"
# define HEX_UPPER "0123456789ABCDEF"

int		ft_printf(const char *s, ...);

/* ft_printf_utils */
size_t	ft_printchar(const char c);
size_t	ft_printstr(const char *s);
size_t	ft_printnbr(long n, char *base);
size_t	ft_printunbr(unsigned long n, char *base);
size_t	ft_printptr(const void *p);

/* libft */
size_t	ft_strlen(const char *s);

#endif
