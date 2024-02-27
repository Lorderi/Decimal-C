#include "decimal.h"

//||||||||||||||||||||||||||||| RESCALE |||||||||||||||||||||||||||||

int _can_be_mul(decimal_long* dec_number);
int _higher_scale(decimal_long* value_max_scale,
                      decimal_long* value_min_scale, int scale_difference);
int _add_to_multiply(decimal_long value_1, decimal_long value_2,
                         decimal_long* result);

//  must have in argument actual scale arguments!!
int _rescale(decimal_long* value_1, decimal_long* value_2) {
  compare_result divide = YES;
  int scale_1 = value_1->scale;
  int scale_2 = value_2->scale;
  decimal_long *value_min_scale = NULL, *value_max_scale = NULL;
  int max_scale = (scale_1 >= scale_2) ? scale_1 : scale_2;
  int scale_difference = abs(scale_1 - scale_2);

  if (max_scale == scale_2) {
    value_min_scale = value_1;
    value_max_scale = value_2;
  } else {
    value_min_scale = value_2;
    value_max_scale = value_1;
  }

  if (_higher_scale(value_max_scale, value_min_scale, scale_difference) ==
      SUCCESS) {
    int i = 0;
    for (; i < scale_difference; i++) {
      divide = _multiply_to_rescale(value_min_scale);
      if (divide == YES)  // overflow => divide
        break;
      value_min_scale->scale += 1;
    }
    scale_difference -= i;
  }
  if (divide == YES) {
    for (int i = 0; i < scale_difference; i++)
      _divide_to_rescale(value_max_scale, 5);
  }
  return max_scale;
}

// checks if scale can be higher
int _higher_scale(decimal_long* value_max_scale,
                      decimal_long* value_min_scale, int scale_difference) {
  operations_result output = ERROR;
  int can_be_mul = _can_be_mul(value_min_scale);
  int min_scale = value_min_scale->scale;

  if (_check_last_zeros(*value_max_scale) == YES &&
      (min_scale + scale_difference) <= 28 && can_be_mul == YES)
    output = SUCCESS;

  return output;
}

// checks if number can be multiplied
int _can_be_mul(decimal_long* dec_number) {
  compare_result output = NO;
  //  32 * 6 = 192 max
  if (_get_bit_long(*dec_number, 189) == NO &&
      _get_bit_long(*dec_number, 190) == NO &&
      _get_bit_long(*dec_number, 191) == NO) {
    decimal_long dec_number_1 = *dec_number;
    for (int i = 0; i < 3; i++)  // mul by 8
      _left_shift_long(&dec_number_1);

    decimal_long dec_number_2 = *dec_number;
    _left_shift_long(&dec_number_2);  // mul by 2

    decimal_long result;
    _init_decimal_long(&result);

    if (_add_to_multiply(dec_number_1, dec_number_1, &result) == NO) {
      output = YES;
    }
  }
  return output;
}

// multiplying by 10 (making number *2 and number * 8 for adding)
int _multiply_to_rescale(decimal_long* dec_number) {
  decimal_long dec_number_1 = *dec_number;
  for (int i = 0; i < 3; i++)  // mul by 8
    _left_shift_long(&dec_number_1);

  decimal_long dec_number_2 = *dec_number;
  _left_shift_long(&dec_number_2);  // mul by 2

  int output = _add_to_multiply(dec_number_1, dec_number_2, dec_number);
  return output;
}

// actual multiplying by 10 (number * 2 + number * 8)
int _add_to_multiply(decimal_long value_1, decimal_long value_2,
                         decimal_long* result) {
  compare_result overflow = NO;
  _make_zeros_bits_long(result);

  _bit_add(value_1, value_2, result);
  if (!_check_last_zeros(*result)) {
    overflow = YES;
  }
  return overflow;
}

// dividing by 10
int _divide_to_rescale(decimal_long* dec_number,
                           const int start_point) {
  unsigned long mod = 0;
  for (int i = start_point; i >= LOW; i--) {
    unsigned long first = mod << 32;
    unsigned int second = dec_number->bits[i];
    unsigned long tmp = first | second;  // first or second (OR)
    mod = tmp % 10u;
    dec_number->bits[i] = tmp / 10u;
  }
  dec_number->scale -= 1;

  return mod;
}

