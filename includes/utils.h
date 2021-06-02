#ifndef UTILS_H
# define UTILS_H


#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include "boolean.h"

#define ERR_ALLOC 0
#define ENCODE_ABSENT 1
#define FILE_ABSENT 2
#define USAGE 3
#define NO_STRING_AFTER_FLAG 4

#define _LITTLE_ENDIAN 0
#define _BIG_ENDIAN    1

#define PLUG ""

int endian();

void reverse(uint8_t *begin, uint8_t *end);

uint32_t to_big_endian(uint32_t nb);

size_t get_optimal_length(size_t base_lenght);

uint32_t rotate_left(uint32_t x, uint32_t s);

uint32_t rotate_right(uint32_t x, uint32_t s);

BOOL print_and_exit(int error_code, const char *what_called, BOOL return_code);


#endif
