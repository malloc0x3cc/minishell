/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 12:00:00 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/16 14:19:45 by madelwau         ###   ########.fr       */
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
static void	handle_sigint_prompt(int sig)
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
static void	handle_sigint_exec(int sig)
{
	g_received_signal = sig;
}

/*
** ============================================================================
** init_signals
** ============================================================================
** Configure le comportement par defaut des signaux pour le shell interactif :
**   - SIGINT (Ctrl+C) est capte par handle_sigint_prompt() pour annuler la
**     saisie courante et relancer proprement un nouveau prompt.
**   - SIGQUIT (Ctrl+\) est purement et simplement ignore (SIG_IGN), pour eviter
**     que le shell ne quitte brutalement avec un "Quit (core dumped)".
**
** Utilise la structure moderne sigaction pour garantir la portabilite et la
** robustesse du comportement (avec SA_RESTART pour eviter d'interrompre les
** appels systeme lents).
** ============================================================================
*/
void	init_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = &handle_sigint_prompt;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

/*
** ============================================================================
** set_signals_for_exec
** ============================================================================
** Ajuste les signaux juste avant de lancer l'execution d'un pipeline de
** commandes.
**
** Durant l'execution, SIGINT est associe a handle_sigint_exec() afin de ne
** pas interferer avec l'affichage de Readline pendant que les enfants tournent.
** SIGQUIT reste ignore au niveau du parent, mais les processus enfants
** heriteront de leur propre configuration.
** ============================================================================
*/
void	set_signals_for_exec(void)
{
	struct sigaction	sa;

	sa.sa_handler = &handle_sigint_exec;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
}

/*
** ============================================================================
** reset_signals_for_child
** ============================================================================
** Restaure les comportements par defaut (SIG_DFL) des signaux SIGINT et
** SIGQUIT au sein d'un processus enfant venant d'etre fork().
**
** C'est une etape essentielle de child_exec() : sans elle, un binaire execute
** par minishell (comme 'cat' ou 'grep') ignorerait le Ctrl+\ et ne pourrait
** pas etre interrompu correctement par l'utilisateur, ce qui violerait le
** comportement standard de Bash.
** ============================================================================
*/
void	reset_signals_for_child(void)
{
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}
