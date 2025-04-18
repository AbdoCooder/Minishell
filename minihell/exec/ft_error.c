/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 16:52:05 by otzarwal          #+#    #+#             */
/*   Updated: 2025/04/15 21:12:42 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_error(char *message)
{
	perror(message);
	exit(EXIT_FAILURE);
}


char	*find_path(t_list *path)
{
	t_list *tmp;

	tmp = path;
	while (tmp)
	{
		if (!ft_strncmp(tmp->key, "PATH", 4))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

char	*check_exec(char *p, t_list *env)
{
	char	*full_path;
	char	*path;
	char	**path_cmd;

	full_path = NULL;
	if (*p == '/' || *p == '.')
	{
		if (access(p, X_OK) == 0)
			return (ft_strdup(p));
		else
			return (NULL);
	}
	path = find_path(env);
	if (!path)
		return (NULL);
	path_cmd = parsing_split(path, ':');
	while (*path_cmd)
	{
		full_path = ft_strjoin(*path_cmd, "/");
		full_path = ft_strjoin(full_path, p);
		if (access(full_path, X_OK) == 0)
			break ;
		free(full_path);
		full_path = NULL;
		path_cmd++;
	}
	free(p);
	return (full_path);
}
