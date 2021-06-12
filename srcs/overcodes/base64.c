#include "../../includes/ft_ssl.h"

#include <stdio.h> //debug


static const char alphabet_of_encoding[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void MakeGroup(t_data_overcode *data, size_t offset)
{
	uint32_t index;
	uint32_t result = (data->payload[offset + 0] << 16)
			| (data->payload[offset + 1] << 8)
			| (data->payload[offset + 2]);

	index = (result >> 18) & 0x3F;
	data->overcode[offset + 0] = alphabet_of_encoding[index];

	index = (result >> 12) & 0x3F;
	data->overcode[offset + 1] = alphabet_of_encoding[index];

	index = (result) & 0x3F;
	data->overcode[offset + 2] = alphabet_of_encoding[index];
}

BOOL base64(t_env_overcode *env)
{
	size_t i = 0;

	env->data->overcode = (char*)malloc(sizeof(char) *
			(env->data->payload_len + (3 - env->data->payload_len % 3) + 1));
	if (!env->data->overcode) return print_and_exit(ERR_ALLOC, PLUG, FALSE);
	while (i < env->data->payload_len)
	{
		MakeGroup(env->data, i);
		i += 3;
	}

	printf("%s\n", env->data->overcode);

	return TRUE;
}
