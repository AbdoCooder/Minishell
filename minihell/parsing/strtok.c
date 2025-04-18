/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strtok.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:08:28 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/15 16:14:10 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_lexer	*ft_lexer_init(char *input)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
	lexer->input = input;
	lexer->pos = 0;
	lexer->len = ft_strlen(input);
	return (lexer);
}

t_token	*ft_strtok(char *input, t_list *minienv)
{
	t_lexer	*lexer;
	t_token	*token;
	t_token	*token_list;

	lexer = ft_lexer_init(input);
	token_list = NULL;
	token = ft_get_next_token(lexer);
	while (token)
	{
		if (token->type != SINGLE_QUOTE)
			ft_expand_variables(&token->value, minienv);
		if (token->value)
			ft_tokadd_back(&token_list, ft_newtok(token));
		ft_free_token(token);
		token = ft_get_next_token(lexer);
	}
	free(lexer);
	return (token_list);
}
