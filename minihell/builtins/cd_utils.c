#include "../minishell.h"

int	size_dp(char **c)
{
	int	i;

	i = 0;
	while (*c)
	{
		c++;
		i++;
	}
	return (i);
}

t_list	*ft_find_node(t_list *env, char *key)
{
	t_list	*tmp;

	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, key) == 0)
			return (tmp);
		tmp = tmp->next;
	}
	return (NULL);
}
