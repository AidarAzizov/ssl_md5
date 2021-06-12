#include "../../includes/ft_ssl.h"

#define ROUND_COUNT 64
#define ONE_ROUND_EAT_BYTE 64

static const uint32_t g_T[64] =
{
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};


static const uint32_t g_s[4][4] =
{
	{7, 12, 17, 22},
	{5, 9, 14, 20},
	{4, 11, 16, 23},
	{6, 10, 15, 21}
};



static BOOL AppendPaddingBits(const uint8_t *message, size_t message_len, t_md5 *md5)
{
	uint8_t	*end;

	md5->prepared_message_len = get_optimal_length(message_len);
	md5->prepared_message = (uint8_t*)malloc(sizeof(uint8_t) * md5->prepared_message_len);
	if (!md5->prepared_message) return FALSE;

	ft_memset(md5->prepared_message, 0, md5->prepared_message_len);
	ft_memcpy(md5->prepared_message, message, message_len);
	end = md5->prepared_message + message_len; *end = 0x80;
	end = md5->prepared_message + md5->prepared_message_len - 8;

	uint64_t length = message_len * 8;
	ft_memcpy(end, &length, sizeof(length));

	if (endian() == _BIG_ENDIAN) reverse(end, end + 7);
	return TRUE;
}

static void InitBufferMD5(t_md5 *md5)
{
	uint8_t A[4] = { 0x01, 0x23, 0x45, 0x67 };
	uint8_t B[4] = { 0x89, 0xab, 0xcd, 0xef };
	uint8_t C[4] = { 0xfe, 0xdc, 0xba, 0x98 };
	uint8_t D[4] = { 0x76, 0x54, 0x32, 0x10 };

	md5->A = *(uint32_t *)A;
	md5->B = *(uint32_t *)B;
	md5->C = *(uint32_t *)C;
	md5->D = *(uint32_t *)D;
}


static uint32_t FunF(uint32_t X, uint32_t Y, uint32_t Z)
{
	return (X & Y) | ((~X) & Z);
}
static uint32_t FunG(uint32_t X, uint32_t Y, uint32_t Z)
{
	return (X & Z) | ((~Z) & Y);
}
static uint32_t FunH(uint32_t X, uint32_t Y, uint32_t Z)
{
	return (X ^ Y ^Z);
}
static uint32_t FunI(uint32_t X, uint32_t Y, uint32_t Z)
{
	return (Y ^ ((~Z) | X));
}


void RoundMD5(uint32_t *a, uint32_t b, uint32_t c,
	uint32_t d, uint32_t k, uint32_t s, uint32_t i,
	uint32_t(*func)(uint32_t, uint32_t, uint32_t),
	const uint32_t *block)
{
	*a += func(b, c, d) + block[k] + g_T[i];
	*a = rotate_left(*a, s);
	*a += b;
}


uint32_t K0(uint32_t i) { return i; }
uint32_t K1(uint32_t i) { return (i * 5 + 1) % 16; }
uint32_t K2(uint32_t i) { return (i * 3 + 5) % 16; }
uint32_t K3(uint32_t i) { return (i * 7) % 16; }
uint32_t(*k_ptr[4])(uint32_t) =
{
	K0, K1, K2, K3
};

uint32_t(*Fun_ptr[4])(uint32_t, uint32_t, uint32_t) =
{
	FunF, FunG, FunH, FunI
};


void process_block(t_md5 *md5, uint8_t* adress)
{
	uint32_t	remember_A = md5->A;
	uint32_t	remember_B = md5->B;
	uint32_t	remember_C = md5->C;
	uint32_t	remember_D = md5->D;

	uint32_t	block[16];
	ft_memcpy(block, adress, ONE_ROUND_EAT_BYTE);

	uint8_t		i = 0;
	while (i != ROUND_COUNT)
	{
		RoundMD5(&md5->A, md5->B, md5->C, md5->D, k_ptr[i / 16](i), g_s[i / 16][i % 4], i, Fun_ptr[i / 16], block); i++;
		RoundMD5(&md5->D, md5->A, md5->B, md5->C, k_ptr[i / 16](i), g_s[i / 16][i % 4], i, Fun_ptr[i / 16], block); i++;
		RoundMD5(&md5->C, md5->D, md5->A, md5->B, k_ptr[i / 16](i), g_s[i / 16][i % 4], i, Fun_ptr[i / 16], block); i++;
		RoundMD5(&md5->B, md5->C, md5->D, md5->A, k_ptr[i / 16](i), g_s[i / 16][i % 4], i, Fun_ptr[i / 16], block); i++;
	}
	md5->A += remember_A;
	md5->B += remember_B;
	md5->C += remember_C;
	md5->D += remember_D;
}

void LaunchResolveRound(t_md5 *md5)
{
	uint8_t*	begin = md5->prepared_message;

	int count = md5->prepared_message_len / 64;
	int i = 0;
	while (i < count)
	{
		process_block(md5, begin); i++;
		if (i != count) begin += ONE_ROUND_EAT_BYTE;
	}
}

BOOL	md5(t_env_hash *env)
{
	char		*hex_char = "0123456789abcdef";
	t_data_hash	*head = env->data;

	ft_strcpy(env->hashname_uppercase, "MD5");
    ft_strcpy(env->hashname_lowercase, "md5");
    ft_memset(env->data->hash, '\0', sizeof(env->data->hash));
	while (env->data)
	{
		t_md5	md5;
		if (!env->data->err.was_error)
		{
			if (!AppendPaddingBits(env->data->payload, env->data->payload_len, &md5)) break;

			InitBufferMD5(&md5);
			LaunchResolveRound(&md5);
			free(md5.prepared_message);

			uint8_t		hash[16];
			ft_memset(hash, 0, sizeof(hash));
			ft_memcpy(&hash[0], &md5.A, sizeof(md5.A));
			ft_memcpy(&hash[4], &md5.B, sizeof(md5.B));
			ft_memcpy(&hash[8], &md5.C, sizeof(md5.C));
			ft_memcpy(&hash[12], &md5.D, sizeof(md5.D));
			int	i = 0; int j = 0;
			while (i < 16)
			{
				env->data->hash[j++] = hex_char[hash[i] >> 4];
				env->data->hash[j++] = hex_char[hash[i] & 0x0F];
				i++;
			}
			env->data->hash[MD5_HLEN] = '\0';
		}
		env->data = env->data->next;
	}
	env->data = head;
	return TRUE;
}

