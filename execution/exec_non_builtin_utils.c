/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_non_builtin_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdrahm <hdrahm@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 16:50:47 by hdrahm            #+#    #+#             */
/*   Updated: 2024/10/19 09:23:05 by hdrahm           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_free_arr(char **paths)
{
	int	i;

	i = 0;
	while (paths[i] != NULL)
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

int	if_contain_directory(char *commande)
{
	int	i;

	i = 0;
	while (commande[i])
	{
		if (commande[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

int	count_nodes(t_env *envp)
{
	int	count;

	count = 0;
	while (envp)
	{
		count++;
		envp = envp->next;
	}
	return (count);
}

int	check_if_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode) != 0)
		{
			ft_print_in_stderr(path, ": Is a directory", "\n");
			exit(126);
		}
		if (access(path, X_OK) != 0)
		{
			perror(path);
			exit(126);
		}
		return (1);
	}
	else
	{
		perror(path);
		exit(127);
	}
}
