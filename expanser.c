/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanser.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 12:05:14 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/16 14:06:21 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** get_expanded_len
** ============================================================================
** Calcule la taille exacte de la chaine finale apres expansion de toutes
** les variables d'environnement ($VAR, $?). Cette etape de pré-calcul est
** indispensable pour faire un malloc() precis dans substitute_variables().
**
** Gestion des quotes :
**   - s_q (single quotes) et d_q (double quotes) agissent comme des booleens
**     qui s'activent/desactivent quand on croise un "'" ou un '"'.
**   - Si on est a l'interieur de simples quotes (s_q == 1), le caractere '$'
**     perd son pouvoir special (pas d'expansion, comme dans bash).
**   - Si on est hors des simples quotes (dans des doubles quotes ou hors de
**     tout quote), et qu'on trouve un '$' valide, on delegue le calcul de la
**     taille de la variable a add_var_len().
**
** Retourne la taille totale requise pour la chaine de caracteres finale.
** ============================================================================
*/
static size_t	get_expanded_len(char *str, char **env, int last_status)
{
	size_t	len;
	size_t	i;
	int		s_q;
	int		d_q;

	len = 0;
	i = 0;
	s_q = 0;
	d_q = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !d_q)
			s_q = !s_q;
		else if (str[i] == '"' && !s_q)
			d_q = !d_q;
		else if (str[i] == '$' && !s_q && str[i + 1]
			&& (is_var_char(str[i + 1]) || str[i + 1] == '?'))
		{
			len += add_var_len(str, &i, env, last_status);
			continue ;
		}
		(void)((len++) + (i++));
	}
	return (len);
}

/*
** ============================================================================
** sub_var
** ============================================================================
** Remplace une variable d'environnement (ex: $USER ou $?) par sa valeur reelle
** et l'ecrit directement dans la chaine de destination 'dest'.
**
** Cas particulier :
**   Si la variable est '?', on appelle insert_status_value() pour recuperer
**   le code de retour de la derniere commande executee (last_status) sous
**   forme de chaine et l'ecrire.
**   Sinon, on isole le nom de la variable avec ft_substr(), on recupere sa
**   valeur dans l'environnement avec get_env_value(), puis on recopie cette
**   valeur caractere par caractere dans 'dest'.
**
** L'index de parcours 'i' de la chaine source et l'index d'ecriture 'j' (via
** ctx->j) de la chaine destination sont mis a jour pour que l'appelant
** reprenne sa boucle exactement la ou la variable s'arrete.
** ============================================================================
*/
static void	sub_var(char *str, char *dest, size_t *i, t_sub_ctx *ctx)
{
	char	*var;
	char	*val;
	size_t	k;

	if (str[*i + 1] == '?')
	{
		insert_status_value(dest, &ctx->j, ctx->status);
		*i += 2;
		return ;
	}
	var = ft_substr(str, *i + 1, get_var_name_len(str + *i + 1));
	val = get_env_value(var, ctx->env);
	k = 0;
	while (val[k])
		dest[(ctx->j)++] = val[k++];
	*i += 1 + get_var_name_len(str + *i + 1);
	free(var);
}

/*
** ============================================================================
** init_ctx
** ============================================================================
** Petite fonction utilitaire pour initialiser la structure t_sub_ctx.
**
** Cette structure est utilisee pour passer l'etat de substitution (les index,
** l'etat des quotes s_q/d_q, l'env et le status) aux sous-fonctions comme
** sub_var(). Cela permet d'eviter d'avoir trop d'arguments par fonction,
** ce qui violerait la Norme 42 (limitee a 4 parametres par fonction).
** ============================================================================
*/
static void	init_ctx(t_sub_ctx *ctx, char **env, int status)
{
	ctx->j = 0;
	ctx->s_q = 0;
	ctx->d_q = 0;
	ctx->env = env;
	ctx->status = status;
}

/*
** ============================================================================
** substitute_variables
** ============================================================================
** Alloue et construit la nouvelle chaine de caracteres en effectuant le
** remplacement reel des variables d'environnement.
**
** Elle fonctionne sur la meme logique de parsing de quotes que get_expanded_len()
** mais, au lieu de juste compter, elle recopie activement les caracteres de
** la source 'str' vers 'dest'. Des qu'elle croise un '$' expansable hors de
** simples quotes, elle passe le relais a sub_var() pour injecter la valeur.
**
** Retourne la nouvelle chaine allouee (terminée par '\0'), ou NULL en cas
** d'echec de malloc().
** ============================================================================
*/
static char	*substitute_variables(char *str, size_t new_len, char **env,
	int last_status)
{
	char		*dest;
	size_t		i;
	t_sub_ctx	ctx;

	dest = malloc(sizeof(char) * (new_len + 1));
	if (!dest)
		return (NULL);
	i = 0;
	init_ctx(&ctx, env, last_status);
	while (str[i])
	{
		if (str[i] == '\'' && !ctx.d_q)
			ctx.s_q = !ctx.s_q;
		else if (str[i] == '"' && !ctx.s_q)
			ctx.d_q = !ctx.d_q;
		else if (str[i] == '$' && !ctx.s_q && str[i + 1]
			&& (is_var_char(str[i + 1]) || str[i + 1] == '?'))
		{
			sub_var(str, dest, &i, &ctx);
			continue ;
		}
		dest[ctx.j++] = str[i++];
	}
	dest[ctx.j] = '\0';
	return (dest);
}

/*
** ============================================================================
** expanser
** ============================================================================
** Point d'entree principal de l'expansion de variables pour le shell.
** Parcourt toute la liste chainee des tokens generes par le lexer.
**
** Pour chaque token de type TOKEN_WORD (les mots simples ou chaines de caracteres
** susceptibles de contenir des variables) :
**   1. get_expanded_len()  : calcule la taille future de la chaine apres expansion.
**   2. substitute_variables() : alloue la nouvelle chaine et y remplace les $VAR.
**   3. Libere l'ancienne chaine (old_str) pour eviter les fuites memoire et
**      la remplace dans le token par la nouvelle chaine translatee.
**
** Cette fonction modifie directement la liste de tokens en place avant que le
** parser ne commence a structurer les commandes.
** ============================================================================
*/
void	expanser(t_token *tokens, char **env, int last_status)
{
	t_token	*tmp;
	char	*old_str;
	size_t	new_len;

	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == TOKEN_WORD)
		{
			new_len = get_expanded_len(tmp->str, env, last_status);
			old_str = tmp->str;
			tmp->str = substitute_variables(old_str, new_len, env, last_status);
			free(old_str);
		}
		tmp = tmp->next;
	}
}
