/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asid-ahm <asid-ahm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 03:43:20 by asid-ahm          #+#    #+#             */
/*   Updated: 2024/07/24 03:43:47 by asid-ahm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_getenv(char *the_env)
{
	if (the_env)
	{
		while (*the_env)
		{
			if (*the_env == '=')
				return (++the_env);
			the_env++;
		}
		return the_env;
	}
	return (NULL);
}
