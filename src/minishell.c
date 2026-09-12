/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:00:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/12 22:09:44 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_received_signal = 0;

/*
** ============================================================================
** filter_empty_tokens
** ============================================================================
** Supprime les tokens devenus vides suite a l'expansion d'une variable
** qui n'etait pas protegee par des quotes (ex: $EMPTY).
** ============================================================================
*/
static t_token	*filter_empty_tokens(t_token *tokens)
{
	t_token	*cur;
	t_token	*prev;
	t_token	*next;

	cur = tokens;
	prev = NULL;
	while (cur)
	{
		next = cur->next;
		if (cur->type == TOKEN_WORD && cur->str && cur->str[0] == '\0')
		{
			if (prev)
				prev->next = next;
			else
				tokens = next;
			free(cur->str);
			free(cur);
		}
		else
			prev = cur;
		cur = next;
	}
	return (tokens);
}

/*
** ============================================================================
** shell_loop
** ============================================================================
** Execute le cycle de vie complet d'une ligne de commande saisie :
**   1. add_history() : Enregistre l'entree dans l'historique readline.
**   2. lexer()       : Decoupe l'entree brute en tokens.
**   3. expanser()    : Remplace les variables $VAR et $? par leur valeur.
**   4. remove_quotes(): Retire les guillemets devenus inutiles.
**   5. parser()      : Organise les tokens en une structure de commandes.
**   6. execute()     : Lance l'execution (pipes, heredocs, redirections).
**
** Gere egalement l'affichage d'un saut de ligne si le processus a ete
** interrompu par un signal SIGINT (Ctrl+C).
** Libere toutes les structures de donnees allouees avant de retourner le
** dernier status d'execution obtenu.
** ============================================================================
*/
static int	shell_loop(char *input, char ***env, int last_status)
{
	t_token	*tokens;
	t_cmd	*cmds;

	add_history(input);
	if (is_all_spaces(input))
		return (last_status);
	tokens = lexer(input);
	if (!tokens)
		return (2);
	if (check_syntax(tokens) != 0)
		return (free_tokens(tokens), 2);
	expanser(tokens, *env, last_status);
	remove_quotes(tokens);
	tokens = filter_empty_tokens(tokens);
	if (!tokens)
		return (0);
	cmds = parser(tokens);
	if (cmds)
		last_status = execute(cmds, env, tokens);
	free_tokens(tokens);
	free_cmds(cmds);
	return (last_status);
}

static char	*get_input(void)
{
	if (isatty(STDIN_FILENO))
		return (readline(PROMPT));
	return (readline(NULL));
}

/*
** ============================================================================
** main
** ============================================================================
** Point d'entree du programme Minishell.
**
** Initialise la gestion des signaux globaux (Ctrl+C ignore Ctrl+\) et lance la
** boucle de lecture infinie (REPL) via readline().
**
** A chaque iteration :
**   - Lit l'entree utilisateur sur le prompt standard.
**   - Si l'utilisateur envoie un EOF (Ctrl+D), readline() retourne NULL, on
**     sort de la boucle pour quitter le programme proprement.
**   - Si l'entree n'est pas vide, elle est passee a shell_loop() qui traite
**     l'expression et met a jour last_status.
**
** Libere l'historique readline avant de retourner le code de sortie final.
** ============================================================================
*/
int	main(int ac, char **av, char **envp)
{
	char	*input;
	char	**my_env;
	int		last_status;

	((void)ac, (void)av);
	my_env = dup_env(envp);
	if (!my_env)
		return (1);
	init_signals();
	last_status = 0;
	rl_outstream = stderr;
	while (last_status >= 0)
	{
		g_received_signal = 0;
		input = get_input();
		if (!input)
			break ;
		if (*input)
			last_status = shell_loop(input, &my_env, last_status);
		free(input);
	}
	if (last_status < 0)
		last_status = -(last_status + 256);
	return (rl_clear_history(), free_env(my_env), last_status);
}
