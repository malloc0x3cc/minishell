/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gahubert <gahubert@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 00:00:00 by gahubert          #+#    #+#             */
/*   Updated: 2026/06/24 00:00:00 by gahubert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** Fichier de tests pour execution.c
**
** Compilation (depuis la racine du projet) :
**   cc -Wall -Wextra -Werror \
**      test_execution.c execution.c \
**      -L./libft -lft \
**      -I./includes \
**      -o test_execution
**
** Puis lancer :
**   ./test_execution
*/

#include "minishell.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* ======================================================================== */
/*  Helpers                                                                  */
/* ======================================================================== */

/* Couleurs terminal */
#define GREEN  "\033[0;32m"
#define RED    "\033[0;31m"
#define YELLOW "\033[0;33m"
#define RESET  "\033[0m"

static int	g_passed = 0;
static int	g_failed = 0;

static void	print_result(const char *test_name, int ok)
{
	if (ok)
	{
		printf(GREEN "[PASS]" RESET " %s\n", test_name);
		g_passed++;
	}
	else
	{
		printf(RED "[FAIL]" RESET " %s\n", test_name);
		g_failed++;
	}
}

/*
** Construit un t_cmd simple depuis un tableau args (pas de free, tests only).
** Si next_cmd != NULL, chaîne les deux.
*/
static t_cmd	*make_cmd(char **args, t_cmd *next_cmd)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = args;
	cmd->next = next_cmd;
	return (cmd);
}

/*
** Lit le contenu d'un fichier dans un buffer statique (tests only).
** Retourne NULL si erreur.
*/
static char	*read_file(const char *path)
{
	static char	buf[4096];
	int			fd;
	ssize_t		n;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	n = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	if (n < 0)
		return (NULL);
	buf[n] = '\0';
	/* Retire le \n final si présent */
	if (n > 0 && buf[n - 1] == '\n')
		buf[n - 1] = '\0';
	return (buf);
}

/* ======================================================================== */
/*  Tests                                                                    */
/* ======================================================================== */

/*
** TEST 1 — Commande simple : echo hello
** Vérifie que execute() retourne 0 et que la sortie est correcte.
** On redirige stdout vers un fichier temporaire pour capturer la sortie.
*/
static void	test_simple_echo(char **env)
{
	t_cmd	*cmd;
	char	*args[] = {"echo", "hello", NULL};
	int		ret;
	int		saved_stdout;
	int		fd;
	char	*content;

	/* Redirige stdout vers /tmp/test_echo.txt */
	fd = open("/tmp/test_echo.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	saved_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);

	cmd = make_cmd(args, NULL);
	ret = execute(cmd, env);

	/* Restaure stdout */
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	free(cmd);

	content = read_file("/tmp/test_echo.txt");
	print_result("simple echo hello → exit 0",
		ret == 0);
	print_result("simple echo hello → output 'hello'",
		content && strcmp(content, "hello") == 0);
}

/*
** TEST 2 — Commande introuvable
** Vérifie que execute() retourne 127 pour une commande inconnue.
*/
static void	test_command_not_found(char **env)
{
	t_cmd	*cmd;
	char	*args[] = {"__commande_inexistante_42__", NULL};
	int		ret;
	int		saved_stderr;

	/* Supprime le message d'erreur pour ne pas polluer la sortie des tests */
	saved_stderr = dup(STDERR_FILENO);
	int devnull = open("/dev/null", O_WRONLY);
	dup2(devnull, STDERR_FILENO);
	close(devnull);

	cmd = make_cmd(args, NULL);
	ret = execute(cmd, env);

	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stderr);
	free(cmd);

	print_result("command not found → exit 127", ret == 127);
}

