/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 13:00:08 by madelwau          #+#    #+#             */
/*   Updated: 2026/09/03 00:50:55 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_received_signal = 0;

/*
** ============================================================================
** clean_str
** ============================================================================
** Alloue une nouvelle chaine et y recopie 'str' en retirant les quotes qui
** ont servi a definir les arguments (quote removal), conformement au
** comportement classique de Bash.
**
** Seuls les quotes exterieurs et effectifs sont supprimes :
** Un simple quote dans des doubles quotes (ex: "hello 'world'") ou inversement
** est preserve car il ne sert pas de delimiteur a ce niveau.
**
** Retourne la chaine "nettoyee" sans les quotes, ou NULL en cas d'echec.
** ============================================================================
*/
static char	*clean_str(char *str)
{
	char	*clean;
	char	quote;
	int		i;
	int		j;

	clean = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!clean)
		return (NULL);
	quote = '\0';
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '\'' || str[i] == '"') && quote == '\0')
			quote = str[i];
		else if (str[i] == quote)
			quote = '\0';
		else
			clean[j++] = str[i];
		i++;
	}
	clean[j] = '\0';
	return (clean);
}

/*
** ============================================================================
** remove_quotes
** ============================================================================
** Parcourt toute la liste chainee des tokens et applique l'etape de "Quote
** Removal" sur chaque TOKEN_WORD.
**
** Remplace la chaine originale du token par sa version nettoyee (sans les
** quotes protecteurs devenus inutiles apres l'expansion) via clean_str(),
** puis libere proprement l'ancienne chaine de caracteres.
** ============================================================================
*/
static void	remove_quotes(t_token *tokens)
{
	t_token	*tmp;
	char	*old_str;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD)
		{
			old_str = tmp->str;
			tmp->str = clean_str(old_str);
			free(old_str);
		}
		tmp = tmp->next;
	}
}

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
		last_status = execute(cmds, env);
	return (free_tokens(tokens), free_cmds(cmds), last_status);
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

	((void) ac, (void) av);
	my_env = dup_env(envp);
	if (!my_env)
		return (1);
	init_signals();
	last_status = 0;
	while (1)
	{
		g_received_signal = 0;
		input = readline(PROMPT);
		if (!input)
			break ;
		if (*input)
			last_status = shell_loop(input, &my_env, last_status);
		free(input);
	}
	rl_clear_history();
	free_env(my_env);
	return (last_status);
}
