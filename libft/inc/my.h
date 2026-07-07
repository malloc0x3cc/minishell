/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/17 21:30:27 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/07 09:04:57 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MY_H
# define MY_H

/* My additions to libft */
/* is */
int		ft_isspace(int c);
int		ft_islower(int c);
int		ft_isupper(int c);
/* put */
void	ft_putchar(char c);
void	ft_putstr(char *s);
void	ft_putendl(char *s);
void	ft_putnbr(int nb);
/* str */
void	ft_free_tab(char **tab);
int		ft_strcmp(const char *s1, const char *s2);
/* to */
double	ft_atof(const char *nptr);
/* mlx */
typedef struct s_mlx
{
	void	*mlx;
	void	*win;
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_mlx;

void	put_pixel(t_mlx *data, int x, int y, int color);

#endif