/*
** TEST 3 — Redirection sortie : echo world > /tmp/test_redir.txt
** Vérifie que le fichier est créé et contient "world".
*/
static void	test_redir_output(char **env)
{
	t_cmd	*cmd;
	char	*args[] = {"echo", "world", ">", "/tmp/test_redir.txt", NULL};
	int		ret;
	char	*content;

	unlink("/tmp/test_redir.txt");
	cmd = make_cmd(args, NULL);
	ret = execute(cmd, env);
	free(cmd);

	content = read_file("/tmp/test_redir.txt");
	print_result("redir '>' → exit 0", ret == 0);
	print_result("redir '>' → fichier contient 'world'",
		content && strcmp(content, "world") == 0);
}

/*
** TEST 4 — Redirection append : deux fois echo >> fichier
** Vérifie que le fichier contient les deux lignes.
*/
static void	test_redir_append(char **env)
{
	t_cmd	*cmd;
	char	*args1[] = {"echo", "ligne1", ">>", "/tmp/test_append.txt", NULL};
	char	*args2[] = {"echo", "ligne2", ">>", "/tmp/test_append.txt", NULL};
	int		fd;
	char	buf[256];
	ssize_t	n;

	unlink("/tmp/test_append.txt");

	cmd = make_cmd(args1, NULL);
	execute(cmd, env);
	free(cmd);

	cmd = make_cmd(args2, NULL);
	execute(cmd, env);
	free(cmd);

	fd = open("/tmp/test_append.txt", O_RDONLY);
	n = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	buf[n] = '\0';

	print_result("redir '>>' → les deux lignes présentes",
		strstr(buf, "ligne1") && strstr(buf, "ligne2"));
}

/*
** TEST 5 — Redirection entrée : cat < /tmp/test_input.txt
** Crée un fichier avec "42Network", vérifie que cat le relit.
*/
static void	test_redir_input(char **env)
{
	t_cmd	*cmd;
	char	*args[] = {"cat", "<", "/tmp/test_input.txt", NULL};
	int		ret;
	int		fd;
	int		saved_stdout;
	char	*content;

	/* Crée le fichier source */
	fd = open("/tmp/test_input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	write(fd, "42Network\n", 10);
	close(fd);

	/* Capture stdout */
	fd = open("/tmp/test_cat_out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	saved_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);

	cmd = make_cmd(args, NULL);
	ret = execute(cmd, env);
	free(cmd);

	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);

	content = read_file("/tmp/test_cat_out.txt");
	print_result("redir '<' → exit 0", ret == 0);
	print_result("redir '<' → output '42Network'",
		content && strcmp(content, "42Network") == 0);
}

/*
** TEST 6 — Pipe simple : echo foo | cat
** Vérifie que la sortie finale contient "foo".
*/
static void	test_pipe_simple(char **env)
{
	t_cmd	*cmd1;
	t_cmd	*cmd2;
	char	*args1[] = {"echo", "foo", NULL};
	char	*args2[] = {"cat", NULL};
	int		ret;
	int		saved_stdout;
	int		fd;
	char	*content;

	fd = open("/tmp/test_pipe.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	saved_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);

	cmd2 = make_cmd(args2, NULL);
	cmd1 = make_cmd(args1, cmd2);
	ret = execute(cmd1, env);
	free(cmd1);
	free(cmd2);

	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);

	content = read_file("/tmp/test_pipe.txt");
	print_result("pipe echo | cat → exit 0", ret == 0);
	print_result("pipe echo | cat → output 'foo'",
		content && strcmp(content, "foo") == 0);
}

/*
** TEST 7 — Pipe double : echo "hello world" | cat | cat
** Vérifie que deux pipes chainés fonctionnent.
*/
static void	test_pipe_double(char **env)
{
	t_cmd	*cmd1;
	t_cmd	*cmd2;
	t_cmd	*cmd3;
	char	*args1[] = {"echo", "hello world", NULL};
	char	*args2[] = {"cat", NULL};
	char	*args3[] = {"cat", NULL};
	int		ret;
	int		saved_stdout;
	int		fd;
	char	*content;

	fd = open("/tmp/test_pipe2.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	saved_stdout = dup(STDOUT_FILENO);
	dup2(fd, STDOUT_FILENO);
	close(fd);

	cmd3 = make_cmd(args3, NULL);
	cmd2 = make_cmd(args2, cmd3);
	cmd1 = make_cmd(args1, cmd2);
	ret = execute(cmd1, env);
	free(cmd1);
	free(cmd2);
	free(cmd3);

	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);

	content = read_file("/tmp/test_pipe2.txt");
	print_result("pipe double echo | cat | cat → exit 0", ret == 0);
	print_result("pipe double → output 'hello world'",
		content && strcmp(content, "hello world") == 0);
}

/*
** TEST 8 — Pipe + redirection : echo bar | cat > /tmp/test_pipe_redir.txt
** Vérifie que la redirection en fin de pipe fonctionne.
*/
static void	test_pipe_with_redir(char **env)
{
	t_cmd	*cmd1;
	t_cmd	*cmd2;
	char	*args1[] = {"echo", "bar", NULL};
	char	*args2[] = {"cat", ">", "/tmp/test_pipe_redir.txt", NULL};
	char	*content;

	unlink("/tmp/test_pipe_redir.txt");
	cmd2 = make_cmd(args2, NULL);
	cmd1 = make_cmd(args1, cmd2);
	execute(cmd1, env);
	free(cmd1);
	free(cmd2);

	content = read_file("/tmp/test_pipe_redir.txt");
	print_result("pipe + redir '>' → fichier contient 'bar'",
		content && strcmp(content, "bar") == 0);
}

/*
** TEST 9 — Code de sortie d'une commande qui échoue
** "ls /chemin_qui_nexiste_pas" doit retourner != 0.
*/
static void	test_exit_code_failure(char **env)
{
	t_cmd	*cmd;
	char	*args[] = {"ls", "/chemin_absolument_inexistant_42", NULL};
	int		ret;
	int		saved_stderr;

	saved_stderr = dup(STDERR_FILENO);
	int devnull = open("/dev/null", O_WRONLY);
	dup2(devnull, STDERR_FILENO);
	close(devnull);

	cmd = make_cmd(args, NULL);
	ret = execute(cmd, env);
	free(cmd);

	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stderr);

	print_result("exit code != 0 sur commande échouée", ret != 0);
}

