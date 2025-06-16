#include "../minishell.h"

/*
 * Implements the pwd builtin command.
 * Prints the current working directory path to stdout using the getcwd
 * system call. Handles errors by printing an error message via perror.
 *
 * @param env: Pointer to environment variables list (unused)
 * @return: Always returns 1
 * Side effects: Writes current directory to stdout, may write error to stderr
 */
int	ft_pwd(t_list **env)
{
	char	*cwd;

	cwd = ft_getenv("PWD", *env);
	if (cwd != NULL)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
	else
		perror("pwd");
	return (0);
}
