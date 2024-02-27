#if !defined(DECIMAL_UTILS_H)
#define DECIMAL_UTILS_H

#include "decimal.h"

//  for tests
#define MAX_UINT 4294967295
#define EXPONENT_MINUS_1 2147549184   // 0b10000000000000010000000000000000
#define EXPONENT_PLUS_1 65536         // 0b00000000000000010000000000000000
#define EXPONENT_PLUS_2 196608        // 0b00000000000000110000000000000000
#define EXPONENT_MINUS_28 2149318656  // 0b10000000000111000000000000000000
#define EXPONENT_PLUS_28 1835008      // 0b00000000000111000000000000000000

//  standart

int _get_bit(decimal dec_number, const int index);
void _set_bit(decimal *dec_number, unsigned bit_number,
                  unsigned bit_to_set);
int _get_scale(decimal dec_number);
void _set_scale(decimal *dst, unsigned scale);
bool _get_sign(decimal dec_number);
void _set_sign(decimal *dec_number, int sign);
void _copy_decimal(decimal src, decimal *dest);
void _init_decimal(decimal *dec_number);
int _dec_zero(decimal value);
int _check_dec_scale(const decimal value_1, const decimal value_2);
//  long

void _make_zeros_bits_long(decimal_long *dec_number);
int _compare_sign(decimal_long value_1, decimal_long value_2);
int _all_zero_single(decimal_long value);
int _all_zero(decimal_long value_1, decimal_long value_2);
int _last_bit(decimal_long value);
void _set_bit_long(decimal_long *dec_number, unsigned index,
                       unsigned bit_to_set);
void _left_shift_long(decimal_long *dec_number);
void _right_shift_long(decimal_long *dec_number);
void _init_decimal_long(decimal_long *dec_number);
int _get_bit_long(decimal_long dec_number, const int index);
void _longd_to_decimal(decimal_long long_dec_number,
                           decimal *dec_number);
void _copy_decimal_long(decimal_long src, decimal_long *dest);
int _check_big_num(decimal value_1, decimal value_2,
                       decimal *result, compare_result *fill_res,
                       const int sub);
decimal_long _decimal_to_longd(decimal *dec_number);
int _check_overflow(decimal_long *value);
int _parse_float(float src, int *scale);

#endif  // DECIMAL_UTILS_H
