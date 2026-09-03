/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 18:19:17 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/02 18:20:20 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>
/*
** ============================================================================
** handle_sigint_prompt
** ============================================================================
** Handler de signal specifique pour SIGINT (Ctrl+C) lorsque le shell attend
** une commande sur son prompt interactif principal.
**
** Son role est de :
**   1. Memoriser la reception du signal dans la variable globale
**      g_received_signal pour que le reste du shell sache qu'une interruption
**      a eu lieu.
**   2. Forcer l'affichage d'un saut de ligne pour revenir a la ligne.
**   3. Demander a Readline de replacer le curseur sur une nouvelle ligne vide
**      et de rafraichir l'affichage pour afficher un prompt propre.
** ============================================================================
*/
void	handle_sigint_prompt(int sig)
{
	g_received_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/*
** ============================================================================
** handle_sigint_exec
** ============================================================================
** Handler de signal minimaliste pour SIGINT utilise lorsque le shell est
** activement en train d'executer des commandes enfants en arriere-plan.
**
** On se contente d'enregistrer l'evenement dans g_received_signal. On ne doit
** surtout pas redessiner le prompt ici, car ce sont les processus enfants qui
** ont la main sur le terminal et qui s'interrompent d'eux-memes.
** ============================================================================
*/
void	handle_sigint_exec(int sig)
{
	g_received_signal = sig;
}

void	handle_sigint_heredoc(int sig)
{
	g_received_signal = sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}