//||||||||||||||||||||||||||||| \RESCALE |||||||||||||||||||||||||||||

//||||||||||||||||||||||||||   ROUND FUNCS     |||||||||||||||||||||||

// standart round in div
int _round_adding(decimal_long* long_result) {
  compare_result overflow = NO;
  decimal_long round_result, one_value;
  _init_decimal_long(&round_result);
  _init_decimal_long(&one_value);
  one_value.bits[0] = 1;
  one_value.scale = long_result->scale;
  one_value.sign = long_result->sign;
  round_result.scale = long_result->scale;
  round_result.sign = long_result->sign;
  overflow = _bit_add(*long_result, one_value, &round_result);
  *long_result = round_result;

  return overflow;
}

//  if bit overflow and no scale, check values scales
int _check_infinity(decimal_long long_result, const int sign_1) {
  arithmetics_result res = OK;
  if (long_result.scale == NO && !sign_1)
    res = INF;  //  if overflow, no scale, and plus
  else if (long_result.scale == NO && sign_1)
    res = NEG_INF;  //  if overflow, no scale, and minus
  return res;
}

int _round_dividing(decimal_long* long_result, int* mod, const int sign,
                        compare_result overflow) {
  arithmetics_result rtrn_value = OK;
  for (int i = 0; overflow; i++) {
    rtrn_value = _check_infinity(*long_result, sign);
    if (rtrn_value)
      break;
    else if (i == 28 * 2 - 1) {
      rtrn_value = (sign) ? NEG_INF : INF;
      break;
    }
    //  if overflow, scale > 0
    *mod = _divide_to_rescale(long_result, 5);
    overflow = _check_overflow(long_result);
  }
  return rtrn_value;
}

int _bank_round_adding(decimal_long* long_result, const int mod) {
  compare_result overflow = NO;
  //  last bit - uneven
  if (mod >= 5 && (long_result->scale > 0 || long_result->bits[0] & 1))
    overflow = _round_adding(long_result);

  return overflow;
}

//  round with bank, in add, sub
int _bank_round(decimal_long* long_result, const int sign_1) {
  compare_result overflow = YES;
  arithmetics_result rtrn_value = OK;
  int mod = 0;
  rtrn_value = _round_dividing(long_result, &mod, sign_1, overflow);

  if (!rtrn_value) rtrn_value = _bank_round_adding(long_result, mod);
  return rtrn_value;
}

// standart round in div
int _simple_round_adding(decimal_long* long_result, const int mod) {
  compare_result overflow = NO;
  //  if first bit, dig is odd
  if (mod >= 5) overflow = _round_adding(long_result);

  return overflow;
}

void divide_zeroes(decimal_long* long_result) {
  decimal_long copy_long_result;
  int mod = 0;
  while (!mod && long_result->scale >= 0) {
    _copy_decimal_long(*long_result, &copy_long_result);
    mod = _divide_to_rescale(&copy_long_result, 5);
    if (!mod) _divide_to_rescale(long_result, 5);
  }
}

int _simple_round(decimal_long* long_result, int mod) {
  arithmetics_result rtrn_value = OK;
  compare_result overflow = NO;
  overflow = _check_overflow(long_result);
  rtrn_value =
      _round_dividing(long_result, &mod, long_result->sign, overflow);
  if (!rtrn_value) {
    rtrn_value = _simple_round_adding(long_result, mod);
    divide_zeroes(long_result);
  }
  return rtrn_value;
}

//||||||||||||||||||||||||||   \ROUND FUNCS  ||||||||||||||||||||||||

//||||||||||||||||||||||||||||| COMPARE ||||||||||||||||||||||||||||||

