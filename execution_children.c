/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_children.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: madelwau <madelwau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 13:52:13 by ghub              #+#    #+#             */
/*   Updated: 2026/09/02 19:22:28 by madelwau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <sys/stat.h>

/*
** ============================================================================
** ft_dup2_close
** ============================================================================
** Petite fonction utilitaire qui factorise un motif repete 3 fois dans
** setup_fds() : dupliquer un fd vers une destination standard (stdin ou
** stdout), verifier l'erreur, puis fermer le fd d'origine devenu inutile.
**
** En cas d'echec de dup2(), on affiche l'erreur avec perror() et on quitte
** immediatement avec exit(1), car sans une redirection correcte le
** processus enfant ne peut pas continuer dans un etat coherent.
**
** Cette fonction n'existe que pour eviter la duplication de code et
** garder setup_fds() courte et lisible.
** ============================================================================
*/
static void	ft_dup2_close(int old_fd, int new_fd)
{
	if (dup2(old_fd, new_fd) == -1)
	{
		perror("dup2");
		exit(1);
	}
	close(old_fd);
}

/*
** ============================================================================
** setup_fds
** ============================================================================
** Met en place les entrees/sorties du processus enfant a partir d'une
** struct t_fds regroupant in_fd, out_fd et hd_fd.
**
** Ordre des redirections (important) :
**   1. D'abord in_fd / out_fd : ce sont les bouts de pipe herites de la
**      commande precedente/suivante dans une chaine "cmd1 | cmd2 | cmd3".
**   2. Ensuite hd_fd : si un heredoc a ete lu pour cette commande, il prend
**      la PRIORITE sur in_fd, car "<<" redirige explicitement l'entree,
**      exactement comme le ferait bash.
**
** Chaque redirection effective est deleguee a ft_dup2_close(), qui gere
** le dup2() suivi de la fermeture du fd d'origine.
** ============================================================================
*/
static void	setup_fds(t_fds *fds)
{
	if (fds->in_fd != STDIN_FILENO)
		ft_dup2_close(fds->in_fd, STDIN_FILENO);
	if (fds->out_fd != STDOUT_FILENO)
		ft_dup2_close(fds->out_fd, STDOUT_FILENO);
	if (fds->hd_fd != -1)
		ft_dup2_close(fds->hd_fd, STDIN_FILENO);
}

/*
** ============================================================================
** cmd_not_found
** ============================================================================
** Affiche le message d'erreur standard "command not found" sur stderr,
** au format utilise par la plupart des shells (bash inclus), puis quitte
** le processus enfant avec exit(127), qui est le code de sortie
** conventionnel pour une commande introuvable.
**
** Isoler ce message dans sa propre fonction permet de garder exec_cmd()
** focalisee sur la logique d'execution plutot que sur le formatage
** d'un message d'erreur.
** ============================================================================
*/
/*
** ============================================================================
** handle_exec_error
** ============================================================================
** Affiche l'erreur appropriee et quitte le processus enfant avec le bon code :
**   - 127 : Commande introuvable dans PATH ou fichier inexistant (avec slash).
**   - 126 : Cible est un repertoire ou probleme de permission (non executable).
** ============================================================================
*/
static void	handle_exec_error(char *path, char *cmd_name)
{
	struct stat	path_stat;

	ft_putstr_fd("minishell: ", 2);
	if (!path || !ft_strchr(cmd_name, '/'))
	{
		ft_putstr_fd(cmd_name, 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd(path, 2);
		ft_putendl_fd(": Is a directory", 2);
		free(path);
		exit(126);
	}
	perror(path);
	if (access(path, F_OK) != 0)
	{
		free(path);
		exit(127);
	}
	free(path);
	exit(126);
}

/*
** ============================================================================
** exec_cmd
** ============================================================================
** Prepare puis lance l'execution reelle de la commande, une fois que les
** fds (pipes/heredoc) ont deja ete mis en place par setup_fds().
**
** Etapes :
**   1. apply_redirs() : gere les ">" ">>" "<" restants directement dans
**      les args (ceux-ci peuvent encore ecraser stdin/stdout, comme dans
**      un vrai shell ou la derniere redirection l'emporte).
**   2. clean_args() : retire les tokens de redirection des args pour ne
**      garder que la commande et ses vrais arguments.
**   3. find_path() : cherche le chemin complet de l'executable dans le
**      PATH de l'environnement.
**   4. execve() : remplace le processus enfant par la commande demandee.
**
** Si execve() echoue (fichier trouve mais non executable, par exemple),
** on affiche l'erreur avec perror() et on quitte avec exit(1). Si la
** commande n'existe pas du tout, on delegue a cmd_not_found() qui quitte
** avec exit(127) (convention shell).
** ============================================================================
*/
static void	exec_cmd(t_cmd *cmd, char **env)
{
	char	*path;

	if (apply_redirs(cmd->redirs) != 0)
		exit(1);
	if (!cmd->args || !cmd->args[0]|| !cmd->args[0][0])
		exit(0);
	if (is_builtin(cmd->args[0]))
		exit(exec_builtin(cmd, &env));
	path = find_path(cmd->args[0], env);
	if (!path || ft_strchr(cmd->args[0], '/'))
	{
		if (!path || access(path, F_OK) != 0)
			handle_exec_error(path, cmd->args[0]);
	}
	execve(path, cmd->args, env);
	handle_exec_error(path, cmd->args[0]);
}

/*
** ============================================================================
** child_exec
** ============================================================================
** Code execute DANS LE PROCESSUS ENFANT (apres le fork), pour une commande
** donnee. Met en place les entrees/sorties puis lance execve().
**
** Cette fonction est volontairement reduite a un role d'orchestrateur,
** pour respecter la norme 42 (max 25 lignes, max 4 parametres) :
**   - setup_fds()  s'occupe de toute la mise en place des fds (pipes,
**                  heredoc), via la struct t_fds qui les regroupe.
**   - exec_cmd()   s'occupe de la logique de redirection sur les args,
**                  de la recherche du binaire et de l'execve() final.
**
** En regroupant in_fd/out_fd/hd_fd dans t_fds, child_exec() ne prend plus
** que 3 parametres (cmd, fds, env) au lieu des 5 d'origine.
** ============================================================================
*/
void	child_exec(t_cmd *cmd, t_fds *fds, char **env)
{
	reset_signals_for_child();
	setup_fds(fds);
	exec_cmd(cmd, env);
}
