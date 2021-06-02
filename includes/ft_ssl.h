#ifndef FT_SSH_H
# define FT_SSH_H

#define BUFF_SIZE 1024

#define MD5_HLEN (32)
#define SHA256_HLEN (64)
#define SHA512_HLEN (128)

#define MAX_HNAME_LEN (6 + 1)
#define MAX_HLEN (SHA512_HLEN + 1)

#include <stdlib.h>
#include "utils.h"

typedef struct s_md5
{
	uint8_t	*prepared_message;
	size_t	prepared_message_len;
	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
} t_md5;


typedef struct s_sha256
{
	size_t		offset;
	uint8_t		*prepared_message;
	size_t		prepared_message_len;
	uint32_t h0;
	uint32_t h1;
	uint32_t h2;
	uint32_t h3;
	uint32_t h4;
	uint32_t h5;
	uint32_t h6;
	uint32_t h7;
} t_sha256;

typedef struct s_error
{
    BOOL was_error;
	char *error_message;
} t_error;

typedef struct s_data
{
	t_error			err;
    BOOL			is_string;
    BOOL			is_stdin;
	uint8_t			*payload;
	size_t			payload_len;
	char			*payload_name;
	char			hash[MAX_HLEN];
	struct s_data	*next;
} t_data;


typedef struct s_flags
{
    BOOL p;
    BOOL q;
    BOOL r;
} t_flags;


typedef struct s_env
{
	t_flags		flags;
	t_data		*data;
    BOOL 	    (*hash_func)(struct s_env*);
	char		hashname_uppercase[MAX_HNAME_LEN];
	char		hashname_lowercase[MAX_HNAME_LEN];

} t_env;

void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memset(void *destination, int c, size_t n);
size_t	ft_strlen(char const *s);
char	*ft_strcpy(char *dst, const char *src);
int		ft_strcmp(const char *s1, const char *s2);


void    PrintHashes(t_env *env);

BOOL	parse(int argc, char *argv[], t_env* env);

BOOL	md5(t_env *env);

BOOL	sha256(t_env *env);

#endif