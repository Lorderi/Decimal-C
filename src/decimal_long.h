#if !defined(DECIMAL_LONG_H)
#define DECIMAL_LONG_H

#include "decimal.h"

//  rescale

int _rescale(decimal_long *value_1, decimal_long *value_2);
int _check_last_zeros(decimal_long dec_number);
int _divide_to_rescale(decimal_long *dec_number, int start_point);
int _multiply_to_rescale(decimal_long *dec_number);
//  round

int _bank_round(decimal_long *long_result, int sign1);
int _simple_round(decimal_long *long_result, int mod);
//  compare

int _is_greater_or_equal_long(decimal_long long_value_1,
                                  decimal_long long_value_2);
int _is_greater_long(decimal_long long_value_1,
                         decimal_long long_value_2);
int _is_equal_long(decimal_long long_value_1,
                       decimal_long long_value_2);
//  arithmetic

int _div_long(decimal_long long_value_1, decimal_long long_value_2,
                  decimal_long *long_result);
int _sub_long(decimal_long long_value_1, decimal_long long_value_2,
                  decimal_long *long_result, compare_result *change_sign);
int _bit_add(const decimal_long value_1, const decimal_long value_2,
                 decimal_long *result);
int _bit_sub(const decimal_long value_1, const decimal_long value_2,
                 decimal_long *result);
void _bit_div(decimal_long dividend, decimal_long divisor,
                  decimal_long *result, decimal_long *my_remainder);
int _bit_mul(decimal_long long_value_1,
                 const decimal_long long_value_2,
                 decimal_long *long_result);
//  division

void _local_divide(const decimal_long value,
                       decimal_long *sub_value, const int start_bit);
void _copy_sub_to_dividend(const decimal_long sub_value,
                               decimal_long *value, const int start_bit);
void _shift_to_normalize(decimal_long *value, const int end);
int _multiply_part(decimal_long *dividend, decimal_long *divisor,
                       decimal_long *real_result, int *scale,
                       int do_add_apply);
int _real_div_part(decimal_long my_remainder,
                       decimal_long long_value_2,
                       decimal_long *long_result, int *mod,
                       int *result_scale, int overflow);

#endif  // DECIMAL_LONG_H
