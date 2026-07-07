/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 16:01:26 by gahubert          #+#    #+#             */
/*   Updated: 2026/07/07 12:46:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


/*
** ============================================================================
** find_path
** ============================================================================
** Cherche la commande "cmd" (ex: "ls") dans les repertoires listes dans la
** variable d'environnement PATH, et retourne le chemin complet trouve
** (ex: "/bin/ls").
**
** Fonctionnement :
**   1. On cherche la ligne "PATH=..." dans le tableau env.
**   2. On decoupe cette ligne sur les ":" pour obtenir un tableau de
**      repertoires (ex: ["/usr/bin", "/bin", ...]).
**   3. Pour chaque repertoire, on construit le chemin complet
**      "repertoire/cmd" et on teste s'il est executable avec access().
**   4. Des qu'on trouve un chemin valide, on libere le tableau de chemins
**      et on retourne le chemin trouve (le caller doit le free()).
**
** Retourne NULL si PATH n'existe pas dans env, ou si aucune commande
** correspondante n'est trouvee.
** ============================================================================
*/
static char	*find_path(char *cmd, char **env)
{
	char	**paths;
	char	*tmp;
	char	*full;
	int		i;

	i = 0;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (!env[i])
		return (NULL);
	paths = ft_split(env[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full, X_OK) == 0)
		{
			ft_free_tab(paths);
			return (full);
		}
		free(full);
		i++;
	}
	ft_free_tab(paths);
	return (NULL);
}

/*
** ============================================================================
** apply_redirs
** ============================================================================
** Applique les redirections classiques presentes dans args : ">", ">>", "<".
** Le heredoc "<<" N'EST PAS traite ici : il est deja resolu en amont, avant
** le fork, par handle_heredocs() (voir plus bas). A ce stade, si un token
** "<<" traine encore dans args, on le laisse simplement de cote (il sera de
** toute facon retire par clean_args() avant l'execve).
**
** Pour chaque token de redirection trouve :
**   - ">"  : ouvre le fichier cible en ecriture, l'ecrase (O_TRUNC), et
**            redirige stdout dessus.
**   - ">>" : pareil mais en mode ajout (O_APPEND) au lieu d'ecraser.
**   - "<"  : ouvre le fichier cible en lecture et redirige stdin dessus.
**
** Retourne 0 si tout s'est bien passe, 1 si une erreur survient (fichier
** introuvable, dup2 qui echoue, etc.) auquel cas un message d'erreur est
** affiche via perror().
** ============================================================================
*/
static int	apply_redirs(char **args)
{
	int	i;
	int	fd;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 && args[i + 1])
		{
			fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (perror(args[i + 1]), 1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (close(fd), perror("dup2"), 1);
			close(fd);
			i += 2;
		}
		else if (ft_strcmp(args[i], ">>") == 0 && args[i + 1])
		{
			fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (perror(args[i + 1]), 1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (close(fd), perror("dup2"), 1);
			close(fd);
			i += 2;
		}
		else if (ft_strcmp(args[i], "<") == 0 && args[i + 1])
		{
			fd = open(args[i + 1], O_RDONLY);
			if (fd == -1)
				return (perror(args[i + 1]), 1);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (close(fd), perror("dup2"), 1);
			close(fd);
			i += 2;
		}
		else
			i++;
	}
	return (0);
}

/*
** ============================================================================
** clean_args
** ============================================================================
** Construit une copie de args debarrassee de tous les tokens de
** redirection ET de leurs cibles, pour obtenir le tableau final a passer
** a execve().
** Ex: ["cat", "<<", "EOF", "-n", ">", "out.txt", NULL]
**     -> ["cat", "-n", NULL]
**
** Fonctionnement en deux passes :
**   1. Premiere passe : on compte combien d'elements "utiles" il reste
**      (hors tokens de redirection et leurs arguments).
**   2. On alloue un tableau de la bonne taille.
**   3. Deuxieme passe : on recopie uniquement les elements utiles.
**
** Retourne NULL si l'allocation echoue.
** ============================================================================
*/
static char	**clean_args(char **args)
{
	char	**clean;
	int		i;
	int		j;
	int		count;

	count = 0;
	i = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0
				|| ft_strcmp(args[i], "<") == 0
				|| ft_strcmp(args[i], "<<") == 0) && args[i + 1])
			i += 2;
		else
			(count++, i++);
	}
	clean = malloc(sizeof(char *) * (count + 1));
	if (!clean)
		return (NULL);
	i = 0;
	j = 0;
	while (args[i])
	{
		if ((ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0
				|| ft_strcmp(args[i], "<") == 0
				|| ft_strcmp(args[i], "<<") == 0) && args[i + 1])
			i += 2;
		else
			clean[j++] = args[i++];
	}
	clean[j] = NULL;
	return (clean);
}