int _cmp_bits_ident_scale(decimal_long value_1,
                              decimal_long value_2) {
  int output = 0;  // default equal
  for (int i = 5; i >= LOW; i--) {
    unsigned int bit_value_1 = value_1.bits[i];
    unsigned int bit_value_2 = value_2.bits[i];
    if (bit_value_1 != bit_value_2) {
      output = (bit_value_1 > bit_value_2) ? 1 : 2;  // 1 or 2 is greater
      break;
    }
  }
  return output;
}
int _is_equal_long(decimal_long long_value_1,
                       decimal_long long_value_2) {
  compare_result output = YES;
  compare_result zero = NO;
  if (_all_zero(long_value_1, long_value_2) == YES)
    zero = YES;
  else if (_compare_sign(long_value_1, long_value_2) != 0)
    output = NO;

  if (zero == NO && output != NO) {
    if (((long_value_1.scale != long_value_2.scale)))
      _rescale(&long_value_1, &long_value_2);
    output =
        (_cmp_bits_ident_scale(long_value_1, long_value_2) == 0) ? YES : NO;
  }
  return output;
}

int _is_greater_long(decimal_long long_value_1,
                         decimal_long long_value_2) {
  compare_result output = YES;
  int sign = _compare_sign(long_value_1, long_value_2);
  if (_all_zero(long_value_1, long_value_2) == YES)
    output = NO;
  else if (sign != PLUS)
    output = (sign == MINUS) ? NO : YES;
  else if (_is_equal_long(long_value_1, long_value_2) == YES)
    output = NO;
  else {
    if (long_value_1.scale != long_value_2.scale)
      _rescale(&long_value_1, &long_value_2);
    output =
        (_cmp_bits_ident_scale(long_value_1, long_value_2) == 1) ? YES : NO;
    if (long_value_1.sign) output = !output;
  }
  return output;
}

int _is_greater_or_equal_long(decimal_long long_value_1,
                                  decimal_long long_value_2) {
  compare_result output = NO;
  if (_is_greater_long(long_value_1, long_value_2) == YES ||
      _is_equal_long(long_value_1, long_value_2) == YES)
    output = YES;
  return output;
}

//||||||||||||||||||||||||||||| \COMPARE |||||||||||||||||||||||||||||

//||||||||||||||||||||||||||| ARITHMETIC |||||||||||||||||||||||||||||

int _bit_add(const decimal_long value_1, const decimal_long value_2,
                 decimal_long* result) {
  operations_result rtrn_value = SUCCESS;
  int exp = 0;
  for (int i = 0; i < 192; i += 1) {
    int bitValue_1 = _get_bit_long(value_1, i);
    int bitValue_2 = _get_bit_long(value_2, i);
    if (!bitValue_1 && !bitValue_2)
      exp ? _set_bit_long(result, i, (exp = 0) + 1)
          : _set_bit_long(result, i, 0);
    else if (bitValue_1 != bitValue_2)
      exp ? _set_bit_long(result, i, (exp = 1) - 1)
          : _set_bit_long(result, i, 1);
    else
      exp ? _set_bit_long(result, i, (exp = 1))
          : _set_bit_long(result, i, (exp = 1) - 1);
  }
  //  if overlow
  if (result->bits[3] + result->bits[4] + result->bits[5]) rtrn_value = ERROR;
  result->scale = value_1.scale;

  return rtrn_value;
}
/*
 *  value_1 > value_2
 *  no check for overflow cuz use add if different sign
 *  overflow numbers dont used
 */
int _bit_sub(const decimal_long value_1, const decimal_long value_2,
                 decimal_long* result) {
  operations_result rtrn_value = SUCCESS;
  int exp = 0;

  for (int i = 0; i < 192; i += 1) {
    int bitValue_1 = _get_bit_long(value_1, i);
    int bitValue_2 = _get_bit_long(value_2, i);
    int res_value = bitValue_1 ^ bitValue_2 ^ exp;
    _set_bit_long(result, i, res_value);

    if (bitValue_1 == 0 && bitValue_2 == 1)
      exp = 1;
    else if (bitValue_1 == 1 && bitValue_2 == 0)
      exp = 0;
  }

  if (result->bits[3] + result->bits[4] + result->bits[5]) rtrn_value = ERROR;

  return rtrn_value;
}

