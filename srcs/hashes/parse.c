#include "../../includes/ft_ssl.h"

static t_data_hash *DataAlloc()
{
	t_data_hash *data;
	data = (t_data_hash*)malloc(sizeof(t_data_hash));
	if (data)
	{
		data->next = NULL;
		data->payload = NULL;
		data->payload_name = NULL;
		data->payload_len = 0;
		data->is_stdin = FALSE;
		data->err.was_error = FALSE;
		data->err.error_message = NULL;
        ft_strcpy(data->hash, "");
	}
	return data;
}

static BOOL	AllocatedDataInit(t_data_hash *data, BOOL is_stdin)
{
	data->payload_len = 0;
	data->payload = (uint8_t*)malloc(sizeof(uint8_t));
	if (!data->payload) return print_and_exit(ERR_ALLOC, PLUG, FALSE);

	ft_memset(data->payload, 0, 1);
	data->next = NULL; data->is_stdin = is_stdin;
	return TRUE;
}

static t_data_hash *DataInit(t_env_hash *env)
{
	if (env->data)
	{
		t_data_hash *data = env->data;
		while (data->next) data = data->next;
		data->next = DataAlloc();
		data = data->next;
		return data;
	}
	env->data = DataAlloc();
	return env->data;
}

static BOOL StringFlag(t_data_hash *data, const char *argv)
{
	data->payload_len = ft_strlen(argv);
	data->payload = (uint8_t*)malloc(sizeof(uint8_t) * data->payload_len);
	if (!data->payload) return print_and_exit(ERR_ALLOC, PLUG, FALSE);
    ft_memcpy(data->payload, argv, data->payload_len);

	data->payload_name = (char*)malloc(sizeof(char) * (ft_strlen(argv) + 1 + 2));
	if (!data->payload_name) return print_and_exit(ERR_ALLOC, PLUG, FALSE);
    ft_strcpy(data->payload_name + 1, argv); data->payload_name[ft_strlen(argv) + 2] = '\0';
	data->payload_name[0] = '\"'; data->payload_name[ft_strlen(argv) + 1] = '\"';
	return TRUE;
}

static BOOL error_initialize(t_data_hash *data, const char *message, const char *payload)
{
	data->payload_len = ft_strlen(payload);
	data->payload_name = (char*)malloc(sizeof(char) * (data->payload_len + 1));
	if (!data->payload_name) return print_and_exit(ERR_ALLOC, PLUG, FALSE);
    ft_strcpy(data->payload_name, payload); data->payload_name[data->payload_len] = '\0';

	data->err.was_error = TRUE;
	data->err.error_message = (char*)malloc(sizeof(char) * (ft_strlen(message) + 1));
	if (!data->err.error_message) return print_and_exit(ERR_ALLOC, PLUG, FALSE);
    ft_strcpy(data->err.error_message, message);
	data->err.error_message[ft_strlen(message)] = '\0';
	return TRUE;
}

static BOOL read_fd(int fd, const char *filename, t_env_hash *env, BOOL is_stdin)
{
	uint8_t	buffer[BUFF_SIZE];
	int16_t	readed_to_buff;

	t_data_hash	*data = DataInit(env);
	if (!data) return print_and_exit(ERR_ALLOC, PLUG, FALSE);
    data->is_stdin = is_stdin;

	if (fd < 0)
		return error_initialize(data, "No such file or directory", filename);
	if (read(fd, buffer, 0) == -1)
		return error_initialize(data, "Bad read file or directory", filename);

	while ((readed_to_buff = read(fd, buffer, BUFF_SIZE)) > 0)
	{
		if (!data->payload)
		{
			data->payload = (uint8_t*)malloc(sizeof(uint8_t) * (readed_to_buff + 1));
			data->payload[readed_to_buff] = '\0';
			if (!data->payload) return print_and_exit(ERR_ALLOC, PLUG, FALSE);
            ft_memcpy(data->payload, buffer, readed_to_buff); data->payload_len = readed_to_buff;
		}
		else
		{
			uint8_t *remember = data->payload;

			data->payload = (uint8_t*)malloc(sizeof(uint8_t) * (data->payload_len + readed_to_buff + 1));
			if (!data->payload) return print_and_exit(ERR_ALLOC, PLUG, FALSE);
            ft_memcpy(data->payload, remember, data->payload_len); free(remember);
            ft_memcpy(data->payload + data->payload_len, buffer, readed_to_buff);
			data->payload_len += readed_to_buff; data->payload[data->payload_len] = '\0';
		}
	}
	if (readed_to_buff == -1 && fd != 0)
		return print_and_exit(FILE_ABSENT, filename, TRUE);

	data->payload_name = (char*)malloc(sizeof(char) * (ft_strlen(filename) + 1));
	if (!data->payload_name) return print_and_exit(ERR_ALLOC, PLUG, FALSE);
    ft_strcpy(data->payload_name, filename); data->payload_name[ft_strlen(filename)] = '\0';

	if (!data->payload) return AllocatedDataInit(data, is_stdin);
	return TRUE;
}

BOOL ParseHash(int argc, char *argv[], t_env_hash* env)
{
	int i = 0;
	BOOL AYO_FILE_TIME = FALSE;
	while (++i < argc)
	{
		if (!AYO_FILE_TIME && ft_strcmp(argv[i], "-q") == 0) env->flags.q = TRUE;
		else if (!AYO_FILE_TIME && ft_strcmp(argv[i], "-r") == 0) env->flags.r = TRUE;
		else if (!AYO_FILE_TIME && ft_strcmp(argv[i], "-p") == 0)
		{
			env->flags.p = TRUE;
			if (!read_fd(0, "STDIN", env, TRUE)) return FALSE;
		}
		else if (!AYO_FILE_TIME && ft_strcmp(argv[i], "-s") == 0)
		{
			i++;
			if (i == argc) return print_and_exit(NO_STRING_AFTER_FLAG, PLUG, FALSE);
			else
			{
				t_data_hash *data = DataInit(env);

				if (!data)
					return print_and_exit(ERR_ALLOC, PLUG, FALSE);
				if (!StringFlag(data, argv[i]))
					return print_and_exit(ERR_ALLOC, PLUG, FALSE);
			}
		}
		else
		{
			int fd = open(argv[i], O_RDONLY);
			BOOL res = read_fd(fd, argv[i], env, FALSE);
			if (fd > 0) { close(fd); }
			if (!res) { return FALSE; }
			AYO_FILE_TIME = TRUE;
		}
	}
	if (!env->data)
	{
		if (!read_fd(0, "STDIN", env, TRUE)) return FALSE;
	}
	return TRUE;
}
