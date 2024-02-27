#include "decimal.h"

//||||||||||||||||||||||||||| ARITHMETIC |||||||||||||||||||||||||||

int _sub(decimal value_1, decimal value_2, decimal *result,
             const int check);
int _add(decimal value_1, decimal value_2, decimal *result,
             const int check);

int add(decimal value_1, decimal value_2, decimal *result) {
  return _add(value_1, value_2, result, 1);
}

int sub(decimal value_1, decimal value_2, decimal *result) {
  return _sub(value_1, value_2, result, 1);
}

int _add(decimal value_1, decimal value_2, decimal *result,
             const int check) {
  arithmetics_result rtrn_value = OK;
  const int sign_1 = _get_sign(value_1);
  const int sign_2 = _get_sign(value_2);
  _init_decimal(result);
  compare_result fill_res = 0;

  if (check)
    rtrn_value = _check_big_num(value_1, value_2, result, &fill_res, 0);

  if (rtrn_value == OK && !fill_res) {
    if (sign_1 == sign_2) {
      decimal_long long_result,
          long_value_1 = _decimal_to_longd(&value_1),
          long_value_2 = _decimal_to_longd(&value_2);
      _init_decimal_long(&long_result);

      _rescale(&long_value_1, &long_value_2);

      compare_result overflow =
          _bit_add(long_value_1, long_value_2, &long_result);

      long_result.scale = long_value_1.scale;
      long_result.sign = long_value_1.sign;

      if (overflow) rtrn_value = _bank_round(&long_result, sign_1);
      _longd_to_decimal(long_result, result);

    } else if (sign_1 && !sign_2) {
      _set_sign(&value_1, PLUS);
      rtrn_value = sub(value_2, value_1, result);

    } else if (!sign_1 && sign_2) {
      _set_sign(&value_2, PLUS);
      rtrn_value = sub(value_1, value_2, result);
    }
  }

  if (rtrn_value) _init_decimal(result);

  return rtrn_value;
}

int _sub(decimal value_1, decimal value_2, decimal *result,
             const int check) {
  arithmetics_result rtrn_value = OK;
  const int sign_1 = _get_sign(value_1);
  const int sign_2 = _get_sign(value_2);
  _init_decimal(result);
  compare_result fill_res = NO;

  if (check)
    rtrn_value = _check_big_num(value_1, value_2, result, &fill_res, 1);

  if (rtrn_value == OK && !fill_res) {
    if (sign_1 != sign_2) {
      _set_sign(&value_1, PLUS);
      _set_sign(&value_2, PLUS);
      rtrn_value = _add(value_1, value_2, result, 0);
      rtrn_value = rtrn_value ? (sign_1 ? NEG_INF : INF) : rtrn_value;
      _set_sign(result, sign_1);

    } else {
      decimal_long long_result,
          long_value_1 = _decimal_to_longd(&value_1),
          long_value_2 = _decimal_to_longd(&value_2);
      compare_result change_sign = NO;

      rtrn_value =
          _sub_long(long_value_1, long_value_2, &long_result, &change_sign);

      if (change_sign) long_result.sign = !sign_2;

      _longd_to_decimal(long_result, result);
    }
  }

  if (rtrn_value) _init_decimal(result);

  return rtrn_value;
}

int mul(decimal value_1, decimal value_2, decimal *result) {
  decimal_long long_result, long_value_1 = _decimal_to_longd(&value_1),
                                long_value_2 = _decimal_to_longd(&value_2);
  _init_decimal_long(&long_result);
  arithmetics_result rtrn_value = OK;
  compare_result overflow = NO;
  _init_decimal(result);

  overflow = _bit_mul(long_value_1, long_value_2, &long_result);

  for (int i = long_result.scale; i > 28; i--)
    _divide_to_rescale(&long_result, 5);

  if (overflow) rtrn_value = _bank_round(&long_result, long_result.sign);

  _longd_to_decimal(long_result, result);

  if (rtrn_value) _init_decimal(result);

  return rtrn_value;
}

int _check_max_num(const decimal value_1) {
  const decimal max_big = {{-1, -1, -1, 0}};
  return is_equal(value_1, max_big);
}

int _check_one_num(const decimal value_2) {
  const decimal one = {{1, 0, 0, 0}};
  const decimal m_one = {{1, 0, 0, -2147483648}};
  return is_less(value_2, one) && is_greater(value_2, m_one);
}

int _check_min_num(const decimal value_1) {
  const decimal min_big = {{-1, -1, -1, -2147483648}};
  return is_equal(value_1, min_big);
}

