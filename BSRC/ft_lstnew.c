/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asid-ahm <asid-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 08:54:31 by asid-ahm          #+#    #+#             */
/*   Updated: 2024/08/09 03:02:00 by asid-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_files	*ft_lstnew(void *content, int type)
{
	t_files	*node;

	node = (t_files *)malloc(sizeof(t_files));
	if (!node)
		return (NULL);
	node->file_name = content;
	node->type = type;
	node->last_input = 0;
	node->last_output = 0;
	node->heredoc_fd[0] = -1;
	node->heredoc_fd[1] = -1;
	node->fd = -1;
	node->next = NULL;
	return (node);
}