/*
** ============================================================================
** read_heredoc
** ============================================================================
** Lit un heredoc "<< delim" DANS LE PROCESSUS PARENT, avant le fork.
**
** Pourquoi dans le parent et pas dans l'enfant ?
**   Si on lisait le heredoc dans child_exec() (donc apres le fork), et qu'il
**   y a plusieurs commandes pipees ayant chacune un heredoc
**   (ex: "cat << A | grep x << B"), on se retrouverait avec plusieurs
**   processus enfants qui essaient de lire au clavier EN MEME TEMPS :
**   comportement imprevisible et casse. En lisant tout en amont dans le
**   parent, un seul heredoc est lu a la fois, dans l'ordre, avant meme
**   qu'aucun fork n'ait lieu.
**
** Fonctionnement :
**   1. On cree un pipe : pipe_fd[0] = lecture, pipe_fd[1] = ecriture.
**   2. On boucle : on lit une ligne au clavier avec readline("> ").
**      - Si l'utilisateur tape Ctrl+D (EOF), readline retourne NULL -> stop.
**      - Si la ligne tapee est exactement egale au delimiteur -> stop.
**      - Sinon on ecrit la ligne + un saut de ligne dans pipe_fd[1].
**   3. On ferme le bout ecriture du pipe (plus besoin d'ecrire).
**   4. On retourne pipe_fd[0] : ce fd, une fois lu, redonnera tout le texte
**      tape par l'utilisateur, comme si c'etait un fichier.
**
** Retourne le fd de lecture du pipe, ou -1 en cas d'erreur (pipe() echoue).
** ============================================================================
*/
static int	read_heredoc(char *delim)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) == -1)
		return (perror("pipe"), -1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

/*
** ============================================================================
** count_cmds
** ============================================================================
** Compte le nombre de commandes chainees dans la liste (via ->next).
** Sert a savoir quelle taille donner au tableau hd_fds dans execute().
** ============================================================================
*/
static int	count_cmds(t_cmd *cmd)
{
	int	n;

	n = 0;
	while (cmd)
	{
		n++;
		cmd = cmd->next;
	}
	return (n);
}

/*
** ============================================================================
** handle_heredocs
** ============================================================================
** Parcourt TOUTE la liste de commandes AVANT le fork, et pour chaque
** commande qui contient un "<<", lit le heredoc correspondant via
** read_heredoc() et stocke le fd resultant dans hd_fds[i] (i = index de
** la commande dans la chaine).
**
** Si une commande n'a pas de heredoc, hd_fds[i] vaut -1.
** Si une commande a plusieurs "<<" (ex: "cat << A << B"), seul le DERNIER
** compte : on ferme le fd precedent et on le remplace, ce qui reproduit le
** comportement standard de bash.
**
** Retourne 0 si tout s'est bien passe, 1 si une erreur est survenue
** (pipe() a echoue quelque part).
** ============================================================================
*/
static int	handle_heredocs(t_cmd *cmd, int *hd_fds)
{
	int	i;
	int	j;

	i = 0;
	while (cmd)
	{
		hd_fds[i] = -1;
		j = 0;
		while (cmd->args[j])
		{
			if (ft_strcmp(cmd->args[j], "<<") == 0 && cmd->args[j + 1])
			{
				if (hd_fds[i] != -1)
					close(hd_fds[i]);
				hd_fds[i] = read_heredoc(cmd->args[j + 1]);
				if (hd_fds[i] == -1)
					return (1);
				j += 2;
			}
			else
				j++;
		}
		i++;
		cmd = cmd->next;
	}
	return (0);
}

/*
** ============================================================================
** child_exec
** ============================================================================
** Code execute DANS LE PROCESSUS ENFANT (apres le fork), pour une commande
** donnee. Met en place les entrees/sorties puis lance execve().
**
** Ordre des redirections (important) :
**   1. D'abord in_fd / out_fd : ce sont les bouts de pipe herites de la
**      commande precedente/suivante dans une chaine "cmd1 | cmd2 | cmd3".
**   2. Ensuite hd_fd : si un heredoc a ete lu pour cette commande, il prend
**      la PRIORITE sur in_fd, car "<<" redirige explicitement l'entree,
**      exactement comme le ferait bash.
**   3. Enfin apply_redirs() : gere les ">" ">>" "<" restants sur les args
**      (qui eux-memes peuvent encore ecraser stdin/stdout si presents,
**      comme dans un vrai shell ou la derniere redirection l'emporte).
**
** Une fois tout en place :
**   - On nettoie args des tokens de redirection via clean_args().
**   - On cherche le chemin complet de la commande via find_path().
**   - On lance execve(). Si execve() echoue, on affiche l'erreur et on
**     quitte avec exit(1). Si la commande n'existe pas, on affiche
**     "command not found" et on quitte avec exit(127) (convention shell).
** ============================================================================
*/
static void	child_exec(t_cmd *cmd, int in_fd, int out_fd, char **env,
		int hd_fd)
{
	char	*path;
	char	**exec_args;

	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			(perror("dup2"), exit(1));
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			(perror("dup2"), exit(1));
		close(out_fd);
	}
	if (hd_fd != -1)
	{
		if (dup2(hd_fd, STDIN_FILENO) == -1)
			(perror("dup2"), exit(1));
		close(hd_fd);
	}
	if (apply_redirs(cmd->args) != 0)
		exit(1);
	exec_args = clean_args(cmd->args);
	if (!exec_args || !exec_args[0])
		exit(0);
	path = find_path(exec_args[0], env);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(exec_args[0], 2);
		ft_putchar_fd('\n', 2);
		exit(127);
	}
	execve(path, exec_args, env);
	perror(path);
	free(path);
	exit(1);
}

