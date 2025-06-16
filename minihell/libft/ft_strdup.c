#include "libft.h"

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(const char *src)
{
	int		slen;
	char	*dest;

	slen = 0;
	if (src == NULL)
		return (NULL);
	while (src[slen])
		slen++;
	dest = (char *)malloc(sizeof(char) * slen + 1);
	if (!dest)
		return (NULL);
	ft_strcpy(dest, src);
	return (dest);
}