/*
** TEST 10 — Redirection vers fichier inexistant en lecture
** "cat < /tmp/fichier_qui_nexiste_pas" doit retourner 1.
*/
static void	test_redir_input_missing_file(char **env)
{
	t_cmd	*cmd;
	char	*args[] = {"cat", "<", "/tmp/__missing_42__.txt", NULL};
	int		ret;
	int		saved_stderr;

	unlink("/tmp/__missing_42__.txt");

	saved_stderr = dup(STDERR_FILENO);
	int devnull = open("/dev/null", O_WRONLY);
	dup2(devnull, STDERR_FILENO);
	close(devnull);

	cmd = make_cmd(args, NULL);
	ret = execute(cmd, env);
	free(cmd);

	dup2(saved_stderr, STDERR_FILENO);
	close(saved_stderr);

	print_result("redir '<' fichier manquant → exit != 0", ret != 0);
}

/* ======================================================================== */
/*  main                                                                     */
/* ======================================================================== */

int	main(int argc, char **argv, char **env)
{
	(void)argc;
	(void)argv;

	printf(YELLOW "\n=== Tests execution.c (minishell gahubert) ===\n\n" RESET);

	test_simple_echo(env);
	test_command_not_found(env);
	test_redir_output(env);
	test_redir_append(env);
	test_redir_input(env);
	test_pipe_simple(env);
	test_pipe_double(env);
	test_pipe_with_redir(env);
	test_exit_code_failure(env);
	test_redir_input_missing_file(env);

	printf(YELLOW "\n=== Résultats : %d/%d passés ===\n\n" RESET,
		g_passed, g_passed + g_failed);

	return (g_failed > 0 ? 1 : 0);
}