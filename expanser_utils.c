/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanser_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 13:18:39 by madelwau          #+#    #+#             */
/*   Updated: 2026/07/16 14:18:14 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** ============================================================================
** get_env_value
** ============================================================================
** Cherche une variable d'environnement spécifique dans le tableau env et
** renvoie un pointeur vers sa valeur (la partie située juste après le '=').
**
** Si la variable demandée n'existe pas ou si l'un des paramètres est nul,
** on renvoie une chaîne vide ("") plutôt que NULL. Cela évite d'avoir à
** blinder l'appelant avec des vérifications de pointeurs et simplifie la
** concaténation (coller une chaîne vide n'altère pas le résultat final).
** ============================================================================
*/
char	*get_env_value(char *var, char **env)
{
	int		i;
	size_t	len;

	if (!var || !env)
		return ("");
	len = ft_strlen(var);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return ("");
}

/*
** ============================================================================
** is_var_char
** ============================================================================
** Petite fonction utilitaire qui détermine si un caractère est valide pour
** composer le nom d'une variable d'environnement sous Bash.
**
** Les caractères autorisés après le '$' initial sont les lettres (majuscules/
** minuscules), les chiffres, et le caractère de soulignement '_'.
** Retourne 1 (vrai) si c'est le cas, 0 (faux) sinon.
** ============================================================================
*/
int	is_var_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

/*
** ============================================================================
** get_var_name_len
** ============================================================================
** Calcule la longueur du nom de la variable d'environnement qui commence
** à l'adresse mémoire donnée par str.
**
** Cas particulier :
**   Si le premier caractère est '?', la longueur est immédiatement de 1,
**   car "$?" est une variable spéciale qui ne peut pas être suivie d'autres
**   caractères de variables standard (on ne cherche pas à parser "$?_var").
**   Sinon, on avance tant que les caractères sont validés par is_var_char().
**
** Retourne la longueur exacte du nom de la variable.
** ============================================================================
*/
size_t	get_var_name_len(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] == '?')
		return (1);
	while (str[i] && is_var_char(str[i]))
		i++;
	return (i);
}

/*
** ============================================================================
** insert_status_value
** ============================================================================
** Convertit le code de retour 'last_status' (un entier) en chaîne de
** caractères via ft_itoa(), puis l'injecte dans la chaîne de destination 'dest'
** à l'index pointé par 'j'.
**
** Cette fonction s'occupe également de libérer la mémoire allouée temporairement
** par ft_itoa() pour la conversion et de mettre à jour la variable d'index 'j'
** de l'appelant par effet de bord (via le pointeur *j).
** ============================================================================
*/
void	insert_status_value(char *dest, size_t *j, int last_status)
{
	char	*status_str;
	size_t	k;

	status_str = ft_itoa(last_status);
	k = 0;
	while (status_str && status_str[k])
	{
		dest[*j] = status_str[k];
		(*j)++;
		k++;
	}
	free(status_str);
}

/*
** ============================================================================
** add_var_len
** ============================================================================
** Calcule la longueur réelle de la valeur d'une variable d'environnement
** (ou de la variable spéciale $?) afin d'aider get_expanded_len()
** à déterminer la taille globale de la chaîne après expansion.
**
** Fonctionnement :
**   1. Si on détecte un '?' après le '$' (donc à l'index *i + 1), on convertit
**      le statut en chaîne pour mesurer sa longueur, puis on fait avancer
**      l'index global 'i' de 2 caractères (pour sauter le '$' et le '?').
**   2. Sinon, on extrait le nom de la variable avec ft_substr(), on récupère
**      sa valeur correspondante via get_env_value(), puis on mesure sa longueur.
**      L'index global 'i' est alors avancé de 1 + la longueur du nom de la
**      variable traitée.
**
** Retourne la longueur de la valeur de la variable trouvée
** (0 si elle n'existe pas).
** ============================================================================
*/
size_t	add_var_len(char *str, size_t *i, char **env, int status)
{
	char	*var;
	char	*status_str;
	size_t	len;

	if (str[*i + 1] == '?')
	{
		status_str = ft_itoa(status);
		len = ft_strlen(status_str);
		free(status_str);
		*i += 2;
		return (len);
	}
	var = ft_substr(str, *i + 1, get_var_name_len(str + *i + 1));
	len = ft_strlen(get_env_value(var, env));
	*i += 1 + get_var_name_len(str + *i + 1);
	free(var);
	return (len);
}
