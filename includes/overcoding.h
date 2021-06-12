#ifndef OVERCODING_H
# define OVERCODING_H


typedef struct s_data_overcode
{
	struct s_data_overcode	*next;
	uint8_t					*payload;
	size_t					payload_len;

	char *overcode;

} t_data_overcode;

typedef struct s_flag_overcode
{
	BOOL d;
	BOOL e;
	BOOL i;
	BOOL o;

} t_flag_overcode;

typedef struct s_env_overcode
{
	t_data_overcode	*data;
	t_flag_overcode	t_flag_overcode;
	char			overcode_name[OVERCODE_MAXNAME];

} t_env_overcode;

void	PrintOvercode(t_env_overcode *env);

BOOL	ParseOvercode(int argc, char *argv[], t_env_overcode* env);

BOOL	base64(t_env_overcode *env);



#endif