/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yagame <yagame@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 18:19:52 by yagame            #+#    #+#             */
/*   Updated: 2025/04/19 18:20:10 by yagame           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char **handel_quote(char **cmd)
{
    char **trim;
    int i;

    i = 0;
    while(cmd[i])
        i++;
    trim = malloc(sizeof(char *) * (i + 1));
    if (!trim)
        return (NULL);
    i = 0;
    while (cmd[i])
    {
        trim[i] = ft_strtrim(cmd[i], "\"");
        if (!trim[i])
            return (NULL);
        i++;
    }
    trim[i] = NULL;
    free_dp(cmd);
    return (trim);
}