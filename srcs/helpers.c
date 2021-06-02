#include "../includes/ft_ssl.h"

int		ft_strcmp(const char *s1, const char *s2)
{
    int		i;

    i = 0;
    while (*(s1 + i) && *(s1 + i) == *(s2 + i))
        i++;
    return (*((unsigned char *)s1 + i) - *((unsigned char *)s2 + i));
}

char	*ft_strcpy(char *dst, const char *src)
{
    int		i;

    i = -1;
    while (*(src + ++i))
        *(dst + i) = *(src + i);
    *(dst + i) = '\0';
    return (dst);
}

size_t	ft_strlen(char const *s)
{
    size_t	count;

    count = 0;
    while (s[count] != '\0')
        count++;
    return (count);
}

void	*ft_memset(void *destination, int c, size_t n)
{
    size_t	i;

    i = 0;
    if (n == 0)
        return (destination);
    while (i < n)
    {
        ((unsigned char *)destination)[i] = (unsigned char)c;
        i++;
    }
    return (destination);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
    char		*dstc;
    const char	*srccc;

    if ((dst == src) || n == 0)
        return (dst);
    if (!dst && !src)
        return (0);
    dstc = (char *)dst;
    srccc = (const char *)src;
    while (n--)
        dstc[n] = srccc[n];
    return (dst);
}
