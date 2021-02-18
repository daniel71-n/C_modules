#ifndef BIT_UTILS_H
#define BIT_UTILS_H


#include <stdint.h>
typedef enum signed_or_unsigned{
    SIGNED, UNSIGNED
} sign;

// get a string representation of the bits of some number - e.g. '101101110'
void get_binary_string(char *holding_string, long long number_to_convert);
uint8_t Count_bits(long long num);
long long Reverse_bits(long long num);


#endif
