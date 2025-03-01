/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asid-ahm <asid-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 16:29:49 by asid-ahm          #+#    #+#             */
/*   Updated: 2024/08/09 02:09:51 by asid-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>
#include <sys/unistd.h>
#include <unistd.h>

static int	with_path(char **split, char **path, char **env)
{
	char	*command;
	int		i;

	i = -1;
	while (path[++i])
	{
		command = ft_strjoin(path[i], split[0]);
		if (!access(command, F_OK))
			break ;
		free(command);
		command = NULL;
	}
	if (command && !access(command, F_OK) && access(command, X_OK))
	{
		write(2, "zsh: permission denied: ", 24);
		ft_putstr_fd(split[0], 2);
		write(2, "\n", 1);
		ft_free(command, path);
		return (126);
	}
	if (!command || execve(command, split, env) == -1)
	{
		write(2, "zsh: command not found: ", 24);
		ft_putstr_fd(split[0], 2);
		write(2, "\n", 1);
		ft_free(command, path);
		return (127);
	}
	return (0);
}

static int	without_path(char **split, char **env)
{
	if (split[0] && !access(split[0], F_OK) && access(split[0], X_OK))
	{
		write(2, "zsh: permission denied: ", 24);
		ft_putstr_fd(split[0], 2);
		write(2, "\n", 1);
		return (126);
	}
	if (!split || !split[0] || execve(split[0], split, env) == -1)
	{
		write(2, "zsh: command not found: ", 24);
		ft_putstr_fd(split[0], 2);
		write(2, "\n", 1);
		return (127);
	}
	return (0);
}

void	execute_helper(t_cmd *cmd, int *fd)
{
	int		cmd_num;

	if (ft_cmdlstsize(cmd) > 1)
		dup2(fd[1], 1);
	close(fd[0]);
	close(fd[1]);
}
void	select_last_redirect(t_files *redirection)
{
	int	temp_fd[2];

	while (redirection)
	{
		if (redirection->last_output)
		{
			if (redirection->type == REDIR_OUT || redirection->type == REDIR_APPEND)
			{
				dup2(redirection->fd, 1);
				close(redirection->fd);
				redirection->last_output = 0;
			}
		}
		else if (redirection->last_input)
		{
			if (redirection->type == REDIR_IN)
			{
				dup2(redirection->fd, 0);
				close(redirection->fd);
				redirection->last_input = 0;
			}
			else if (redirection->type == REDIR_HEREDOC)
			{
				dup2(redirection->heredoc_fd[0], 0);
				close(redirection->heredoc_fd[0]);
				redirection->last_input = 0;
			}
		}
		redirection = redirection->next;
	}
}

void close_all_fd(t_cmd *cmd)
{
	while (cmd)
	{
		close_last(cmd->redirect);
		cmd = cmd->next;
	}
}

void	execute(t_cmd *full_cmd, t_cmd *cmd, char **env, int *fd)
{
	char	**path;
	int		status;

	status = 0;
	select_last_redirect(cmd->redirect);
	close_all_fd(full_cmd);
	if (cmd->content)
	{
		if (!ft_strchr(cmd->content[0], '/'))
		{
			path = get_path(env);
			if (!path)
				exit(1);
			status = with_path(cmd->content, path, env);
		}
		else
			status = without_path(cmd->content, env);
	}
	free_cmd(full_cmd);
	exit (status);
}
