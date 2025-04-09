/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenajib <abenajib@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 16:13:08 by abenajib          #+#    #+#             */
/*   Updated: 2025/04/09 22:24:54 by abenajib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_isredi(t_token *token)
{
	return (token->type == INPUT || token->type == OUTPUT
			|| token->type == HEREDOC || token->type == APPEND);
}

void	ft_parse_word(t_cmdarg **node, t_token *token_list)
{
	char	*tmp;
	tmp = (*node)->strags;//point on the old cmdargs
	(*node)->strags = ft_strjoin((*node)->strags, token_list->current->value);//join the new word
	free(tmp);//free the old
	// add space between the old and the next new word
	tmp = (*node)->strags;
	(*node)->strags = ft_strjoin((*node)->strags, " ");
	free(tmp);
}

t_redi_list	*ft_redinew(t_token *token)
{
	t_redi_list	*new;

	new = malloc(sizeof(t_redi_list));
	if (!new)
		return (NULL);
	new->type = token->type;
	new->file = token->next->value;
	new->next = NULL;
	return (new);
}

void	ft_rediradd(t_redi_list **redi, t_redi_list *new)
{
	t_redi_list	*tmp;

	if (!*redi)
	{
		*redi = new;
		return ;
	}
	tmp = *redi;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

bool	is_input(t_token_type type)
{
	return (type == INPUT || type == HEREDOC);
}

void	ft_parse_redi(t_cmdarg **node, t_token *token_list)
{
	if (is_input(token_list->type))
		ft_rediradd(&(*node)->input, ft_redinew(token_list->current));
	// else
	// 	ft_rediradd(&(*node)->output, ft_redinew(token_list->current));
	token_list->current = token_list->current->next;//skip redi then the file after
}

t_cmdarg	*ft_init_node()
{
	t_cmdarg	*node;

	node = malloc(sizeof(t_cmdarg));
	if (!node)
		return (NULL);
	node->input = NULL;
	node->is_builtin = false;
	node->next = NULL;
	node->output = NULL;
	node->strags = NULL;
	return (node);
}

t_cmdarg	*get_next_node(t_token *token_list)
{
	t_cmdarg	*node;

	node = ft_init_node();
	while (token_list->current && token_list->current->type != PIPE)//inner loop on all tokens till find the pipe
	{
		if (token_list->current->type == WORD)
			ft_parse_word(&node, token_list);
		else if (ft_isredi(token_list->current))
			ft_parse_redi(&node, token_list);
		token_list->current = token_list->current->next;
	}
	printf("cmdargs: [%s]\n", node->strags);
	t_redi_list *c = node->input;
	while (c)
	{
		printf("[%s]-{%s}\n", c->file, printtype(c->type));
		c = c->next;
	}
	exit(0);
	return (NULL);
}
