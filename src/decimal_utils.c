#include "decimal.h"

//||||||||||||||||||||||| LONG UTILS ||||||||||||||||||||||

void _longd_to_decimal(decimal_long long_dec_number,
                           decimal *dec_number) {
  for (int i = LOW; i <= HIGH; i++)
    dec_number->bits[i] = long_dec_number.bits[i];
  _set_scale(dec_number, long_dec_number.scale);
  _set_sign(dec_number, long_dec_number.sign);
}

decimal_long _decimal_to_longd(decimal *dec_number) {
  decimal_long long_dec_number;
  _init_decimal_long(&long_dec_number);

  for (int i = LOW; i <= HIGH; i++)
    long_dec_number.bits[i] = dec_number->bits[i];
  long_dec_number.scale = _get_scale(*dec_number);
  long_dec_number.sign = _get_sign(*dec_number);

  return long_dec_number;
}

int _get_bit_long(decimal_long dec_number, const int index) {
  return !!(dec_number.bits[index / 32] & (1 << index % 32));
}
void _set_bit_long(decimal_long *dec_number, unsigned index,
                       unsigned bit_to_set) {
  if (bit_to_set == 1)
    dec_number->bits[index / 32] |= (1 << (index % 32));
  else
    dec_number->bits[index / 32] &= ~(1 << index % 32);
}

void _left_shift_long(decimal_long *dec_number) {
  int bit[5] = {0};
  for (int i = 31, j = 0; i <= 159; i += 32, j++)
    bit[j] = _get_bit_long(*dec_number, i);

  for (int i = LOW; i <= 5; i++) dec_number->bits[i] <<= 1;

  for (int i = 32, j = 0; i <= 160; i += 32, j++)
    _set_bit_long(dec_number, i, bit[j]);
}

void _right_shift_long(decimal_long *dec_number) {
  int bit[5] = {0};
  for (int i = 32, j = 0; i <= 160; i += 32, j++)
    bit[j] = _get_bit_long(*dec_number, i);

  for (int i = LOW; i <= 5; i++) dec_number->bits[i] >>= 1;

  for (int i = 31, j = 0; i <= 159; i += 32, j++)
    _set_bit_long(dec_number, i, bit[j]);
}

void _make_zeros_bits_long(decimal_long *dec_number) {
  for (int i = LOW; i < 6; i++) dec_number->bits[i] = 0;
}

void _init_decimal_long(decimal_long *dec_number) {
  _make_zeros_bits_long(dec_number);
  dec_number->scale = 0;
  dec_number->sign = 0;
}

int _compare_sign(const decimal_long value_1,
                      const decimal_long value_2) {
  compare_result output = 0;  // default equal sign
  bool value_1_sign = value_1.sign;
  bool value_2_sign = value_2.sign;
  if (value_1_sign != value_2_sign)
    output = (value_1_sign == MINUS) ? 1 : 2;  // first or second is smaller
  return output;
}

int _all_zero_single(const decimal_long value) {
  compare_result output = NO;
  if (value.bits[LOW] == 0 && value.bits[MID] == 0 && value.bits[HIGH] == 0 &&
      value.bits[3] == 0 && value.bits[4] == 0 && value.bits[5] == 0)
    output = YES;
  return output;
}

int _all_zero(const decimal_long value_1, decimal_long value_2) {
  compare_result output = NO;
  if (_all_zero_single(value_1) && _all_zero_single(value_2))
    output = YES;
  return output;
}

int _check_last_zeros(const decimal_long dec_number) {
  compare_result output = NO;
  if ((_get_bit_long(dec_number, 189) + _get_bit_long(dec_number, 190) +
       _get_bit_long(dec_number, 191)) == 0)
    output = YES;
  return output;
}

void _copy_decimal_long(const decimal_long src,
                            decimal_long *dest) {
  for (int i = LOW; i <= 5; i++) dest->bits[i] = src.bits[i];
  dest->scale = src.scale;
  dest->sign = src.sign;
}

int _last_bit(const decimal_long value) {
  int i = -1;
  if (!_all_zero_single(value)) {
    for (i = 191; i >= 0; i--)
      if (_get_bit_long(value, i)) break;
  }
  return i;
}

int _check_overflow(decimal_long *value) {
  compare_result output = NO;
  if ((value->bits[3] + value->bits[4] + value->bits[5]) ||
      (value->scale > 28)) {
    output = YES;
  }
  return output;
}

//||||||||||||||||||||||| \LONG UTILS ||||||||||||||||||||||

//|||||||||||||||||||||| STANDARD UTILS ||||||||||||||||||||

int _get_bit(decimal dec_number, const int index) {
  return !!(dec_number.bits[index / 32] & (1 << index % 32));
}

void _set_bit(decimal *dec_number, unsigned index,
                  unsigned bit_to_set) {
  if (bit_to_set == 1)
    dec_number->bits[index / 32] |= (1 << (index % 32));
  else
    dec_number->bits[index / 32] &= ~(1 << index % 32);
}

bool _get_sign(decimal dec_number) {
  return !!(dec_number.bits[SCALE] & (2147483648));
}

void _set_sign(decimal *dec_number, int sign) {
  _set_bit(dec_number, 127, sign);
}

void _init_decimal(decimal *dec_number) {
  for (int i = LOW; i <= SCALE; i++) dec_number->bits[i] = 0;
}

void _copy_decimal(decimal src, decimal *dest) {
  for (int i = LOW; i <= SCALE; i++) dest->bits[i] = src.bits[i];
}

int _get_scale(decimal dec_number) {
  return (dec_number.bits[SCALE] << 1) >> 17;
}

