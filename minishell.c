/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:00:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/06/05 00:00:52 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av)
{
	char	*test;

	(void)ac;
	(void)av;
	while (1)
	{
		test = readline(PROMPT);
		printf("%s\n", test);
	}
	return (0);
}