int _sub_long(decimal_long long_value_1, decimal_long long_value_2,
                  decimal_long* long_result, compare_result* change_sign) {
  arithmetics_result rtrn_value = OK;
  _init_decimal_long(long_result);
  _rescale(&long_value_1, &long_value_2);

  if (!_is_equal_long(long_value_1, long_value_2)) {
    long_result->scale = long_value_1.scale;
    long_result->sign = long_value_1.sign;

    long_value_1.sign = PLUS;
    long_value_2.sign = PLUS;

    const int sign_1 = long_value_1.sign;

    _rescale(&long_value_1, &long_value_2);
    compare_result overflow = NO;

    if (!_is_greater_or_equal_long(long_value_1, long_value_2)) {
      overflow = _bit_sub(long_value_2, long_value_1, long_result);
      *change_sign = YES;
    } else {
      overflow = _bit_sub(long_value_1, long_value_2, long_result);
    }

    if (overflow) rtrn_value = _bank_round(long_result, sign_1);
  }

  return rtrn_value;
}
/*
 *    *1000
 *     0100
 *     ----
 *     0000 - shift
 *    0000 - shift
 *   1000 - add + shift
 */
int _bit_mul(decimal_long long_value_1,
                 const decimal_long long_value_2,
                 decimal_long* long_result) {
  compare_result overflow = NO;
  for (int i = 0; i < 192; i++) {
    if (_get_bit_long(long_value_2, i)) {
      overflow = _bit_add(*long_result, long_value_1, long_result);
    }
    _left_shift_long(&long_value_1);
  }

  long_result->sign = (_compare_sign(long_value_1, long_value_2)) ? 1 : 0;
  long_result->scale = long_value_1.scale + long_value_2.scale;

  return overflow;
}

//||||||||||||||||||||||||||| \ARITHMETIC |||||||||||||||||||||||||||

//|||||||||||||||||||||   DIVISION FUNCS     |||||||||||||||||||||

/*
 *  copy from start_bit to last_bit
 *  101 / 11
 *  11 - 11 > 10 - local_divide
 *   0
 *  101 - local_divide
 *   11
 *   10 - remainder
 *
 *  local_divide = 101 if (start_bit = 1) ->> local_divide = 10
 */
void _local_divide(const decimal_long value,
                       decimal_long* sub_value, const int start_bit) {
  int last_bit = _last_bit(value);
  decimal_long new_value;
  _init_decimal_long(&new_value);

  if (!_all_zero_single(value)) {
    for (int val_i = start_bit, sub_i = 0; val_i <= last_bit; val_i++, sub_i++)
      _set_bit_long(&new_value, sub_i, _get_bit_long(value, val_i));
  }
  *sub_value = new_value;
}

/*
 *  copy result of sub to dividend
 *  101 / 11 = 01
 *
 *  101 / 11
 *  11 - 11 > 10
 *   0
 *  101
 *   11
 *   10 - remainder
 *  now start value not 101, its 10
 *
 *  dividend = 101 -> dividend = 010
 */
void _copy_sub_to_dividend(const decimal_long sub_value,
                               decimal_long* value, const int start_bit) {
  int last_bit = _last_bit(*value);
  decimal_long new_value;
  _copy_decimal_long(*value, &new_value);

  for (int val_i = start_bit, sub_i = 0; val_i <= last_bit; val_i++, sub_i++)
    _set_bit_long(&new_value, val_i, _get_bit_long(sub_value, sub_i));

  *value = new_value;
}

//  result sets at the end of decimal_long, then shifting
void _shift_to_normalize(decimal_long* value, const int end) {
  for (int i = 0; i < end; i++) _right_shift_long(value);
}

/*
 *  division of two decimal_long
 *  use my_remainder for division of remainder and
 *  adding to integer of division
 *
 *  3 / 2 = 1 + remainder 1 / 2 = 1,5
 *
 *  my_remainder is a sub part of division
 *  101 / 11
 *  11 - 11 > 10
 *   0 - it is an my_remainder
 *
 *  101 is dynamic dividend (its edits while all bit_div)
 */
