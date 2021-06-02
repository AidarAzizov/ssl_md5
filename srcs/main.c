#include "../includes/ft_ssl.h"

void ClearAll(t_env *env)
{
	while (env->data)
	{
		if (env->data->payload)
			free(env->data->payload);
		if (env->data->payload_name)
			free(env->data->payload_name);
		if (env->data->err.error_message)
			free(env->data->err.error_message);

		t_data *remember = env->data;
		env->data = env->data->next;
		free(remember);
	}
}

int main(int argc, char *argv[])
{
	t_env env; env.data = NULL;
	env.flags.q = FALSE; env.flags.r = FALSE; env.flags.p = FALSE;

	while (TRUE)
	{
		if (!parse(--argc, ++argv, &env)) break;
		if (!env.hash_func(&env)) break;
		PrintHashes(&env);
		break;
	}
	ClearAll(&env);
	return 0;
}