/*
** ============================================================================
** wait_children
** ============================================================================
** Attend la fin de TOUS les processus enfants lances (utile pour les
** pipelines a plusieurs commandes, ou chaque commande est un enfant).
** waitpid(-1, ...) attend n'importe quel enfant, un par un, jusqu'a ce
** qu'il n'y en ait plus (retour -1, typiquement ECHILD).
**
** Retourne le code de sortie du DERNIER enfant qui a termine, converti via
** WEXITSTATUS si le processus s'est termine normalement, sinon 1.
** ============================================================================
*/
int	wait_children(pid_t last_pid, int *status)
{
	pid_t	pid;

	while (1)
	{
		pid = waitpid(-1, status, 0);
		if (pid == -1)
			break ;
	}
	(void)last_pid;
	if (WIFEXITED(*status))
		return (WEXITSTATUS(*status));
	return (1);
}

/*
** ============================================================================
** execute
** ============================================================================
** Fonction principale d'execution. Gere les pipes entre les t_cmd chaines
** via ->next, ainsi que les heredocs eventuels.
**
** Etapes :
**   1. On compte le nombre de commandes (count_cmds) pour allouer un
**      tableau hd_fds de la bonne taille : un slot par commande, qui
**      contiendra soit -1 (pas de heredoc), soit un fd deja pret a lire.
**   2. AVANT tout fork, on appelle handle_heredocs() qui lit
**      sequentiellement tous les heredocs de toutes les commandes de la
**      chaine. C'est la partie cruciale qui evite les lectures clavier
**      concurrentes entre plusieurs enfants.
**   3. On boucle ensuite sur chaque commande comme avant :
**      - Si elle a une commande suivante (cmd->next), on cree un pipe
**        pour communiquer avec elle.
**      - On fork : l'enfant appelle child_exec() avec le hd_fds[idx]
**        correspondant a sa position dans la chaine.
**      - Le parent ferme les fds qu'il n'utilise plus et avance in_fd
**        vers le prochain bout de pipe.
**   4. A la fin, on libere hd_fds et on attend tous les enfants via
**      wait_children(), dont on retourne le code de sortie.
** ============================================================================
*/
int	execute(t_cmd *cmd, char **env)
{
	int		pipe_fd[2];
	int		in_fd;
	int		*hd_fds;
	int		n;
	int		idx;
	pid_t	pid;
	int		status;
	t_cmd	*head;

	head = cmd;
	n = count_cmds(cmd);
	hd_fds = malloc(sizeof(int) * n);
	if (!hd_fds)
		return (1);
	if (handle_heredocs(head, hd_fds) != 0)
		return (free(hd_fds), 1);
	in_fd = STDIN_FILENO;
	idx = 0;
	pid = 0;
	cmd = head;
	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(pipe_fd) == -1)
				return (free(hd_fds), perror("pipe"), 1);
		}
		else
			pipe_fd[1] = STDOUT_FILENO;
		pid = fork();
		if (pid == -1)
			return (free(hd_fds), perror("fork"), 1);
		if (pid == 0)
		{
			if (cmd->next)
				close(pipe_fd[0]);
			child_exec(cmd, in_fd, pipe_fd[1], env, hd_fds[idx]);
		}
		if (cmd->next)
			close(pipe_fd[1]);
		if (in_fd != STDIN_FILENO)
			close(in_fd);
		if (hd_fds[idx] != -1)
			close(hd_fds[idx]);
		in_fd = (cmd->next) ? pipe_fd[0] : STDIN_FILENO;
		idx++;
		cmd = cmd->next;
	}
	free(hd_fds);
	return (wait_children(pid, &status));
}