void _bit_div(decimal_long dividend, decimal_long divisor,
                  decimal_long* result, decimal_long* my_remainder) {
  decimal_long part_dividend;
  _init_decimal_long(result);

  const int my_remainder_scale = dividend.scale;

  dividend.scale = 0;
  divisor.scale = 0;
  _copy_decimal_long(dividend, &part_dividend);

  int start_point = _last_bit(dividend) - _last_bit(divisor);

  int i = 191;
  int counter = 0;

  while (start_point >= 0) {
    _local_divide(dividend, &part_dividend, start_point);

    _rescale(&divisor, &part_dividend);

    if (_is_greater_long(divisor, part_dividend)) {
      counter += 1;
      start_point -= 1;
      if (start_point == -1) *my_remainder = part_dividend;
      if (counter > 1) _set_bit_long(result, i--, 0);
    } else {
      counter = 0;
      _bit_sub(part_dividend, divisor, my_remainder);
      _copy_sub_to_dividend(*my_remainder, &dividend, start_point);
      _set_bit_long(result, i--, 1);
    }
  }

  my_remainder->scale = my_remainder_scale;
  _shift_to_normalize(result, i + 1);
}
/*
 *  normalize if dividend < divisor
 *  do_add_apply flag for more apply
 *  1 if do, 0 if no
 */
int _multiply_part(decimal_long* dividend, decimal_long* divisor,
                       decimal_long* real_result, int* scale,
                       int do_add_apply) {
  compare_result overflow = NO;
  int i = 0;

  _rescale(dividend, divisor);

  while (!overflow && (!_is_greater_or_equal_long(*dividend, *divisor))) {
    overflow = _multiply_to_rescale(dividend);
    *scale += 1;
    i++;
  }
  while (do_add_apply && (i > 1)) {
    overflow = _multiply_to_rescale(real_result);
    i--;
  }
  return overflow;
}

int _real_div_part(decimal_long my_remainder,
                       decimal_long long_value_2,
                       decimal_long* long_result, int* mod,
                       int* result_scale, int overflow) {
  decimal_long remainder_result, copy_long_result;
  _init_decimal_long(&remainder_result);
  _copy_decimal_long(*long_result, &copy_long_result);

  int copy_result_scale = *result_scale;

  while (!overflow && (*result_scale <= 28) &&
         !_all_zero_single(my_remainder)) {
    _multiply_part(&my_remainder, &long_value_2, &copy_long_result,
                       &copy_result_scale, 1);
    _bit_div(my_remainder, long_value_2, &remainder_result, &my_remainder);
    //  leading bit, high bit
    _multiply_to_rescale(&copy_long_result);
    _bit_add(copy_long_result, remainder_result, &copy_long_result);

    if ((overflow = _check_overflow(&copy_long_result)) > 0) {
      *mod = _divide_to_rescale(&copy_long_result, 5);
      break;
    } else {
      _copy_decimal_long(copy_long_result, long_result);
      *result_scale = copy_result_scale;
    }
  }
  return overflow;
}

int _div_long(decimal_long long_value_1, decimal_long long_value_2,
                  decimal_long* long_result) {
  compare_result overflow = NO;
  arithmetics_result rtrn_value = OK;
  _init_decimal_long(long_result);
  int result_sign = (long_value_1.sign == long_value_2.sign) ? PLUS : MINUS;

  decimal_long my_remainder;
  _init_decimal_long(&my_remainder);

  int mod = 0, result_scale = 0;
  long_value_1.sign = PLUS;
  long_value_2.sign = PLUS;
  _rescale(&long_value_1, &long_value_2);

  if (_is_equal_long(long_value_1, long_value_2)) {
    long_result->bits[0] = 1;
  } else {
    //  integer part of result
    overflow = _multiply_part(&long_value_1, &long_value_2, long_result,
                                  &result_scale, 0);
    _bit_div(long_value_1, long_value_2, long_result, &my_remainder);
    overflow = _real_div_part(my_remainder, long_value_2, long_result, &mod,
                                  &result_scale, overflow);
  }
  long_result->scale = result_scale;
  long_result->sign = result_sign;
  if (overflow || long_result->scale >= 28)
    rtrn_value = _simple_round(long_result, mod);

  return rtrn_value;
}

//|||||||||||||||||||||   DIVISION FUNCS     |||||||||||||||||||||
