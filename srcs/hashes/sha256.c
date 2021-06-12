#include "../../includes/ft_ssl.h"

static const uint32_t k[64] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};


static void InitBufferSHA256(t_sha256 *sha256)
{
	sha256->h0 = 0x6A09E667;
	sha256->h1 = 0xBB67AE85;
	sha256->h2 = 0x3C6EF372;
	sha256->h3 = 0xA54FF53A;
	sha256->h4 = 0x510E527F;
	sha256->h5 = 0x9B05688C;
	sha256->h6 = 0x1F83D9AB;
	sha256->h7 = 0x5BE0CD19;
}


static BOOL AppendPaddingBits(const uint8_t *message, size_t message_len, t_sha256 *sha256)
{
	uint8_t	*end;

	sha256->prepared_message_len = get_optimal_length(message_len);
	sha256->prepared_message = (uint8_t*)malloc(sizeof(uint8_t) * sha256->prepared_message_len);
	if (!sha256->prepared_message) return FALSE;

    ft_memset(sha256->prepared_message, 0, sha256->prepared_message_len);
    ft_memcpy(sha256->prepared_message, message, message_len);
	end = sha256->prepared_message + message_len; *end = 0x80;
	end = sha256->prepared_message + sha256->prepared_message_len - 8;

	uint64_t length = message_len * 8;
    ft_memcpy(end, &length, sizeof(uint64_t));
	if (endian() == _LITTLE_ENDIAN) reverse(end, end + 7);
	return TRUE;
}

static void WBufferInit(t_sha256 *sha256, uint32_t *w)
{
	int i = -1;
	while (++i < 16)
	{
        ft_memcpy(&w[i], sha256->prepared_message + sha256->offset + (i * 4), sizeof(uint32_t));
		if (endian() == _LITTLE_ENDIAN) w[i] = to_big_endian(w[i]);
	}
	while (i < 64)
	{
		uint32_t tmp = w[i - 15];
		uint32_t s0 = rotate_right(tmp, 7) ^ rotate_right(tmp, 18) ^ (tmp >> 3);
		tmp = w[i - 2];
		uint32_t s1 = rotate_right(tmp, 17) ^ rotate_right(tmp, 19) ^ (tmp >> 10);
		w[i] = w[i - 16] + s0 + w[i - 7] + s1;
		i++;
	}

}

static BOOL LaunchResolveRound(t_sha256 *sha256)
{
	uint32_t w[64]; ft_memset(w, 0, sizeof(w));
	WBufferInit(sha256, w);

	uint32_t a = sha256->h0;
	uint32_t b = sha256->h1;
	uint32_t c = sha256->h2;
	uint32_t d = sha256->h3;
	uint32_t e = sha256->h4;
	uint32_t f = sha256->h5;
	uint32_t g = sha256->h6;
	uint32_t h = sha256->h7;

	int i = -1;
	while (++i < 64)
	{
		uint32_t s1 = rotate_right(e, 6) ^ rotate_right(e, 11) ^ rotate_right(e, 25);
		uint32_t ch = (e & f) ^ ((~e) & g);
		uint32_t temp1 = h + s1 + ch + k[i] + w[i];
		uint32_t s0 = rotate_right(a, 2) ^ rotate_right(a, 13) ^ rotate_right(a, 22);
		uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
		uint32_t temp2 = s0 + maj;
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}
	sha256->h0 += a;
	sha256->h1 += b;
	sha256->h2 += c;
	sha256->h3 += d;
	sha256->h4 += e;
	sha256->h5 += f;
	sha256->h6 += g;
	sha256->h7 += h;
	sha256->offset += 64;
	return (sha256->offset < sha256->prepared_message_len);
}


BOOL sha256(t_env_hash *env)
{
	const char	hex_char[17] = "0123456789abcdef";
	t_data_hash	*head = env->data;
	uint8_t		hash[32];

    ft_strcpy(env->hashname_uppercase, "SHA256");
    ft_strcpy(env->hashname_lowercase, "sha256");
	while (env->data)
	{
		t_sha256	sha256; sha256.offset = 0;
		if (!env->data->err.was_error)
		{
			if (!AppendPaddingBits(env->data->payload, env->data->payload_len, &sha256)) break;

			InitBufferSHA256(&sha256);
			while (LaunchResolveRound(&sha256)) ;
			free(sha256.prepared_message);

			sha256.h0 = to_big_endian(sha256.h0); ft_memcpy(&hash[0], &sha256.h0, sizeof(sha256.h0));
			sha256.h1 = to_big_endian(sha256.h1); ft_memcpy(&hash[4], &sha256.h1, sizeof(sha256.h1));
			sha256.h2 = to_big_endian(sha256.h2); ft_memcpy(&hash[8], &sha256.h2, sizeof(sha256.h2));
			sha256.h3 = to_big_endian(sha256.h3); ft_memcpy(&hash[12], &sha256.h3, sizeof(sha256.h3));
			sha256.h4 = to_big_endian(sha256.h4); ft_memcpy(&hash[16], &sha256.h4, sizeof(sha256.h4));
			sha256.h5 = to_big_endian(sha256.h5); ft_memcpy(&hash[20], &sha256.h5, sizeof(sha256.h5));
			sha256.h6 = to_big_endian(sha256.h6); ft_memcpy(&hash[24], &sha256.h6, sizeof(sha256.h6));
			sha256.h7 = to_big_endian(sha256.h7); ft_memcpy(&hash[28], &sha256.h7, sizeof(sha256.h7));

			int	i = 0, j = 0;
			while (i < 32)
			{
				env->data->hash[j++] = hex_char[hash[i] >> 4];
				env->data->hash[j++] = hex_char[hash[i] & 0x0F];
				i++;
			}
			env->data->hash[SHA256_HLEN] = '\0';
		}
		env->data = env->data->next;
	}
	env->data = head;
	return TRUE;
}