int _max_both(const decimal value_1, const decimal value_2) {
  return (_check_max_num(value_1) || _check_min_num(value_1)) &&
         (_check_max_num(value_2) || _check_min_num(value_2));
}

int _div_check_case(const decimal value_1, const decimal value_2,
                        decimal *result, const int result_sign,
                        compare_result *do_alg) {
  arithmetics_result rtrn_value = OK;

  if (_dec_zero(value_2)) {
    rtrn_value = _NAN;
    *do_alg = NO;
  } else if (_dec_zero(value_1)) {
    _set_sign(result, result_sign);
    *do_alg = NO;
  } else if (_check_max_num(value_1) && _check_one_num(value_2)) {
    rtrn_value = INF;
    *do_alg = NO;
  } else if (_check_min_num(value_1) && _check_one_num(value_2)) {
    rtrn_value = NEG_INF;
    *do_alg = NO;
  } else if (_max_both(value_1, value_2)) {
    result->bits[0] = 1;
    _set_sign(result, result_sign);
    *do_alg = NO;
  }
  return rtrn_value;
}

int div(decimal value_1, decimal value_2, decimal *result) {
  arithmetics_result rtrn_value = OK;
  compare_result do_alg = YES;
  _init_decimal(result);
  const int result_sign =
      (_get_sign(value_1) == _get_sign(value_2)) ? PLUS : MINUS;

  rtrn_value =
      _div_check_case(value_1, value_2, result, result_sign, &do_alg);
  if (do_alg) {
    decimal_long long_result,
        long_value_1 = _decimal_to_longd(&value_1),
        long_value_2 = _decimal_to_longd(&value_2);

    rtrn_value = _div_long(long_value_1, long_value_2, &long_result);

    _longd_to_decimal(long_result, result);
  }

  if (rtrn_value) _init_decimal(result);

  return rtrn_value;
}

int mod(decimal value_1, decimal value_2, decimal *result) {
  arithmetics_result rtrn_value = OK;
  _init_decimal(result);
  const int result_sign = (_get_sign(value_1)) ? MINUS : PLUS;
  _set_sign(&value_1, PLUS);
  _set_sign(&value_2, PLUS);

  if (is_less(value_1, value_2)) {
    _copy_decimal(value_1, result);
  } else {
    decimal loc_result, copy_result;
    _init_decimal(&loc_result);
    _init_decimal(&copy_result);

    rtrn_value = div(value_1, value_2, &copy_result);
    if (!rtrn_value) {
      truncate(copy_result, &loc_result);
      mul(value_2, loc_result, &copy_result);
      sub(value_1, copy_result, &loc_result);
      _copy_decimal(loc_result, result);
    }
  }
  if (!rtrn_value) _set_sign(result, result_sign);
  if (rtrn_value == INF && result_sign) rtrn_value = NEG_INF;

  return rtrn_value;
}

//||||||||||||||||||||||||||| \ARITHMETIC |||||||||||||||||||||||||||

//||||||||||||||||||||||||||| COMPARISON ||||||||||||||||||||||||||||

int is_not_equal(decimal value_1, decimal value_2) {
  return !is_equal(value_1, value_2);
}

int is_equal(decimal value_1, decimal value_2) {
  compare_result output = NO;
  if (_get_scale(value_1) <= 28 && _get_scale(value_2) <= 28) {
    decimal_long long_value_1 = _decimal_to_longd(&value_1),
                     long_value_2 = _decimal_to_longd(&value_2);
    output = _is_equal_long(long_value_1, long_value_2);
  }
  return output;
}

int is_greater(decimal value_1, decimal value_2) {
  compare_result output = NO;
  if (_check_dec_scale(value_1, value_2)) {
    decimal_long long_value_1 = _decimal_to_longd(&value_1),
                     long_value_2 = _decimal_to_longd(&value_2);
    output = _is_greater_long(long_value_1, long_value_2);
  }
  return output;
}

int is_greater_or_equal(decimal value_1, decimal value_2) {
  compare_result output = NO;
  if ((is_greater(value_1, value_2) == YES ||
       is_equal(value_1, value_2) == YES) &&
      _check_dec_scale(value_1, value_2))
    output = YES;
  return output;
}

int is_less_or_equal(decimal value_1, decimal value_2) {
  return !is_greater(value_1, value_2);
}

int is_less(decimal value_1, decimal value_2) {
  return !is_greater_or_equal(value_1, value_2);
}

//||||||||||||||||||||||||||| \COMPARISON ||||||||||||||||||||||||||||

//||||||||||||||||||||||||||| OTHER FUNC ||||||||||||||||||||||||||||

