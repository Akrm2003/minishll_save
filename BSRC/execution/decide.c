/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decide.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asid-ahm <asid-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 12:29:45 by asid-ahm          #+#    #+#             */
/*   Updated: 2024/08/09 03:02:07 by asid-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdio.h>

static int	my_waitpid(pid_t *pid, int size, int *status)
{
	int	i;

	i = 0;
	while (i < size)
	{
		waitpid(pid[i], status, 0);
		if (i == size - 1)
			return (WEXITSTATUS(*status));
		i++;
	}
	return (0);
}

static void open_all(t_cmd *cmd, int *tmp_fd, char **env)
{
	int		size;
	int		i;
	t_cmd	*temp;
	// int		*status;

	i = 0;
	size = ft_cmdlstsize(cmd);
	temp = cmd;
	// status = malloc(sizeof(int) * size);
	while (temp)
	{
		if (temp->redirect)
		{
			temp->redirect_status = the_ultimate_dup(cmd, temp->redirect, tmp_fd, env);
		}
		else
		{
			temp->redirect_status = 0;
		}
		i++;
		temp = temp->next;
	}
}

void close_last(t_files *redirection)
{
	while (redirection)
	{
		if (redirection->last_output)
		{
			if (redirection->type == REDIR_OUT || redirection->type == REDIR_APPEND)
			{
				// printf("1 did you close the file descriptor?\n");
				if (redirection->fd != -1)
					close(redirection->fd);
			}
		}
		else if (redirection->last_input)
		{
			if (redirection->type == REDIR_IN)
			{
				// printf("2 did you close the file descriptor?\n");
				if (redirection->fd != -1)
					close(redirection->fd);
			}
			else if (redirection->type == REDIR_HEREDOC)
			{
				// printf("3 did you close the file descriptor?\n");
				if (redirection->heredoc_fd[0] != -1)
					close(redirection->heredoc_fd[0]);
			}
		}
		redirection = redirection->next;
	}
}
static void loop_close(t_cmd *cmd)
{
	while (cmd)
	{
		close_last(cmd->redirect);
		cmd = cmd->next;
	}
}

void	pipe_decide(t_cmd *cmd, char **env, int tmp_fd[2])
{
	pid_t	*pid;
	t_cmd	*temp;
	int		*fd_status;
	int		status;
	int		i;
	int		size;
	int		fd[2];
	int		restore_fd[2];

	i = 0;
	status = 0;
	size = ft_cmdlstsize(cmd);
	open_all(cmd, tmp_fd, env);
	temp = cmd;
	while (i < size && temp)
	{
		if (size && i == 0)
			pid = malloc(sizeof(pid_t) * size);
		pipe(fd);
		pid[i] = fork();
		if (!pid[i])
		{
			close(tmp_fd[0]);
			close(tmp_fd[1]);
			ft_free(pid, NULL);
			execute_helper(temp, fd);
			if (temp->redirect_status == 0)
				execute(cmd, temp, env, fd);
			else
			{
				close (fd[0]);
				close (fd[1]);
				close_all_fd(cmd);
				free_cmd(cmd);
				// printf("here\n");
				exit (1);
			}
		}
		else
		{
			close(fd[1]);
			dup2(fd[0], 0);
			close(fd[0]);
			if (i == size - 1)
			{
				g_exit_status = my_waitpid(pid, size, &status);
				loop_close(cmd);
				ft_free(pid, NULL);
				// exit(status);
			}
		}
		temp = temp->next;
		i++;
	}
}
