#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new_node)
{
	t_list	*last;

	if (!lst || !new_node)
		return ;
	if (*lst == NULL)
	{
		new_node->prev = NULL;
		*lst = new_node;
	}
	else
	{
		last = *lst;
		while (last->next)
			last = last->next;
		new_node->prev = last;
		last->next = new_node;
	}
}
