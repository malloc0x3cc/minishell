/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_pixel.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 13:00:20 by madelwau          #+#    #+#             */
/*   Updated: 2026/05/06 13:04:40 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	put_pixel(t_mlx *data, int x, int y, int color)
{
	int	offset;

	offset = (y * data->line_length) + (x * (data->bits_per_pixel / 8));
	*(unsigned int *)(data->addr + offset) = color;
}
