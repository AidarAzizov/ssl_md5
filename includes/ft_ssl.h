#ifndef FT_SSH_H
# define FT_SSH_H

#define BUFF_SIZE 1024

#define OVERCODE_MAXNAME (6 + 1) //base64



#include "hashing.h"
#include "overcoding.h"


void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memset(void *destination, int c, size_t n);
size_t	ft_strlen(char const *s);
char	*ft_strcpy(char *dst, const char *src);
int		ft_strcmp(const char *s1, const char *s2);


#endif