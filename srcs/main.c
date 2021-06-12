#include "../includes/ft_ssl.h"

void ClearAll(t_env_hash *env)
{
	while (env->data)
	{
		if (env->data->payload)
			free(env->data->payload);
		if (env->data->payload_name)
			free(env->data->payload_name);
		if (env->data->err.error_message)
			free(env->data->err.error_message);

		t_data_hash *remember = env->data;
		env->data = env->data->next;
		free(remember);
	}
}


BOOL LaunchHash(int argc, char *argv[], BOOL (*hash_func)(struct s_env_hash*))
{
	t_env_hash env; env.data = NULL;
	env.flags.q = FALSE; env.flags.r = FALSE; env.flags.p = FALSE;

	while (TRUE)
	{
		if (!ParseHash(--argc, ++argv, &env)) break;
		if (!hash_func(&env)) break;
		PrintHash(&env);
		break;
	}
	ClearAll(&env);
	return TRUE;
}

BOOL LaunchOvercode(int argc, char *argv[], BOOL (*overcode_func)(struct s_env_overcode*))
{
	t_env_overcode env;

	//debug
	env.data = (t_data_overcode*)malloc(sizeof(t_data_overcode));
	env.data->payload_len = 4;
	env.data->payload = (uint8_t*)malloc(sizeof(uint8_t) * env.data->payload_len);
	ft_memset(env.data->payload, 0, env.data->payload_len);
	ft_memcpy(env.data->payload, "demo", env.data->payload_len);
	//debug

	while (TRUE)
	{
		if (!ParseOvercode(--argc, ++argv, &env)) break;
		if (!overcode_func(&env)) break;

		PrintOvercode(&env);

		break;
	}

	return TRUE;
}


int main(int argc, char *argv[])
{
	LaunchOvercode(argc, argv, base64); //debug
	return 0;

	if (argc == 0) return print_and_exit(USAGE, PLUG, FALSE);


	if (ft_strcmp(argv[0], "md5") == 0)LaunchHash(argc, argv, md5);
	else if (ft_strcmp(argv[0], "sha256") == 0)LaunchHash(argc, argv, sha256);
	else if (ft_strcmp(argv[0], "base64") == 0)LaunchOvercode(argc, argv, base64);
	else return print_and_exit(ENCODE_ABSENT, argv[0], FALSE);

	return 0;
}

