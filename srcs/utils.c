#include "../includes/utils.h"


static void swap(uint8_t *ptr1, uint8_t *ptr2)
{
	uint8_t temp = *ptr1;
	*ptr1 = *ptr2;
	*ptr2 = temp;
}

void reverse(uint8_t *begin, uint8_t *end)
{
	while (begin < end)
	{
		swap(begin, end);
		end--; begin++;
	}
}

int endian()
{
	uint32_t i = 1;
	uint8_t *p = (uint8_t*)&i;

	if (*p == 1) return _LITTLE_ENDIAN;
	else return _BIG_ENDIAN;
}


size_t get_optimal_length(size_t base_lenght)
{
	size_t length = base_lenght + 1; //default 1

	uint8_t mod = length % 64;
	length += (mod > 56) ? (64 - mod + 54) : (56 - mod);
	length += 8; //is message length

	return length;
}

uint32_t	to_big_endian(uint32_t nb)
{
	return ((nb >> 24 & 0xff) | (nb >> 8 & 0xff00) | (nb << 8 & 0xff0000)
		| (nb << 24 & 0xff000000));
}

uint32_t rotate_left(uint32_t x, uint32_t s)
{
	return (x << s) | (x >> (32 - s));
}
uint32_t rotate_right(uint32_t x, uint32_t s)
{
	return (x >> s) | (x << (32 - s));
}