int floor(decimal value, decimal *result) {
  decimal out = {{0, 0, 0, 0}};
  decimal minus_one = {{1, 0, 0, -2147483648}};
  decimal_long new_res = _decimal_to_longd(&value);
  int scale = _get_scale(value);
  unsigned long number = 0;
  _init_decimal(result);
  for (int i = 0; i < scale; i++)
    number = _divide_to_rescale(&new_res, HIGH);
  _longd_to_decimal(new_res, result);
  if (_get_sign(value) == MINUS && scale > 0 && number > 0) {
    add(*result, minus_one, &out);
    *result = out;
  }
  return SUCCESS;
}

int round(decimal value, decimal *result) {
  decimal out = {{0, 0, 0, 0}};
  decimal one = {{1, 0, 0, 0}};
  decimal minus_one = {{1, 0, 0, -2147483648}};
  decimal_long new_res = _decimal_to_longd(&value);
  int scale = _get_scale(value);
  unsigned long number = 0;
  _init_decimal(result);
  for (int i = 0; i < scale; i++)
    number = _divide_to_rescale(&new_res, HIGH);
  _longd_to_decimal(new_res, result);
  if (number >= 5) {
    (_get_sign(value) == MINUS) ? add(*result, minus_one, &out)
                                    : add(*result, one, &out);
    *result = out;
  }
  return SUCCESS;
}

int truncate(decimal value, decimal *result) {
  decimal_long new_res = _decimal_to_longd(&value);
  _init_decimal(result);
  int scale = _get_scale(value);
  for (int i = 0; i < scale; i++) _divide_to_rescale(&new_res, HIGH);
  _longd_to_decimal(new_res, result);
  return SUCCESS;
}

int negate(decimal value, decimal *result) {
  _copy_decimal(value, result);
  _set_sign(result, !_get_sign(*result));
  return SUCCESS;
}

//||||||||||||||||||||||||||| \OTHER FUNC |||||||||||||||||||||||||||

// //|||||||||||||||||||||||   PARSER     |||||||||||||||||||||||||||

int from_int_to_decimal(int src, decimal *dst) {
  operations_result error_flag = ERROR;
  if (dst) {
    error_flag = SUCCESS;
    _init_decimal(dst);
    int sign = (src >= 0) ? PLUS : MINUS;  // 1 - negative, 0 - positive
    dst->bits[LOW] = abs(src);
    _set_sign(dst, sign);
  }
  return error_flag;
}

int from_decimal_to_int(decimal src, int *dst) {
  operations_result error_flag = ERROR;
  if (dst && _get_scale(src) <= 28) {
    error_flag = SUCCESS;
    decimal truncated = {0};
    int code = truncate(src, &truncated);

    int sign = _get_sign(src);
    if (code == 1) error_flag = ERROR;
    if (truncated.bits[1] || truncated.bits[2]) error_flag = ERROR;
    int result = 0;
    for (int i = 0; i <= 31; i++)
      result += pow(2, i) * _get_bit(truncated, i);
    if (sign) result = -result;
    *dst = result;
  }
  return error_flag;
}

int from_float_to_decimal(float src, decimal *dst) {
  decimal ten = {{0}};
  from_int_to_decimal(10, &ten);
  int result = 0;
  int scale = 0;
  int sign = (src >= 0) ? PLUS : MINUS;
  src = fabsf(src);
  if ((src < 1e-28 && src != 0.0f) || src > 79228162514264337593543950335.0 ||
      isinf(src) || isnan(src)) {
    result = 1;
    _init_decimal(dst);
  } else {
    int number = _parse_float(src, &scale);
    _init_decimal(dst);
    dst->bits[0] = number;
    _set_sign(dst, sign);
    if (scale >= 0 && scale <= 28) {
      _set_scale(dst, scale);
    } else if (scale < 0) {
      for (int i = 0; i < -scale; ++i) mul(*dst, ten, dst);
    }
  }
  return result;
}

int from_decimal_to_float(decimal src, float *dst) {
  operations_result error_flag = ERROR;
  if (dst && _get_scale(src) <= 28) {
    error_flag = SUCCESS;
    long double result = 0;
    for (int i = 0; i < 96; i++) {
      if (_get_bit(src, i)) {
        result += (long double)pow(2, i);
      }
    }
    int scale = _get_scale(src);
    for (; scale; scale -= 1) result /= 10;
    if (_get_sign(src)) result = -(result);
    *dst = result;
  }
  return error_flag;
}

// //||||||||||||||||||||||   \PARSER     |||||||||||||||||||||||||||