void _set_scale(decimal *dec_number, unsigned scale) {
  int sign = 0;
  if (_get_bit(*dec_number, 127) == 1) sign = 1;
  dec_number->bits[SCALE] = 0;
  dec_number->bits[SCALE] |= (scale << 16);
  _set_bit(dec_number, 127, sign);
}

int _dec_zero(const decimal value) {
  compare_result output = NO;
  if (!(value.bits[LOW] + value.bits[MID] + value.bits[HIGH])) output = YES;
  return output;
}

int _check_dec_scale(const decimal value_1, const decimal value_2) {
  compare_result output = NO;
  if (_get_scale(value_1) <= 28 && _get_scale(value_2) <= 28)
    output = YES;
  return output;
}

int _compare_max_num(const decimal value_1, const decimal value_2,
                         decimal *result, compare_result *fill_res) {
  arithmetics_result output = OK;
  const decimal max_big = {{-1, -1, -1, 0}};
  const decimal min_big = {{-1, -1, -1, -2147483648}};
  const decimal five = {{5, 0, 0, 65536}};
  const decimal m_five = {{5, 0, 0, -2147418112}};
  const decimal one = {{1, 0, 0, 0}};
  if (is_equal(value_1, max_big)) {
    if (is_greater_or_equal(value_2, five)) {
      output = INF;
    } else if (is_less(value_2, one)) {
      _copy_decimal(value_1, result);
      *fill_res = YES;
    }
  } else if (is_equal(value_1, min_big)) {
    if (is_less_or_equal(value_2, m_five)) {
      output = NEG_INF;
    } else if (is_less(value_2, five)) {
      _copy_decimal(value_1, result);
      *fill_res = YES;
    }
  }
  return output;
}

int _compare_min_num(const decimal value_1, const decimal value_2,
                         decimal *result, compare_result *fill_res) {
  arithmetics_result output = OK;
  const decimal max_big = {{-1, -1, -1, 0}};
  const decimal min_big = {{-1, -1, -1, -2147483648}};
  const decimal five = {{5, 0, 0, 65536}};
  const decimal m_five = {{5, 0, 0, -2147418112}};
  if (is_equal(value_1, max_big)) {
    if (is_less_or_equal(value_2, m_five)) {
      output = INF;
    } else {
      _copy_decimal(value_1, result);
      *fill_res = YES;
    }
  } else if (is_equal(value_1, min_big)) {
    if (is_greater_or_equal(value_2, five)) {
      output = NEG_INF;
    } else {
      _copy_decimal(value_1, result);
      *fill_res = YES;
    }
  } else if (is_equal(value_2, max_big)) {
    if (is_greater_or_equal(value_1, five) ||
        is_less_or_equal(value_1, m_five)) {
      output = NEG_INF;
    } else {
      _copy_decimal(value_2, result);
      *fill_res = YES;
    }
  } else if (is_equal(value_2, min_big)) {
    if (is_greater_or_equal(value_1, five)) {
      output = INF;
    } else {
      _copy_decimal(value_2, result);
      *fill_res = YES;
    }
  }
  return output;
}

int _compare_min_num_fill(const decimal value_1,
                              const decimal value_2, decimal *result,
                              compare_result *fill_res) {
  arithmetics_result output = OK;
  const decimal max_big = {{-1, -1, -1, 0}};
  const decimal min_big = {{-1, -1, -1, -2147483648}};
  const decimal five = {{5, 0, 0, 65536}};
  const decimal m_five = {{5, 0, 0, -2147418112}};
  if (is_equal(value_1, max_big) && is_less(value_2, five)) {
    _copy_decimal(value_1, result);
    *fill_res = YES;
  } else if (is_equal(value_1, min_big) &&
             is_greater(value_2, m_five)) {
    _copy_decimal(value_1, result);
    *fill_res = YES;
  } else if (is_equal(value_2, min_big) &&
             is_greater(value_1, m_five)) {
    _copy_decimal(value_2, result);
    *fill_res = YES;
  }
  return output;
}

int _check_big_num(const decimal value_1, const decimal value_2,
                       decimal *result, compare_result *fill_res,
                       const int sub) {
  const int sign1 = _get_sign(value_1);
  const int sign2 = _get_sign(value_2);
  arithmetics_result output = OK;
  if (sub && sign1 != sign2) {
    output = _compare_min_num(value_1, value_2, result, fill_res);
  } else if (sub) {
    output = _compare_min_num_fill(value_1, value_2, result, fill_res);
  } else {
    output = _compare_max_num(value_1, value_2, result, fill_res);
    if (!output && !(*fill_res))
      output = _compare_max_num(value_2, value_1, result, fill_res);
  }
  return output;
}

int _parse_float(float src, int *scale) {
  char exp_str[20] = {0}, input_num_str[20] = {0};
  input_num_str[0] = '0';
  int output_number, exp;
  char num_part_1, num_part_2[7];

  sprintf(exp_str, "%.6E", src);
  sscanf(exp_str, "%c.%6sE%d", &num_part_1, num_part_2, &exp);
  sprintf(input_num_str, "%c%s", num_part_1, num_part_2);
  *scale = strlen(input_num_str) - exp - 1;

  for (int i = strlen(input_num_str) - 1; i >= 0; --i) {
    if (input_num_str[i] == '0' && i != 0) {
      input_num_str[i] = '\0';
      --(*scale);
    } else {
      break;
    }
  }
  sscanf(input_num_str, "%d", &output_number);
  return output_number;
}

//|||||||||||||||||||||| \STANDARD UTILS ||||||||||||||||||||
