#include <check.h>
#include <locale.h>

#include "decimal.h"

#define EPSILON 1e-7

//|||||||||||||||||||||  UTILS |||||||||||||||||||||
START_TEST(rescale_test) {
  decimal dec_number_1 = {{50, 0, 0, 65536}};    // 5.0
  decimal dec_number_2 = {{758, 0, 0, 196608}};  // 0.758
  decimal_long long_dec_number_1 = _decimal_to_longd(&dec_number_1),
                   long_dec_number_2 = _decimal_to_longd(&dec_number_2);
  int rescale_1 = _get_scale(dec_number_2);
  _rescale(&long_dec_number_1, &long_dec_number_2);
  ck_assert_int_eq(rescale_1, long_dec_number_2.scale);
}
END_TEST

START_TEST(rescale_test_2) {
  decimal dec_number_1 = {
      {-100, -1, 99, 131072}};                        // 18446744073709551615.0
  decimal dec_number_2 = {{965453154, 0, 0, 0}};  // 965453154
  decimal real_result = {{2056034888, 22, 0, 131072}};  // 965453154.0

  decimal_long long_dec_number_1 = _decimal_to_longd(&dec_number_1),
                   long_dec_number_2 = _decimal_to_longd(&dec_number_2);

  _rescale(&long_dec_number_1, &long_dec_number_2);
  _longd_to_decimal(long_dec_number_2, &dec_number_2);

  ck_assert_int_eq(real_result.bits[3], dec_number_2.bits[3]);
  ck_assert_int_eq(real_result.bits[2], dec_number_2.bits[2]);
  ck_assert_int_eq(real_result.bits[1], dec_number_2.bits[1]);
  ck_assert_int_eq(real_result.bits[0], dec_number_2.bits[0]);
}
END_TEST

START_TEST(div_10_test) {
  decimal dec_number_1 = {{50, 0, 0, 65536}};  // 5.0
  decimal_long long_dec_number_1 = _decimal_to_longd(&dec_number_1);
  _divide_to_rescale(&long_dec_number_1, 5);

  // int result = _get_scale(dec_number_1);

  ck_assert_int_eq(0, long_dec_number_1.scale);
}
END_TEST

START_TEST(set_scale_test) {
  decimal dec_number = {{50, 0, 0, 65536}};  // 5.0
  _set_scale(&dec_number, 29);
}
END_TEST
//|||||||||||||||||||||  \UTILS  |||||||||||||||||||||

//|||||||||||||||||||||    COMPARISON   |||||||||||||||||||||
START_TEST(equal_test_1) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};  // 3.0
  decimal dec_number_2 = {{30, 0, 0, 65536}};  // 3.0
  _set_scale(&dec_number_2, 2);                // => 0.30

  int result = is_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(equal_test_2) {
  decimal dec_number_1 = {{0, 0, 0, 65536}};        // 0.0
  decimal dec_number_2 = {{0, 0, 0, -2147418112}};  // -0.0

  int result = is_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(equal_test_3) {
  decimal dec_number_1 = {{0, 0, 0, 0}};  // 0
  decimal dec_number_2 = {{0, 0, 0, 0}};  // 0

  int result = is_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(equal_test_4) {
  decimal dec_number_1 = {{7831926, 0, 0, 262144}};  // 783.1926
  decimal dec_number_2 = {{7830926, 0, 0, 262144}};  // 783.0926

  int result = is_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(not_equal_test_1) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};  // 3.0
  decimal dec_number_2 = {{30, 0, 0, 65536}};  // 3.0
  _set_scale(&dec_number_2, 2);                // => 0.30

  int result = is_not_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(not_equal_test_2) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};        // 3.0
  decimal dec_number_2 = {{30, 0, 0, -2147418112}};  // -3.0

  int result = is_not_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(not_equal_test_3) {
  decimal dec_number_1 = {
      {-538901499, 2, 0, -2147352576}};  // -123460003.89
  decimal dec_number_2 = {
      {-538901499, 2, 0, -2147352576}};  // -123460003.89

  int result = is_not_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(not_equal_test_4) {
  decimal dec_number_1 = {{1, 0, 0, 262144}};  // 0.0001
  decimal dec_number_2 = {{0, 0, 0, 0}};       // 0

  int result = is_not_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(greater_or_equal_test_1) {
  decimal dec_number_1 = {{0, 0, 0, 65536}};        // 0.0
  decimal dec_number_2 = {{0, 0, 0, -2147418112}};  // -0.0

  int result = is_greater_or_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(greater_or_equal_test_2) {
  decimal dec_number_1 = {{3, 0, 0, 65536}};  // 0.3
  decimal dec_number_2 = {{3, 0, 0, 65536}};  // 0.3
  _set_scale(&dec_number_2, 17);              // => 0.00000000000000003

  int result = is_greater_or_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(greater_or_equal_test_3) {
  decimal dec_number_1 = {{7831926, 0, 0, 262144}};  // 783.1926
  decimal dec_number_2 = {{7830926, 0, 0, 262144}};  // 783.0926

  int result = is_greater(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(greater_or_equal_test_4) {
  // 632323465788987654
  decimal dec_number_1 = {{-1988159226, 147224279, 0, 0}};
  // 67543278.897654243546576873
  decimal dec_number_2 = {{1823422441, 2086845379, 3661528, 1179648}};
  _set_scale(&dec_number_2, 17);
  // => 675432788.97654243546576873

  int result = is_greater_or_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(greater_test_1) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};  // 3.0
  decimal dec_number_2 = {{35, 0, 0, 65536}};  // 3.5

  int result = is_greater(dec_number_1, dec_number_2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(greater_test_5) {
  decimal dec_number_1 = {{35, 0, 0, 65536}};  // 3.5
  decimal dec_number_2 = {{30, 0, 0, 65536}};  // 3.0

  int result = is_greater(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(greater_test_2) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};  // 3.0
  decimal dec_number_2 = {{30, 0, 0, 65536}};  // 3.0

  int result = is_greater(dec_number_1, dec_number_2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(greater_test_3) {
  decimal dec_number_1 = {{35, 0, 0, 65536}};  // 3.5
  decimal dec_number_2 = {{30, 0, 0, 65536}};  // 3.0
  _set_scale(&dec_number_1, 17);               // => 0.00000000000000035

  int result = is_greater(dec_number_1, dec_number_2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(greater_test_4) {
  decimal dec_number_1 = {{0, 0, 0, 65536}};        // 0.0
  decimal dec_number_2 = {{0, 0, 0, -2147418112}};  // -0.0

  int result = is_greater(dec_number_1, dec_number_2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(less_test_1) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};  // 3.0
  decimal dec_number_2 = {{35, 0, 0, 65536}};  // 3.5

  int result = is_less(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(less_test_2) {
  decimal dec_number_1 = {{7831926, 0, 0, 262144}};  // 783.1926
  decimal dec_number_2 = {{7830926, 0, 0, 262144}};  // 783.0926

  int result = is_less(dec_number_1, dec_number_2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(less_test_3) {
  decimal dec_number_1 = {{78300926, 0, 0, -2147221504}};  // -783.1926
  decimal dec_number_2 = {{78310926, 0, 0, -2147221504}};  // -783.1926

  int result = is_less(dec_number_1, dec_number_2);
  ck_assert_int_eq(0, result);
}
END_TEST

START_TEST(less_or_equal_test_1) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};  // 3.0
  decimal dec_number_2 = {{35, 0, 0, 65536}};  // 3.5

  int result = is_less_or_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(1, result);
}
END_TEST

START_TEST(less_or_equal_test_2) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};        // 3.0
  decimal dec_number_2 = {{35, 0, 0, -2147221504}};  // -0.0035

  int result = is_less_or_equal(dec_number_1, dec_number_2);
  ck_assert_int_eq(0, result);
}
END_TEST
//|||||||||||||||||||||    \COMPARISON    |||||||||||||||||||||

// //|||||||||||||||||||||    OTHER_FUNC    |||||||||||||||||||||

START_TEST(truncate_1) {
  decimal dec_number = {{80, 0, 0, 65536}};  // 8.0
  decimal real_result = {{8, 0, 0, 0}};      // 8
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_2) {
  decimal dec_number = {{100, 0, 0, -2147418112}};  // -10.0
  decimal real_result = {{10, 0, 0, -2147483648}};  // -10
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_3) {
  decimal dec_number = {{-1, -1, -1, 0}};
  //  79228162514264337593543950335
  decimal real_result = {{-1, -1, -1, 0}};
  // 79228162514264337593543950335
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_4) {
  decimal dec_number = {
      {226492415, -407762874, 3388, 720896}};  // 625142643375.93543950335
  decimal real_result = {{-1922581841, 145, 0, 0}};  // 625142643375
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_5) {
  decimal dec_number = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal real_result = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_6) {
  decimal dec_number = {{226492415, -407762874, 3388, -2146762752}};
  // -625142643375.93543950335
  decimal real_result = {
      {-1922581841, 145, 0, -2147483648}};  // -625142643375
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_7) {
  decimal dec_number = {{2000, 0, 0, -2147418112}};  // -200.0
  decimal real_result = {{200, 0, 0, -2147483648}};  // -200
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_8) {
  decimal dec_number = {{20023456, 0, 0, 327680}};  // 200.23456
  decimal real_result = {{200, 0, 0, 0}};           // 200
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_9) {
  decimal dec_number = {{200000, 0, 0, -2147287040}};  // -200.000
  decimal real_result = {{200, 0, 0, -2147483648}};    // -200
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_10) {
  decimal dec_number = {{0, 0, 0, -2147418112}};   // -0.0
  decimal real_result = {{0, 0, 0, -2147483648}};  // -0 == 0
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_11) {
  decimal dec_number = {{0, 0, 0, 65536}};  // 0.0
  decimal real_result = {{0, 0, 0, 0}};     // 0
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_12) {
  decimal dec_number = {{0, 0, 0, 720896}};  // 0.00000000000
  decimal real_result = {{0, 0, 0, 0}};      // 0
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_13) {
  decimal dec_number = {{0, 0, 0, -2146762752}};   // -0.00000000000
  decimal real_result = {{0, 0, 0, -2147483648}};  // -0 == 0
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_14) {
  decimal dec_number = {{-1865865216, -1999963108, 317,
                             -2146762752}};  // -58574748393.00000000000
  decimal real_result = {
      {-1554793751, 13, 0, -2147483648}};  // -58574748393
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_15) {
  decimal dec_number = {
      {-1865865216, -1999963108, 317, 720896}};  // 58574748393.00000000000
  decimal real_result = {{-1554793751, 13, 0, 0}};  // 58574748393
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(truncate_16) {
  decimal dec_number = {{505050550, 0, 0, 786432}};  // 0.000505050550
  decimal real_result = {{0, 0, 0, 0}};              // 0
  decimal result = {0};

  truncate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_1) {
  decimal dec_number = {{28, 0, 0, 65536}};         // 2.8
  decimal real_result = {{28, 0, 0, -2147418112}};  // -2.8
  decimal result = {0};
  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_2) {
  decimal dec_number = {{28, 0, 0, -2147418112}};  // -2.8
  decimal real_result = {{28, 0, 0, 65536}};       // 2.8
  decimal result = {0};

  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_3) {
  decimal dec_number = {{-1, -1, -1, 0}};  //
  // 79228162514264337593543950335
  decimal real_result = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal result = {0};

  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_4) {
  decimal dec_number = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal real_result = {{-1, -1, -1, 0}};
  // 79228162514264337593543950335
  decimal result = {0};

  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_5) {
  decimal dec_number = {
      {-1, -1, -1, -2145845248}};  // -7922.8162514264337593543950335
  decimal real_result = {
      {-1, -1, -1, 1638400}};  // 7922.8162514264337593543950335
  decimal result = {0};

  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_6) {
  decimal dec_number = {
      {-1, -1, -1, 1638400}};  // 7922.8162514264337593543950335
  decimal real_result = {
      {-1, -1, -1, -2145845248}};  // -7922.8162514264337593543950335
  decimal result = {0};

  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_7) {
  decimal dec_number = {{0, 0, 0, 65536}};         // 0.0
  decimal real_result = {{0, 0, 0, -2147418112}};  // -0.0
  decimal result = {0};

  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_8) {
  decimal dec_number = {{0, 0, 0, -2147418112}};  // -0.0
  decimal real_result = {{0, 0, 0, 65536}};       // 0.0
  decimal result = {0};

  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_9) {
  decimal dec_number = {{34567, 0, 0, -2147155968}};  // -0.34567
  decimal real_result = {{34567, 0, 0, 327680}};      // 0.34567
  decimal result = {0};

  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_10) {
  decimal dec_number = {{34567, 0, 0, 327680}};        // 0.34567
  decimal real_result = {{34567, 0, 0, -2147155968}};  // -0.34567
  decimal result = {0};

  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_11) {
  // 79228162514264337593543933.3
  decimal dec_number = {{687194597, -171798692, 42949672, 65536}};
  // -79228162514264337593543933.3
  decimal real_result = {{687194597, -171798692, 42949672, -2147418112}};
  decimal result = {0};
  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(negate_12) {
  // -79228162514264337593543933.3
  decimal dec_number = {{687194597, -171798692, 42949672, -2147418112}};
  // 79228162514264337593543933.3
  decimal real_result = {{687194597, -171798692, 42949672, 65536}};
  decimal result = {0};

  negate(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(round_1) {
  decimal dec_number = {{23, 0, 0, 65536}};  // 2.3
  decimal real_result = {{2, 0, 0, 0}};      // 2
  decimal result = {0};

  round(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(round_2) {
  decimal dec_number = {{23, 0, 0, -2147418112}};  // -2.3
  decimal real_result = {{2, 0, 0, -2147483648}};  // -2
  decimal result = {0};

  round(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(round_3) {
  decimal dec_number = {{27, 0, 0, 65536}};  // 2.7
  decimal real_result = {{3, 0, 0, 0}};      // 3
  decimal result = {0};

  round(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(round_4) {
  // 79228162514264337593543933.3
  decimal dec_number = {{687194597, -171798692, 42949672, 65536}};
  // 79228162514264337593543933
  decimal real_result = {{-1649267459, 1271310319, 4294967, 0}};
  decimal result = {0};

  round(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(round_5) {
  // -79228162514264337593543933.3
  decimal dec_number = {{687194597, -171798692, 42949672, -2147418112}};
  // -79228162514264337593543933
  decimal real_result = {{-1649267459, 1271310319, 4294967, -2147483648}};
  decimal result = {0};

  round(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(round_6) {
  // 79228162514264337593543934.8
  decimal dec_number = {{687194612, -171798692, 42949672, 65536}};
  // 79228162514264337593543935
  decimal real_result = {{-1649267457, 1271310319, 4294967, 0}};
  decimal result = {0};

  round(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(round_7) {
  // -79228162514264337593543934.8
  decimal dec_number = {{687194612, -171798692, 42949672, -2147418112}};
  // -79228162514264337593543935
  decimal real_result = {{-1649267457, 1271310319, 4294967, -2147483648}};
  decimal result = {0};

  round(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_1) {
  decimal dec_number = {{758, 0, 0, 196608}};  // 0.758
  decimal real_result = {{0, 0, 0, 0}};        // 0
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_2) {
  decimal dec_number = {{-98383678, 540, 0, 786432}};  // 2.323478923458
  decimal real_result = {{2, 0, 0, 0}};                // 2
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_3) {
  decimal dec_number = {{-1, -1, -1, 0}};  //
  // 79228162514264337593543950335
  decimal real_result = {{-1, -1, -1, 0}};
  // 79228162514264337593543950335
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_4) {
  decimal dec_number = {
      {-1, -1, -1, 1376256}};  // 79228162.514264337593543950335
  decimal real_result = {{79228162, 0, 0, 0}};  // 79228162
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_5) {
  decimal dec_number = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal real_result = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_6) {
  decimal dec_number = {
      {-1098810323, 18446, 0, -2147090432}};  // -79228162.898989
  decimal real_result = {{79228163, 0, 0, -2147483648}};  // -79228163
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_7) {
  decimal dec_number = {{0, 0, 0, 65536}};  // 0.0
  decimal real_result = {{0, 0, 0, 0}};     // 0
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_8) {
  decimal dec_number = {{0, 0, 0, -2147418112}};   // -0.0
  decimal real_result = {{0, 0, 0, -2147483648}};  // -0 == 0
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_9) {
  decimal dec_number = {{7, 0, 0, 65536}};  // 0.7
  decimal real_result = {{0, 0, 0, 0}};     // 0
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_10) {
  decimal dec_number = {{7, 0, 0, -2147418112}};   // -0.7
  decimal real_result = {{1, 0, 0, -2147483648}};  // -1
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_11) {
  decimal dec_number = {{687194597, -171798692, 42949672,
                             65536}};  // 79228162514264337593543933.3
  decimal real_result = {
      {-1649267459, 1271310319, 4294967, 0}};  // 79228162514264337593543933
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_12) {
  decimal dec_number = {{687194597, -171798692, 42949672,
                             -2147418112}};  // 79228162514264337593543933.3
  decimal real_result = {{-1649267458, 1271310319, 4294967,
                              -2147483648}};  // 79228162514264337593543934
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_13) {
  decimal dec_number = {{687194613, -171798692, 42949672,
                             65536}};  // 79228162514264337593543934.9
  decimal real_result = {
      {-1649267458, 1271310319, 4294967, 0}};  // 79228162514264337593543934
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_14) {
  decimal dec_number = {{687194613, -171798692, 42949672,
                             -2147418112}};  // -79228162514264337593543934.9
  decimal real_result = {{-1649267457, 1271310319, 4294967,
                              -2147483648}};  // -79228162514264337593543935
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_15) {
  decimal dec_number = {{687194604, -171798692, 42949672,
                             65536}};  // 79228162514264337593543934.0
  decimal real_result = {
      {-1649267458, 1271310319, 4294967, 0}};  // 79228162514264337593543934
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(floor_16) {
  decimal dec_number = {{687194604, -171798692, 42949672,
                             -2147418112}};  // -79228162514264337593543934.0
  decimal real_result = {{-1649267458, 1271310319, 4294967,
                              -2147483648}};  // -79228162514264337593543934
  decimal result = {0};

  floor(dec_number, &result);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

//|||||||||||||||||||||    \OTHER_FUNC    |||||||||||||||||||||

//|||||||||||||||||||||    ARITHMETIC     |||||||||||||||||||||
START_TEST(sub_0) {
  decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_1) {
  decimal val1 = {{2, 0, 0, 0}};
  decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_2) {
  decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_3) {
  decimal val1 = {{2, 0, 0, 0}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_4) {
  decimal val1 = {{8, 0, 0, 0}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_5) {
  decimal val1 = {{2, 0, 0, 0}};
  decimal val2 = {{8, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_6) {
  decimal val1 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_7) {
  decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{8, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_8) {
  decimal val1 = {{2, 0, 0, 0}};
  decimal val2 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_9) {
  decimal val1 = {{8, 0, 0, 0}};
  decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_10) {
  decimal val1 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_11) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{4, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(2, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_12) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal val2 = {{4, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_13) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_14) {
  decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal res;
  ck_assert_int_eq(2, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_15) {
  decimal val1 = {{4, 0, 0, 0}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_16) {
  decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_17) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(1, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_18) {
  decimal val1 = {{4, 0, 0, 0}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(1, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_19) {
  decimal val1 = {{4, 0, 0, 0}};
  decimal val2 = {{4, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(add_0) {
  decimal val1 = {{15, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_1) {
  decimal val1 = {{15, 0, 0, 0}};
  decimal val2 = {{15, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_2) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal val2 = {{1, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(1, add(val1, val2, &res));
}
END_TEST

START_TEST(add_3) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(2, add(val1, val2, &res));
}
END_TEST

START_TEST(add_4) {
  decimal val1 = {{8, 0, 0, 0}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_5) {
  decimal val1 = {{2, 0, 0, 0}};
  decimal val2 = {{8, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_6) {
  decimal val1 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_7) {
  decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{8, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_8) {
  decimal val1 = {{0}};
  decimal val2 = {{0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_9) {
  decimal val1 = {{4, 0, 0, 0}};
  decimal val2 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_10) {
  decimal val1 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_11) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{4, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_12) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(2, add(val1, val2, &res));
}
END_TEST

START_TEST(add_13) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_14) {
  decimal val1 = {{4, 0, 0, 0}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_15) {
  decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_16) {
  decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(2, add(val1, val2, &res));
}
END_TEST

START_TEST(add_17) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  _set_scale(&val1, 5);
  _set_scale(&val2, 3);
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_18) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  _set_scale(&val1, 5);
  _set_scale(&val2, 3);
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_19) {
  decimal value_1, value_2, origin;
  int code_check = 0;
  int code = 0;
  value_1.bits[0] = 0b00000000000001011001000011001100;
  value_1.bits[1] = 0b00000000000000000000000000000000;
  value_1.bits[2] = 0b00000000000000000000000000000000;
  value_1.bits[3] = 0b10000000000000000000000000000000;
  value_2.bits[0] = 0b00000000000000000000000000000001;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = add(value_1, value_2, &result);
  origin.bits[0] = 0b00000000000001011001000011001011;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(add_20) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 18446744073709551615.0;
  // src2 = 965453154;
  src1.bits[0] = 0b11111111111111111111111111110110;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b00000000000000000000000000001001;
  src1.bits[3] = 0b00000000000000010000000000000000;
  src2.bits[0] = 0b00111001100010111010010101100010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = add(src1, src2, &result);
  origin.bits[0] = 0b00111111011101000111010111001010;
  origin.bits[1] = 0b00000000000000000000000000000010;
  origin.bits[2] = 0b00000000000000000000000000001010;
  origin.bits[3] = 0b00000000000000010000000000000000;
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
  ck_assert_int_eq(code_check, code);
}

END_TEST
START_TEST(add_21) {
  decimal value_1, value_2, origin;
  int code_check = 0;
  int code = 0;
  value_1.bits[0] = 0b10000010111000100101101011101101;
  value_1.bits[1] = 0b11111001111010000010010110101101;
  value_1.bits[2] = 0b10110000001111101111000010010100;
  value_1.bits[3] = 0b10000000000011100000000000000000;
  value_2.bits[0] = 0b00000011010000001001011100101110;
  value_2.bits[1] = 0b00000000000000000000000000000000;
  value_2.bits[2] = 0b00000000000000000000000000000000;
  value_2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = add(value_1, value_2, &result);
  origin.bits[0] = 0b01110001001010101101101011101101;
  origin.bits[1] = 0b00101110001111001110000111111000;
  origin.bits[2] = 0b10110000001111101110111101101101;
  origin.bits[3] = 0b10000000000011100000000000000000;
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(add_22) {
  // 632323465788987654
  decimal dec_number_1 = {
      {-1988159226, 147224279, 0, 0}};  // 632323465788987654
  // 67543278.897654243546576873
  decimal dec_number_2 = {{1823422441, 2086845379, 3661528, 1179648}};
  _set_scale(&dec_number_2, 17);
  // => 675432788.97654243546576873
  decimal real_result = {{-1555885861, 1165861392, -867134916, 720896}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = add(dec_number_1, dec_number_2, &result);

  ck_assert_int_eq(0, error_code);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
}
END_TEST

START_TEST(sub_0) {
  decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_1) {
  decimal val1 = {{2, 0, 0, 0}};
  decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_2) {
  decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_3) {
  decimal val1 = {{2, 0, 0, 0}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_4) {
  decimal val1 = {{8, 0, 0, 0}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_5) {
  decimal val1 = {{2, 0, 0, 0}};
  decimal val2 = {{8, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_6) {
  decimal val1 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_7) {
  decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{8, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_8) {
  decimal val1 = {{2, 0, 0, 0}};
  decimal val2 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_9) {
  decimal val1 = {{8, 0, 0, 0}};
  decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_10) {
  decimal val1 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_11) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{4, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(2, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_12) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal val2 = {{4, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_13) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_14) {
  decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal res;
  ck_assert_int_eq(2, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_15) {
  decimal val1 = {{4, 0, 0, 0}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_16) {
  decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_17) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;

  ck_assert_int_eq(1, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_18) {
  decimal val1 = {{4, 0, 0, 0}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(1, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_19) {
  decimal val1 = {{4, 0, 0, 0}};
  decimal val2 = {{4, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, sub(val1, val2, &res));
}
END_TEST

START_TEST(sub_20) {
  decimal dec1;
  decimal dec2;
  int tmp1 = 100;
  int tmp2 = 99999;
  int res_s21 = 0;
  int res = -99899;
  decimal res1;
  from_int_to_decimal(tmp1, &dec1);
  from_int_to_decimal(tmp2, &dec2);
  sub(dec1, dec2, &res1);
  from_decimal_to_int(res1, &res_s21);
  ck_assert_int_eq(res_s21, res);
}
END_TEST

START_TEST(sub_21) {
  decimal dec1;
  decimal dec2;
  int tmp1 = -100;
  int tmp2 = -99999;
  int res_s21 = 0;
  decimal res1;
  int res = 99899;
  from_int_to_decimal(tmp1, &dec1);
  from_int_to_decimal(tmp2, &dec2);
  sub(dec1, dec2, &res1);
  from_decimal_to_int(res1, &res_s21);
  ck_assert_int_eq(res_s21, res);
}
END_TEST

START_TEST(sub_max_30) {
  // 79228162514264337593543950335
  // 0.002
  decimal src1 = {{-1, -1, -1, 0}};
  decimal src2 = {{2, 0, 0, 196608}};
  decimal original_res = {{-1, -1, -1, 0}};
  decimal res;
  ck_assert_int_eq(sub(src1, src2, &res), 0);
  ck_assert_int_eq(original_res.bits[0], res.bits[0]);
  ck_assert_int_eq(original_res.bits[1], res.bits[1]);
  ck_assert_int_eq(original_res.bits[2], res.bits[2]);
  ck_assert_int_eq(original_res.bits[3], res.bits[3]);
}
END_TEST

START_TEST(sub_max_31) {
  // 79228162514264337593543950335
  // 0.5
  decimal src1 = {{-1, -1, -1, 0}};
  decimal src2 = {{5, 0, 0, 65536}};
  decimal original_res = {{-2, -1, -1, 0}};
  decimal res;
  sub(src1, src2, &res);
  ck_assert_int_eq(sub(src1, src2, &res), 0);
  ck_assert_int_eq(original_res.bits[0], res.bits[0]);
  ck_assert_int_eq(original_res.bits[1], res.bits[1]);
  ck_assert_int_eq(original_res.bits[2], res.bits[2]);
  ck_assert_int_eq(original_res.bits[3], res.bits[3]);
}
END_TEST

START_TEST(sub_max_32) {
  // 79228162514264337593543950335
  // 1
  decimal src1 = {0};
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  decimal src2 = {0};
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal original_res = {0};
  original_res.bits[0] = 0b11111111111111111111111111111110;
  original_res.bits[1] = 0b11111111111111111111111111111111;
  original_res.bits[2] = 0b11111111111111111111111111111111;
  original_res.bits[3] = 0b00000000000000000000000000000000;
  decimal res = {0};
  sub(src1, src2, &res);
  ck_assert_int_eq(sub(src1, src2, &res), 0);
  ck_assert_int_eq(original_res.bits[0], res.bits[0]);
  ck_assert_int_eq(original_res.bits[1], res.bits[1]);
  ck_assert_int_eq(original_res.bits[2], res.bits[2]);
  ck_assert_int_eq(original_res.bits[3], res.bits[3]);
}
END_TEST

START_TEST(sub_max_33) {
  // -79228162514264337593543950335
  // 0.002
  decimal src1 = {0};
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  decimal src2 = {0};
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000110000000000000000;
  decimal original_res = {{-1, -1, -1, -2147483648}};
  decimal res = {0};
  ck_assert_int_eq(sub(src1, src2, &res), 0);
  ck_assert_int_eq(original_res.bits[0], res.bits[0]);
  ck_assert_int_eq(original_res.bits[1], res.bits[1]);
  ck_assert_int_eq(original_res.bits[2], res.bits[2]);
  ck_assert_int_eq(original_res.bits[3], res.bits[3]);
}
END_TEST

START_TEST(sub_max_34) {
  // -79228162514264337593543950335
  // 0.5
  decimal src1 = {0};
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  decimal src2 = {0};
  src2.bits[0] = 0b00000000000000000000000000000101;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000010000000000000000;
  decimal res = {0};
  ck_assert_int_eq(sub(src1, src2, &res), 2);
}
END_TEST

START_TEST(sub_max_35) {
  decimal src1 = {0};
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  decimal src2 = {0};
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal res = {0};
  ck_assert_int_eq(sub(src1, src2, &res), 2);
}
END_TEST

START_TEST(subTest1) {
  // 863
  decimal src1, src2, origin;
  // src1 = 2;
  // src2 = 3;

  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest2) {
  // 889
  decimal src1, src2, origin;
  // src1 = 3;
  // src2 = 2;

  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest3) {
  // 915
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = 3;

  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000000000000000000000000000011;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest4) {
  // 941
  decimal src1, src2, origin;
  // src1 = 2;
  // src2 = 0;

  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000000000000000000000000000010;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest5) {
  // 967
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = 3;

  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000000000000000000000000000011;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest6) {
  // 993
  decimal src1, src2, origin;
  // src1 = 2;
  // src2 = 0;

  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000000000000000000000000000010;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest7) {
  // 1019
  decimal src1, src2, origin;
  // src1 = 6521;
  // src2 = 74121;

  src1.bits[0] = 0b00000000000000000001100101111001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000010010000110001001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000000000000010000100000010000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest8) {
  // 1045
  decimal src1, src2, origin;
  // src1 = 4;
  // src2 = 97623323;

  src1.bits[0] = 0b00000000000000000000000000000100;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000101110100011001110100011011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000101110100011001110100010111;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest9) {
  // 1071
  decimal src1, src2, origin;
  // src1 = 65658654;
  // src2 = 5;

  src1.bits[0] = 100;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 95;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 5;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest10) {
  // 1097
  decimal src1, src2, origin;
  // src1 = -364748;
  // src2 = 1;

  src1.bits[0] = 0b00000000000001011001000011001100;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000000000001011001000011001101;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest11) {
  // 1123
  decimal src1, src2, origin;
  // src1 = 1;
  // src2 = 98745654321;

  src1.bits[0] = 0b00000000000000000000000000000001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b11111101101100110001110000110001;
  src2.bits[1] = 0b00000000000000000000000000010110;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b11111101101100110001110000110000;
  origin.bits[1] = 0b00000000000000000000000000010110;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest12) {
  // 1149
  decimal src1, src2, origin;
  // src1 = -9878798789;
  // src2 = -3;

  src1.bits[0] = 0b01001100110100101000000111000101;
  src1.bits[1] = 0b00000000000000000000000000000010;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b01001100110100101000000111000010;
  origin.bits[1] = 0b00000000000000000000000000000010;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest13) {
  // 1175
  decimal src1, src2, origin;
  // src1 = 9999999999999999999;
  // src2 = 1;

  src1.bits[0] = 0b10001001111001111111111111111111;
  src1.bits[1] = 0b10001010110001110010001100000100;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b10001001111001111111111111111110;
  origin.bits[1] = 0b10001010110001110010001100000100;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest14) {
  // 1201
  decimal src1, src2, origin;
  // src1 = 18446744073709551615;
  // src2 = 1;

  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b11111111111111111111111111111110;
  origin.bits[1] = 0b11111111111111111111111111111111;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest15) {
  // 1227
  decimal src1, src2, origin;
  // src1 = 18446744073709551615.0;
  // src2 = 965453154;

  src1.bits[0] = 0b11111111111111111111111111110110;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b00000000000000000000000000001001;
  src1.bits[3] = 0b00000000000000010000000000000000;

  src2.bits[0] = 0b00111001100010111010010101100010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b11000000100010111000101000100010;
  origin.bits[1] = 0b11111111111111111111111111111101;
  origin.bits[2] = 0b00000000000000000000000000001001;
  origin.bits[3] = 0b00000000000000010000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest16) {
  // 1253
  decimal src1, src2, origin;
  // src1 = -545454512454545.35265454545645;
  // src2 = 54564654;

  src1.bits[0] = 0b10000010111000100101101011101101;
  src1.bits[1] = 0b11111001111010000010010110101101;
  src1.bits[2] = 0b10110000001111101111000010010100;
  src1.bits[3] = 0b10000000000011100000000000000000;

  src2.bits[0] = 0b00000011010000001001011100101110;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b10010100100110011101101011101101;
  origin.bits[1] = 0b11000101100100110110100101100010;
  origin.bits[2] = 0b10110000001111101111000110111100;
  origin.bits[3] = 0b10000000000011100000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest17) {
  // 1279
  decimal src1, src2,
      origin = {{1335558144, -102351141, -1338052460, -2146566144}};
  // src1 = -545454512454545.35265454545645;
  // src2 = -5.352654545456454545645464;

  // -545454512454545.35265454545645 exp 14
  src1.bits[0] = 0b10000010111000100101101011101101;
  src1.bits[1] = 0b11111001111010000010010110101101;
  src1.bits[2] = 0b10110000001111101111000010010100;
  src1.bits[3] = 0b10000000000011100000000000000000;

  // -5.352654545456454545645464 exp 24
  src2.bits[0] = 0b01001000000110110001111110011000;
  src2.bits[1] = 0b11111011111111011000100101101101;
  src2.bits[2] = 0b00000000000001000110110101110111;
  src2.bits[3] = 0b10000000000110000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  // -54545451245453999999999999999 exp 14
  sub(src1, src2, &result);

  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest18) {
  // 1305
  decimal src1, src2, origin;
  // src1 = 7961327845421.879754123131254;
  // src2 = 0;

  src1.bits[0] = 0b01001110111001000011100101110110;
  src1.bits[1] = 0b01001011001101011010000111011001;
  src1.bits[2] = 0b00011001101110010111010010111111;
  src1.bits[3] = 0b00000000000011110000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b01001110111001000011100101110110;
  origin.bits[1] = 0b01001011001101011010000111011001;
  origin.bits[2] = 0b00011001101110010111010010111111;
  origin.bits[3] = 0b00000000000011110000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest19) {
  // 12345677.987654345678987654346
  // 87654323456.9876545678987653
  decimal src1 = {{-1861552950, -1069165070, 669260544, 1376256}};
  decimal src2 = {{298701189, -172200000, 47517503, 1048576}};
  decimal origin = {{-1845069659, -1955968801, 475108113, -2146369536}};
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);
  // -87641977779.00000022221977765
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest20) {
  // 1357
  decimal src1, src2, origin;
  // src1 = -12345677.987654345678987654346;
  // src2 = 87654323456.9876545678987653;

  src1.bits[0] = 0b10010001000010101111010011001010;
  src1.bits[1] = 0b11000000010001011101010111110010;
  src1.bits[2] = 0b00100111111001000001101100000000;
  src1.bits[3] = 0b10000000000101010000000000000000;

  src2.bits[0] = 0b00010001110011011101000110000101;
  src2.bits[1] = 0b11110101101111000110111111000000;
  src2.bits[2] = 0b00000010110101010000111100111111;
  src2.bits[3] = 0b00000000000100000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b11010010000011011110010110111111;
  origin.bits[1] = 0b10100111010011100111001000100001;
  origin.bits[2] = 0b00011100010100111001110111101101;
  origin.bits[3] = 0b10000000000100010000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest21) {
  // 1383
  decimal src1, src2, origin;
  // src1 = 12345677.987654345678987654346;
  // src2 = -87654323456.9876545678987653;

  src1.bits[0] = 0b10010001000010101111010011001010;
  src1.bits[1] = 0b11000000010001011101010111110010;
  src1.bits[2] = 0b00100111111001000001101100000000;
  src1.bits[3] = 0b00000000000101010000000000000000;

  src2.bits[0] = 0b00010001110011011101000110000101;
  src2.bits[1] = 0b11110101101111000110111111000000;
  src2.bits[2] = 0b00000010110101010000111100111111;
  src2.bits[3] = 0b10000000000100000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b11010010000011011110010110111111;
  origin.bits[1] = 0b10100111010011100111001000100001;
  origin.bits[2] = 0b00011100010100111001110111101101;
  origin.bits[3] = 0b00000000000100010000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest22) {
  // src1 = -12345677.987654345678987654346
  // src2 = -87654323456.9876545678987653

  decimal src1 = {{-1861552950, -1069165070, 669260544, -2146107392}};
  decimal src2 = {{298701189, -172200000, 47517503, -2146435072}};
  decimal origin = {{-1845069659, -1955968801, 475108113, 1114112}};
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest23) {
  // 1435
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = 0;

  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest24) {
  // 1461
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = 0;

  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest25) {
  // 1487
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = 0;

  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest26) {
  // 1513
  decimal src1, src2, origin;
  // src1 = 792281625142643375935439503;
  // src2 = -1;

  src1.bits[0] = 0b00101000111101011100001010001111;
  src1.bits[1] = 0b11110101110000101000111101011100;
  src1.bits[2] = 0b00000010100011110101110000101000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00101000111101011100001010010000;
  origin.bits[1] = 0b11110101110000101000111101011100;
  origin.bits[2] = 0b00000010100011110101110000101000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest27) {
  // 1539
  decimal src1, src2, origin;
  // src1 = 7922816251427554395;
  // src2 = 65645646;

  src1.bits[0] = 0b01011111000010000000010001011011;
  src1.bits[1] = 0b01101101111100110111111101100111;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000011111010011010110001001110;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b01011011000111100101100000001101;
  origin.bits[1] = 0b01101101111100110111111101100111;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest28) {
  // 1565
  decimal src1, src2, origin;
  // src1 = 665464545;
  // src2 = 8798232189789785;

  src1.bits[0] = 0b00100111101010100010111011100001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00010000010100101110011001011001;
  src2.bits[1] = 0b00000000000111110100000111110010;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b11101000101010001011011101111000;
  origin.bits[1] = 0b00000000000111110100000111110001;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest29) {
  // 1591
  decimal src1, src2, origin;
  // src1 = 2.7986531268974139743;
  // src2 = 9.979623121254565121244554;

  src1.bits[0] = 0b11111110100100001101100101011111;
  src1.bits[1] = 0b10000100011001000010000111101000;
  src1.bits[2] = 0b00000000000000000000000000000001;
  src1.bits[3] = 0b00000000000100110000000000000000;

  src2.bits[0] = 0b11010100101011100011110110001010;
  src2.bits[1] = 0b01110100000101000010011110100011;
  src2.bits[2] = 0b00000000000010000100000101000100;
  src2.bits[3] = 0b00000000000110000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b00001110111111111010100000101010;
  origin.bits[1] = 0b00101001110101100000001000110100;
  origin.bits[2] = 0b00000000000001011111000010100001;
  origin.bits[3] = 0b10000000000110000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest30) {
  // 1617
  decimal src1, src2, origin;
  // src1 = -9798956154578676.797564534156;
  // src2 = -2156878451.854764;

  src1.bits[0] = 0b01010100010000000011110110001100;
  src1.bits[1] = 0b10001011010100100000010101011001;
  src1.bits[2] = 0b00011111101010011000000110101101;
  src1.bits[3] = 0b10000000000011000000000000000000;

  src2.bits[0] = 0b01100101111100100100110110101100;
  src2.bits[1] = 0b00000000000001111010100110101011;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000001100000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b11010010100100100111101010001100;
  origin.bits[1] = 0b10011110100111100111000100000100;
  origin.bits[2] = 0b00011111101010011000000100111000;
  origin.bits[3] = 0b10000000000011000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest31) {
  // 1643
  decimal src1, src2, origin;
  // src1 = 0.5456465465486476846545465485;
  // src2 = 0.68985125146545154;

  src1.bits[0] = 0b00001011000011110111000010001101;
  src1.bits[1] = 0b10010110101100000101100101010001;
  src1.bits[2] = 0b00010001101000010111101110000001;
  src1.bits[3] = 0b00000000000111000000000000000000;

  src2.bits[0] = 0b00010111001001010100110000000010;
  src2.bits[1] = 0b00000000111101010001010110011011;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000100010000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b01011010101111100101111101110011;
  origin.bits[1] = 0b00101101001111010011000001101000;
  origin.bits[2] = 0b00000100101010001101010101010111;
  origin.bits[3] = 0b10000000000111000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest32) {
  // 1669
  decimal src1, src2, origin;
  // src1 = -0.77545545454546589781;
  // src2 = 87894515154546456456;

  src1.bits[0] = 0b10101100001010000001100001010101;
  src1.bits[1] = 0b00110100001010010001111010111100;
  src1.bits[2] = 0b00000000000000000000000000000100;
  src1.bits[3] = 0b10000000000101000000000000000000;

  src2.bits[0] = 0b00010011111011011111011110001000;
  src2.bits[1] = 0b11000011110010000000101111011001;
  src2.bits[2] = 0b00000000000000000000000000000100;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b10111000010101011100100001001001;
  origin.bits[1] = 0b01100110100011011000001100010111;
  origin.bits[2] = 0b00011100011001100111011011000001;
  origin.bits[3] = 0b10000000000010000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(subTest33) {
  // 1695
  decimal src1, src2, origin;
  // src1 = -7922816251426433759354395033;
  // src2 = 1;

  src1.bits[0] = 0b10011001100110011001100110011001;
  src1.bits[1] = 0b10011001100110011001100110011001;
  src1.bits[2] = 0b00011001100110011001100110011001;
  src1.bits[3] = 0b10000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {{0, 0, 0, 0}};
  sub(src1, src2, &result);

  origin.bits[0] = 0b10011001100110011001100110011010;
  origin.bits[1] = 0b10011001100110011001100110011001;
  origin.bits[2] = 0b00011001100110011001100110011001;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(origin.bits[3], result.bits[3]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[0], result.bits[0]);
}
END_TEST

START_TEST(test_decimal_sub_simple_0) {  // 10 -5 =5
  decimal c = {{10, 0, 0, 0}};
  decimal d = {{5, 0, 0, 0}};
  decimal etalon = {{5, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = sub(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_sub_simple_1) {  // 100 -5 !=5
  decimal c = {{100, 0, 0, 0}};
  decimal d = {{5, 0, 0, 0}};
  decimal etalon = {{5, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = sub(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, NO);  // *
}
END_TEST

START_TEST(test_decimal_sub_simple_2) {  // INT_MAX -5 = 2147483642
  decimal c = {{INT_MAX, 0, 0, 0}};
  decimal d = {{5, 0, 0, 0}};
  decimal etalon = {{2147483642, 0, 0, 0}};
  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = sub(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_sub_simple_3) {  // роботает только с ппервым
                                             // массивом - ложный результат
  decimal c = {{INT_MAX, INT_MAX, 0, 0}};
  decimal d = {{5, 0, 0, 0}};
  decimal etalon = {{INT_MAX - 5, INT_MAX, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = sub(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}

START_TEST(test_decimal_sub_simple_7) {  // 1-(-1)=2
  decimal c = {{1, 0, 0, 0}};
  decimal d = {{1, 0, 0, ~(INT_MAX)}};
  decimal etalon = {{2, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = sub(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_sub_simple_8) {  // -1-(-1)=0
  decimal c = {{1, 0, 0, ~(INT_MAX)}};
  decimal d = {{1, 0, 0, ~(INT_MAX)}};
  decimal etalon = {{0, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = sub(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

// START_TEST(test_decimal_sub_simple_9) {  // 1-1=0
//   decimal c = {{100, 0, 0, 0}};
//   decimal d = {{100, 0, 0, 0}};
//   decimal etalon = {{200, 0, 0, 0}};

//   decimal res = {{0, 0, 0, 0}};
//   decimal* p_res = &res;

//     // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число
//     слишком мало или равно отрицательной бесконечности 3 - деление на 0 int
//     add = sub(c, d, p_res); ck_assert_int_eq(add, 0);

//   int equal =
//       is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 -
//       TRUE
//   ck_assert_int_eq(equal, YES);
// }
// END_TEST

START_TEST(test_decimal_sub_simple_10) {  // 1-(-1)=2
  decimal c = {{100, 0, 0, 0}};
  decimal d = {{100, 0, 0, ~(INT_MAX)}};
  decimal etalon = {{200, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = sub(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_sub_simple_11) {  // -1-(-1)=0
  decimal c = {{100, 0, 0, ~(INT_MAX)}};
  decimal d = {{100, 0, 0, ~(INT_MAX)}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = sub(c, d, p_res);
  ck_assert_int_eq(add, 0);
}
END_TEST

START_TEST(add_0) {
  decimal val1 = {{15, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_1) {
  decimal val1 = {{15, 0, 0, 0}};
  decimal val2 = {{15, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_2) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal val2 = {{1, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(1, add(val1, val2, &res));
}
END_TEST

START_TEST(add_3) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(2, add(val1, val2, &res));
}
END_TEST

START_TEST(add_4) {
  decimal val1 = {{8, 0, 0, 0}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_5) {
  decimal val1 = {{2, 0, 0, 0}};
  decimal val2 = {{8, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_6) {
  decimal val1 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{2, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_7) {
  decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{8, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_8) {
  decimal val1 = {{0}};
  decimal val2 = {{0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_9) {
  decimal val1 = {{4, 0, 0, 0}};
  decimal val2 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_10) {
  decimal val1 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_11) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{4, 0, 0, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_12) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(2, add(val1, val2, &res));
}
END_TEST

START_TEST(add_13) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal val2 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_14) {
  decimal val1 = {{4, 0, 0, 0}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_15) {
  decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_16) {
  decimal val1 = {{4, 0, 0, ~(UINT_MAX / 2)}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal res;
  ck_assert_int_eq(2, add(val1, val2, &res));
}
END_TEST

START_TEST(add_17) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  _set_scale(&val1, 5);
  _set_scale(&val2, 3);
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(add_18) {
  decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  decimal val2 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  _set_scale(&val1, 5);
  _set_scale(&val2, 3);
  decimal res;
  ck_assert_int_eq(0, add(val1, val2, &res));
}
END_TEST

START_TEST(minus_overflow_test) {
  decimal a = {0};
  decimal b = {0};
  decimal res = {0};

  _set_bit(&a, 95, 1);
  _set_bit(&a, 94, 1);
  _set_bit(&b, 95, 1);
  _set_bit(&b, 94, 1);
  _set_sign(&a, 1);
  _set_sign(&b, 1);
  int code = add(a, b, &res);

  ck_assert_int_eq(code, NEG_INF);
}
END_TEST

START_TEST(overflow_test) {
  decimal a = {0};
  decimal b = {0};
  decimal res = {0};

  _set_bit(&a, 95, 1);
  _set_bit(&a, 94, 1);
  _set_bit(&b, 95, 1);
  _set_bit(&b, 94, 1);
  int code = add(a, b, &res);

  ck_assert_int_eq(code, INF);
}
END_TEST

START_TEST(test_decimal_add_0) {
  decimal c = {{5, 0, 0, 0}};
  decimal d = {{5, 0, 0, 0}};
  decimal etalon = {{10, 0, 0, 0}};
  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;
  // 0 - OK 1 - число слишком велико или равно бесконечности
  // 2 - число слишком мало или равно отрицательной бесконечности
  // 3 - деление на 0
  int add = add(c, d, p_res);
  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(add, 0);
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_1) {
  decimal c = {{100, 0, 0, 0}};
  decimal d = {{100, 0, 0, 0}};
  decimal etalon = {{100, 0, 0, 0}};
  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;
  // 0 - OK 1 - число слишком велико или равно бесконечности
  // 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  int equal = is_equal(res, etalon);
  ck_assert_int_eq(add, 0);
  ck_assert_int_eq(equal, NO);
}
END_TEST

START_TEST(test_decimal_add_2) {
  decimal c = {{2147483647, 0, 0, 0}};
  decimal d = {{2147483647, 0, 0, 0}};
  decimal etalon = {{4294967294, 0, 0, 0}};
  decimal result = {{0, 0, 0, 0}};
  decimal* p_result = &result;
  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_result);
  ck_assert_int_eq(add, 0);
  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  int equal = is_equal(result, etalon);
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_3) {
  decimal c = {{1000, 0, 0, 0}};
  decimal d = {{1000, 0, 0, 0}};
  decimal etalon = {{2000, 0, 0, 0}};
  decimal result = {{0, 0, 0, 0}};
  decimal* p_result = &result;
  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_result);
  ck_assert_int_eq(add, 0);
  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  int equal = is_equal(result, etalon);
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_4) {
  decimal c = {{1000, 0, 0, ~(INT_MAX)}};
  decimal d = {{1000, 0, 0, 0}};
  decimal etalon = {{0, 0, 0, 0}};
  decimal result = {{0, 0, 0, 0}};
  decimal* p_result = &result;
  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_result);
  ck_assert_int_eq(add, 0);
  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  int equal = is_equal(result, etalon);
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_5) {
  decimal c = {{1, 0, 0, ~(INT_MAX)}};
  decimal d = {{1, 0, 0, ~(INT_MAX)}};
  decimal etalon = {{2, 0, 0, ~(INT_MAX)}};
  decimal result = {{0, 0, 0, 0}};
  decimal* p_result = &result;
  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_result);
  ck_assert_int_eq(add, 0);
  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  int equal = is_equal(*p_result, etalon);
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_6) {
  decimal c = {{0, 0, 0, 0}};
  decimal d = {{0, 0, 0, 0}};
  decimal etalon = {{0, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_7) {
  decimal c = {{1, 0, 0, 0}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{2, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_8) {
  decimal c = {{1000, 0, 0, 0}};
  decimal d = {{1000, 0, 0, 0}};
  decimal etalon = {{2000, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_8_1) {
  decimal c = {{INT_MAX, 0, 0, 0}};
  decimal d = {{INT_MAX, 0, 0, 0}};
  decimal etalon = {{4294967294, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_9) {
  decimal c = {{INT_MAX, 0, 0, 0}};
  decimal d = {{INT_MAX, 0, 0, 0}};
  decimal etalon = {{UINT32_MAX - 1, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_10) {
  decimal c = {{UINT32_MAX, 0, 0, 0}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{0, 1, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[3], etalon.bits[3]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[0], etalon.bits[0]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_11) {
  decimal c = {{0, MAX_UINT - 1, 0, 0}};
  decimal d = {{0, 1, 0, 0}};
  decimal etalon = {{0, MAX_UINT, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_12) {
  decimal c = {{0, 0, MAX_UINT - 1, 0}};
  decimal d = {{0, 0, 1, 0}};
  decimal etalon = {{0, 0, MAX_UINT, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_13) {
  decimal c = {{0, 0, 0, ~(INT_MAX)}};
  decimal d = {{0, 0, 0, ~(INT_MAX)}};
  decimal etalon = {{0, 0, 0, ~(INT_MAX)}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_14) {
  decimal c = {{1, 0, 0, ~(INT_MAX)}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{0, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_15) {  // -1 + (-1) = -2
  decimal c = {{1, 0, 0, ~(INT_MAX)}};
  decimal d = {{1, 0, 0, ~(INT_MAX)}};
  decimal etalon = {{2, 0, 0, ~(INT_MAX)}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[3], etalon.bits[3]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[0], etalon.bits[0]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_16) {
  decimal c = {{11, 0, 0, EXPONENT_PLUS_1}};
  decimal d = {{0, 0, 0, 0}};
  decimal etalon = {{11, 0, 0, EXPONENT_PLUS_1}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_17) {
  decimal c = {{11, 0, 0, EXPONENT_PLUS_1}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{21, 0, 0, EXPONENT_PLUS_1}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_18) {
  decimal c = {{111, 0, 0, EXPONENT_PLUS_1}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{121, 0, 0, EXPONENT_PLUS_1}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_19) {
  decimal c = {{111, 0, 0, EXPONENT_PLUS_2}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{1111, 0, 0, EXPONENT_PLUS_2}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_20) {
  decimal c = {0};
  c.bits[0] = 0b11111111111111111111111111111110;
  c.bits[1] = 0b00000000000000000000000000000000;
  c.bits[2] = 0b00000000000000000000000000000000;
  c.bits[3] = 0b00000000000000000000000000000000;
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {0};
  etalon.bits[0] = 0b11111111111111111111111111111111;
  etalon.bits[1] = 0b00000000000000000000000000000000;
  etalon.bits[2] = 0b00000000000000000000000000000000;
  etalon.bits[3] = 0b00000000000000000000000000000000;
  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_21) {
  decimal c = {{UINT32_MAX - 1, UINT32_MAX, 0, 0}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{UINT32_MAX, UINT32_MAX, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_25) {
  decimal c = {{0b00000000000000000001100101111001, 0, 0, 0}};
  decimal d = {{0b00000000000000010010000110001001, 0, 0, 0}};
  decimal etalon = {{0b00000000000000010011101100000010, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_26) {
  decimal c = {{0b10001001111001111111111111111111,
                    0b10001010110001110010001100000100, 0, 0}};
  decimal d = {{0b00000000000000000000000000000001, 0, 0, 0}};
  decimal etalon = {{0b10001001111010000000000000000000,
                         0b10001010110001110010001100000100, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_27) {
  decimal c = {{MAX_UINT, MAX_UINT, 0, 0}};
  decimal d = {{0b00000000000000000000000000000001, 0, 0, 0}};
  decimal etalon = {{0, 0, 1, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_28) {
  decimal c = {
      {0b10000010111000100101101011101101, 0b11111001111010000010010110101101,
       0b10110000001111101111000010010100, 0b10000000000011100000000000000000}};
  decimal d = {{0b00000011010000001001011100101110, 0, 0, 0}};
  decimal etalon = {
      {0b01110001001010101101101011101101, 0b00101110001111001110000111111000,
       0b10110000001111101110111101101101, 0b10000000000011100000000000000000}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_29) {
  decimal c = {
      {0b10000010111000100101101011101101, 0b11111001111010000010010110101101,
       0b10110000001111101111000010010100, 0b10000000000011100000000000000000}};
  decimal d = {
      {0b01001000000110110001111110011000, 0b11111011111111011000100101101101,
       0b00000000000001000110110101110111, 0b10000000000110000000000000000000}};
  decimal etalon = {
      {0b10110110001010011011010111011010, 0b11111001111010100000110001111111,
       0b10110000001111101111000010010100, 0b10000000000011100000000000000000}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_30) {
  decimal src1, src2, origin;
  src1.bits[0] = 0b01001110111001000011100101110110;
  src1.bits[1] = 0b01001011001101011010000111011001;
  src1.bits[2] = 0b00011001101110010111010010111111;
  src1.bits[3] = 0b00000000000011110000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;

  origin.bits[0] = 0b01001110111001000011100101110110;
  origin.bits[1] = 0b01001011001101011010000111011001;
  origin.bits[2] = 0b00011001101110010111010010111111;
  origin.bits[3] = 0b00000000000011110000000000000000;

  decimal result = {{0, 0, 0, 0}};
  decimal* p_res = &result;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(src1, src2, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(origin.bits[0], result.bits[0]);
  ck_assert_int_eq(origin.bits[1], result.bits[1]);
  ck_assert_int_eq(origin.bits[2], result.bits[2]);
  ck_assert_int_eq(origin.bits[3], result.bits[3]);

  int equal = is_equal(
      result, origin);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}

END_TEST

START_TEST(test_decimal_add_simple_0) {
  decimal c = {{0, 0, 0, 0}};
  decimal d = {{0, 0, 0, 0}};
  decimal etalon = {{0, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_1) {
  decimal c = {{1, 0, 0, 0}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{2, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_2) {
  decimal c = {{1000, 0, 0, 0}};
  decimal d = {{1000, 0, 0, 0}};
  decimal etalon = {{2000, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_3) {
  decimal c = {{INT_MAX, 0, 0, 0}};
  decimal d = {{INT_MAX, 0, 0, 0}};
  decimal etalon = {{4294967294, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_4) {
  decimal c = {{INT_MAX, 0, 0, 0}};
  decimal d = {{INT_MAX, 0, 0, 0}};
  decimal etalon = {{UINT32_MAX - 1, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_5) {
  decimal c = {{MAX_UINT, 0, 0, 0}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{0, 1, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[3], etalon.bits[3]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[0], etalon.bits[0]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_6) {
  decimal c = {{0, MAX_UINT - 1, 0, 0}};
  decimal d = {{0, 1, 0, 0}};
  decimal etalon = {{0, MAX_UINT, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_7) {
  decimal c = {{0, 0, MAX_UINT - 1, 0}};
  decimal d = {{0, 0, 1, 0}};
  decimal etalon = {{0, 0, MAX_UINT, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_8) {
  decimal c = {{0, 0, 0, ~(INT_MAX)}};
  decimal d = {{0, 0, 0, ~(INT_MAX)}};
  decimal etalon = {{0, 0, 0, ~(INT_MAX)}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_9) {
  decimal c = {{1, 0, 0, ~(INT_MAX)}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{0, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_10) {  // -1 + (-1) = -2
  decimal c = {{1, 0, 0, ~(INT_MAX)}};
  decimal d = {{1, 0, 0, ~(INT_MAX)}};
  decimal etalon = {{2, 0, 0, ~(INT_MAX)}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_11) {
  decimal c = {{11, 0, 0, EXPONENT_PLUS_1}};
  decimal d = {{0, 0, 0, 0}};
  decimal etalon = {{11, 0, 0, EXPONENT_PLUS_1}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_12) {
  decimal c = {{11, 0, 0, EXPONENT_PLUS_1}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{21, 0, 0, EXPONENT_PLUS_1}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_13) {
  decimal c = {{111, 0, 0, EXPONENT_PLUS_1}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{121, 0, 0, EXPONENT_PLUS_1}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_14) {
  decimal c = {{111, 0, 0, EXPONENT_PLUS_2}};
  decimal d = {{1, 0, 0, 0}};
  decimal etalon = {{1111, 0, 0, EXPONENT_PLUS_2}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);
  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_20) {
  decimal c = {{0b00000000000000000001100101111001, 0, 0, 0}};
  decimal d = {{0b00000000000000010010000110001001, 0, 0, 0}};
  decimal etalon = {{0b00000000000000010011101100000010, 0, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_21) {
  decimal c = {{0b10001001111001111111111111111111,
                    0b10001010110001110010001100000100, 0, 0}};
  decimal d = {{0b00000000000000000000000000000001, 0, 0, 0}};
  decimal etalon = {{0b10001001111010000000000000000000,
                         0b10001010110001110010001100000100, 0, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_22) {
  decimal c = {{MAX_UINT, MAX_UINT, 0, 0}};
  decimal d = {{0b00000000000000000000000000000001, 0, 0, 0}};
  decimal etalon = {{0, 0, 1, 0}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(test_decimal_add_simple_23) {
  decimal c = {
      {0b10000010111000100101101011101101, 0b11111001111010000010010110101101,
       0b10110000001111101111000010010100, 0b10000000000011100000000000000000}};
  decimal d = {{0b00000011010000001001011100101110, 0, 0, 0}};
  decimal etalon = {
      {0b01110001001010101101101011101101, 0b00101110001111001110000111111000,
       0b10110000001111101110111101101101, 0b10000000000011100000000000000000}};

  decimal res = {{0, 0, 0, 0}};
  decimal* p_res = &res;

  int add = add(c, d, p_res);
  ck_assert_int_eq(add, 0);

  ck_assert_int_eq(res.bits[0], etalon.bits[0]);
  ck_assert_int_eq(res.bits[1], etalon.bits[1]);
  ck_assert_int_eq(res.bits[2], etalon.bits[2]);
  ck_assert_int_eq(res.bits[3], etalon.bits[3]);

  // 0 - OK 1 - число слишком велико или равно бесконечности 2 - число слишком
  // мало или равно отрицательной бесконечности 3 - деление на 0
  int equal =
      is_equal(res, etalon);  // Возвращаемое значение: 0 - FALSE 1 - TRUE
  ck_assert_int_eq(equal, YES);
}
END_TEST

START_TEST(add_max_31) {
  decimal src1 = {0};
  //  79228162514264337593543950335
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  decimal src2 = {0};
  //  0.002
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000110000000000000000;
  decimal res = {0};
  decimal true_result = {{-1, -1, -1, 0}};
  ck_assert_int_eq(add(src1, src2, &res), 0);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(res.bits[i], true_result.bits[i]);
}
END_TEST

START_TEST(add_max_32) {
  decimal src1 = {0};
  //  79228162514264337593543950335
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  decimal src2 = {0};
  //  0.5
  src2.bits[0] = 0b00000000000000000000000000000101;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000010000000000000000;
  decimal res = {0};
  ck_assert_int_eq(add(src1, src2, &res), 1);
}
END_TEST

START_TEST(add_max_33) {
  decimal src1 = {0};
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  decimal src2 = {0};
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal res = {0};
  ck_assert_int_eq(add(src1, src2, &res), 1);
}
END_TEST

START_TEST(add_max_34) {
  decimal src1 = {0};
  //  -79228162514264337593543950335
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  decimal src2 = {0};
  //  0.002
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000110000000000000000;
  decimal original_res = {0};
  original_res.bits[0] = 0b11111111111111111111111111111111;
  original_res.bits[1] = 0b11111111111111111111111111111111;
  original_res.bits[2] = 0b11111111111111111111111111111111;
  original_res.bits[3] = 0b10000000000000000000000000000000;
  decimal res = {0};
  add(src1, src2, &res);
  ck_assert_int_eq(add(src1, src2, &res), 0);
  ck_assert_int_eq(original_res.bits[0], res.bits[0]);
  ck_assert_int_eq(original_res.bits[1], res.bits[1]);
  ck_assert_int_eq(original_res.bits[2], res.bits[2]);
  ck_assert_int_eq(original_res.bits[3], res.bits[3]);
}
END_TEST

START_TEST(add_max_35) {
  decimal src1 = {0};
  //  -79228162514264337593543950335
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  decimal src2 = {0};
  //  0.5
  src2.bits[0] = 0b00000000000000000000000000000101;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000010000000000000000;
  decimal original_res = {0};
  original_res.bits[0] = 0b11111111111111111111111111111110;
  original_res.bits[1] = 0b11111111111111111111111111111111;
  original_res.bits[2] = 0b11111111111111111111111111111111;
  original_res.bits[3] = 0b10000000000000000000000000000000;
  decimal res = {0};
  add(src1, src2, &res);
  ck_assert_int_eq(add(src1, src2, &res), 0);
  ck_assert_int_eq(original_res.bits[0], res.bits[0]);
  ck_assert_int_eq(original_res.bits[1], res.bits[1]);
  ck_assert_int_eq(original_res.bits[2], res.bits[2]);
  ck_assert_int_eq(original_res.bits[3], res.bits[3]);
}
END_TEST

START_TEST(add_max_36) {
  decimal src1 = {0};
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  decimal src2 = {0};
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal original_res = {0};
  original_res.bits[0] = 0b11111111111111111111111111111110;
  original_res.bits[1] = 0b11111111111111111111111111111111;
  original_res.bits[2] = 0b11111111111111111111111111111111;
  original_res.bits[3] = 0b10000000000000000000000000000000;
  decimal res = {0};
  add(src1, src2, &res);
  ck_assert_int_eq(add(src1, src2, &res), 0);
  ck_assert_int_eq(original_res.bits[0], res.bits[0]);
  ck_assert_int_eq(original_res.bits[1], res.bits[1]);
  ck_assert_int_eq(original_res.bits[2], res.bits[2]);
  ck_assert_int_eq(original_res.bits[3], res.bits[3]);
}
END_TEST

START_TEST(add_test_37) {
  decimal src1 = {0};
  src1.bits[0] = 0b00000000000000000000001111111111;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000010000000000000000;
  decimal src2 = {0};
  src2.bits[0] = 0b00000000000000000000001111111111;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000100000000000000000;
  decimal original_res = {0};
  original_res.bits[0] = 0b00000000000000000010101111110101;
  original_res.bits[1] = 0b00000000000000000000000000000000;
  original_res.bits[2] = 0b00000000000000000000000000000000;
  original_res.bits[3] = 0b00000000000000100000000000000000;
  decimal res = {0};
  add(src1, src2, &res);
  ck_assert_int_eq(add(src1, src2, &res), 0);
  ck_assert_int_eq(original_res.bits[0], res.bits[0]);
  ck_assert_int_eq(original_res.bits[1], res.bits[1]);
  ck_assert_int_eq(original_res.bits[2], res.bits[2]);
  ck_assert_int_eq(original_res.bits[3], res.bits[3]);
}
END_TEST

START_TEST(mul_test_1) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};  // 3.0
  decimal dec_number_2 = {{30, 0, 0, 65536}};  // 3.0
  decimal real_result = {{900, 0, 0, 131072}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);

  ck_assert_int_eq(0, error_code);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_2) {
  decimal dec_number_1 = {{0, 0, 0, 65536}};        // 0.0
  decimal dec_number_2 = {{0, 0, 0, -2147418112}};  // -0.0
  decimal real_result = {{0, 0, 0, -2147352576}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);
  ck_assert_int_eq(0, error_code);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_3) {
  decimal dec_number_1 = {{0, 0, 0, 0}};  // 0
  decimal dec_number_2 = {{0, 0, 0, 0}};  // 0
  decimal real_result = {{0, 0, 0, 0}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);
  ck_assert_int_eq(0, error_code);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_4) {
  decimal dec_number_1 = {{7831926, 0, 0, 262144}};  // 783.1926
  decimal dec_number_2 = {{7830926, 0, 0, 262144}};  // 783.0926
  decimal real_result = {{-900043404, 14279, 0, 524288}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);
  ck_assert_int_eq(0, error_code);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_5) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};        // 3.0
  decimal dec_number_2 = {{30, 0, 0, -2147418112}};  // -3.0
  decimal real_result = {{900, 0, 0, -2147352576}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);

  ck_assert_int_eq(0, error_code);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_6) {
  decimal dec_number_1 = {
      {-538901499, 2, 0, -2147352576}};  // -123460003.89
  decimal dec_number_2 = {
      {-538901499, 2, 0, -2147352576}};  // -123460003.89
  decimal real_result = {{-958781415, 1129175772, 8, 262144}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);
  ck_assert_int_eq(0, error_code);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_7) {
  decimal dec_number_1 = {{1, 0, 0, 262144}};  // 0.0001
  decimal dec_number_2 = {{0, 0, 0, 0}};       // 0
  decimal real_result = {{0, 0, 0, 262144}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);
  ck_assert_int_eq(0, error_code);

  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_8) {
  decimal dec_number_1 = {{3, 0, 0, 65536}};  // 0.3
  decimal dec_number_2 = {{3, 0, 0, 65536}};  // 0.3
  _set_scale(&dec_number_2, 17);              // => 0.00000000000000003
  decimal real_result = {{9, 0, 0, 1179648}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);

  ck_assert_int_eq(0, error_code);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_9) {
  // 632323465788987654
  decimal dec_number_1 = {{-1988159226, 147224279, 0, 0}};
  // 67543278.897654243546576873
  decimal dec_number_2 = {{1823422441, 2086845379, 3661528, 1179648}};
  // => 675432788.97654243546576873
  _set_scale(&dec_number_2, 17);

  decimal real_result = {{-1524000657, 1192832152, -1979696914, 131072}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);

  ck_assert_int_eq(0, error_code);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(mul_test_10) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};  // 3.0
  decimal dec_number_2 = {{35, 0, 0, 65536}};  // 3.5
  decimal real_result = {{1050, 0, 0, 131072}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);
  ck_assert_int_eq(0, error_code);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_11) {
  decimal dec_number_1 = {{35, 0, 0, 65536}};  // 3.5
  decimal dec_number_2 = {{30, 0, 0, 65536}};  // 3.0
  _set_scale(&dec_number_1, 17);               // => 0.00000000000000035
  decimal real_result = {{1050, 0, 0, 1179648}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);

  ck_assert_int_eq(0, error_code);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_12) {
  decimal dec_number_1 = {{7831926, 0, 0, -2147221504}};  // -783.1926
  decimal dec_number_2 = {{7831926, 0, 0, -2147221504}};  // -783.1926
  decimal real_result = {{-1658051996, 14281, 0, 524288}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);

  ck_assert_int_eq(0, error_code);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_13) {
  decimal dec_number_1 = {{30, 0, 0, 65536}};        // 3.0
  decimal dec_number_2 = {{35, 0, 0, -2147221504}};  // -0.0035
  decimal real_result = {{1050, 0, 0, -2147155968}};
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);
  ck_assert_int_eq(0, error_code);
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(real_result.bits[i], result.bits[i]);
}
END_TEST

START_TEST(mul_test_14) {
  decimal dec_number_1 = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal dec_number_2 = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal result = {{0, 0, 0, 0}};

  int error_code = mul(dec_number_1, dec_number_2, &result);
  ck_assert_int_eq(1, error_code);
}
END_TEST

START_TEST(mul_test_15) {
  decimal dec_number_1 = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal dec_number_2 = {{5, 0, 0, 65536}};  // 0.5
  decimal result = {{0, 0, 0, 0}};
  decimal real_result = {{0, 0, -2147483648, -2147483648}};

  int error_code = mul(dec_number_1, dec_number_2, &result);
  ck_assert_int_eq(error_code, 0);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
}
END_TEST

START_TEST(mul_test_16) {
  decimal dec_number_1 = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal dec_number_2 = {{2, 0, 0, 196608}};  // 0.002
  decimal result = {{0, 0, 0, 0}};
  decimal real_result = {{858993459, 858993459, 858993459, -2147352576}};

  int error_code = mul(dec_number_1, dec_number_2, &result);

  ck_assert_int_eq(error_code, 0);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
}
END_TEST

START_TEST(mul_test_17) {
  decimal dec_number_1 = {
      {-1, -1, -1, 0}};  // 79228162514264337593543950335
  decimal dec_number_2 = {{2, 0, 0, 196608}};  // 0.002
  decimal result = {{0, 0, 0, 0}};
  decimal real_result = {{858993459, 858993459, 858993459, 131072}};

  int error_code = mul(dec_number_1, dec_number_2, &result);

  ck_assert_int_eq(error_code, 0);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
}
END_TEST

START_TEST(mul_test_18) {
  decimal dec_number_1 = {{1, 0, 0, 917504}};   // 0.00000000000001
  decimal dec_number_2 = {{1, 0, 0, 1179648}};  // 0.000000000000000001
  decimal result = {{0, 0, 0, 0}};
  decimal real_result = {{0, 0, 0, 1835008}};

  int error_code = mul(dec_number_1, dec_number_2, &result);

  ck_assert_int_eq(error_code, 0);
  ck_assert_int_eq(real_result.bits[0], result.bits[0]);
  ck_assert_int_eq(real_result.bits[1], result.bits[1]);
  ck_assert_int_eq(real_result.bits[2], result.bits[2]);
  ck_assert_int_eq(real_result.bits[3], result.bits[3]);
}
END_TEST

START_TEST(div_1) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 2;
  // src2 = -3;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00001010101010101010101010101011;
  origin.bits[1] = 0b00101001011011100000000110010110;
  origin.bits[2] = 0b00010101100010101000100110010100;
  origin.bits[3] = 0b10000000000111000000000000000000;

  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_2) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 3;
  // src2 = -2;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000001111;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000010000000000000000;

  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_3) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0;
  // src2 = -3;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_4) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 2;
  // src2 = 0;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  code_check = 3;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_5) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0;
  // src2 = -3;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_7) {
  // 0.087977766085185035280150025
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -6521;
  // src2 = -74121;
  src1.bits[0] = 0b00000000000000000001100101111001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000010010000110001001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11001100111001010110111000001001;
  origin.bits[1] = 0b01000001100010010010010111001101;
  origin.bits[2] = 0b00000000010010001100011000000100;
  origin.bits[3] = 0b00000000000110110000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_8) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 4;
  // src2 = -97623323;
  src1.bits[0] = 0b00000000000000000000000000000100;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000101110100011001110100011011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b10010111101101001110101000001011;
  origin.bits[1] = 0b00110110010000100101011010111111;
  origin.bits[2] = 0b00000000000000000000000000010110;
  origin.bits[3] = 0b10000000000111000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_9) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 65658654;
  // src2 = -5;
  src1.bits[0] = 0b00000011111010011101111100011110;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000101;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00000111110100111011111000111100;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000010000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_10) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -364748;
  // src2 = -1;
  src1.bits[0] = 0b00000000000001011001000011001100;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00000000000001011001000011001100;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_11) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 1;
  // src2 = -98745654321;
  src1.bits[0] = 0b00000000000000000000000000000001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b11111101101100110001110000110001;
  src2.bits[1] = 0b00000000000000000000000000010110;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b01010100010110111010010000111111;
  origin.bits[1] = 0b00000001011001111100100011001000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000111000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_12) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -9878798789;
  // src2= 3;
  src1.bits[0] = 0b01001100110100101000000111000101;
  src1.bits[1] = 0b00000000000000000000000000000010;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00000010100000101010101010101011;
  origin.bits[1] = 0b00011000110010011011111011000100;
  origin.bits[2] = 0b01101010011001101000000010110110;
  origin.bits[3] = 0b10000000000100110000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_13) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 9999999999999999999;
  // src2 = -1;
  src1.bits[0] = 0b10001001111001111111111111111111;
  src1.bits[1] = 0b10001010110001110010001100000100;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b10001001111001111111111111111111;
  origin.bits[1] = 0b10001010110001110010001100000100;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_14) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 18446744073709551615;
  // src2 = -1;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11111111111111111111111111111111;
  origin.bits[1] = 0b11111111111111111111111111111111;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_15) {
  // 19106824600.740339613619409233
  // 16
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 18446744073709551615.0;
  // src2 = -965453154;
  src1.bits[0] = 0b11111111111111111111111111110110;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b00000000000000000000000000001001;
  src1.bits[3] = 0b00000000000000010000000000000000;
  src2.bits[0] = 0b00111001100010111010010101100010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b10001010010011110101000101010001;
  origin.bits[1] = 0b00010010001101000001111111010011;
  origin.bits[2] = 0b00111101101111001100101101111101;
  origin.bits[3] = 0b10000000000100100000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_16) {
  // 9996480.733746526692069658436
  // 20
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -545454512454545.35265454545645;
  // src2 = -54564654;
  src1.bits[0] = 0b10000010111000100101101011101101;
  src1.bits[1] = 0b11111001111010000010010110101101;
  src1.bits[2] = 0b10110000001111101111000010010100;
  src1.bits[3] = 0b10000000000011100000000000000000;
  src2.bits[0] = 0b00000011010000001001011100101110;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b01111001011111001100111101000100;
  origin.bits[1] = 0b01101111110101101111010000100100;
  origin.bits[2] = 0b00100000010011001110010100100010;
  origin.bits[3] = 0b00000000000101010000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_17) {
  // 101903552307060
  // 101903552307060.199601685208
  // 10
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -545454512454545.35265454545645;
  // src2 = 5.352654545456454545645464;
  src1.bits[0] = 0b10000010111000100101101011101101;
  src1.bits[1] = 0b11111001111010000010010110101101;
  src1.bits[2] = 0b10110000001111101111000010010100;
  src1.bits[3] = 0b10000000000011100000000000000000;
  src2.bits[0] = 0b01001000000110110001111110011000;
  src2.bits[1] = 0b11111011111111011000100101101101;
  src2.bits[2] = 0b00000000000001000110110101110111;
  src2.bits[3] = 0b00000000000110000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b10100100111010110100001011011000;
  origin.bits[1] = 0b10100011110001111010011110110101;
  origin.bits[2] = 0b00000000010101000100101011101010;
  origin.bits[3] = 0b10000000000011000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_18) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 7961327845421.879754123131254;
  // src2 = 0;
  src1.bits[0] = 0b01001110111001000011100101110110;
  src1.bits[1] = 0b01001011001101011010000111011001;
  src1.bits[2] = 0b00011001101110010111010010111111;
  src1.bits[3] = 0b00000000000011110000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  code_check = 3;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_19) {
  //-0.0001 ??
  //-0.0001408450547646109187862971
  // 21
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 12345677.987654345678987654346;
  // src2 = -87654323456.9876545678987653;
  src1.bits[0] = 0b10010001000010101111010011001010;
  src1.bits[1] = 0b11000000010001011101010111110010;
  src1.bits[2] = 0b00100111111001000001101100000000;
  src1.bits[3] = 0b00000000000101010000000000000000;
  src2.bits[0] = 0b00010001110011011101000110000101;
  src2.bits[1] = 0b11110101101111000110111111000000;
  src2.bits[2] = 0b00000010110101010000111100111111;
  src2.bits[3] = 0b10000000000100000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11000110100011000111000110111011;
  origin.bits[1] = 0b01000001110101101000100101010000;
  origin.bits[2] = 0b00000000000000010010101001000000;
  origin.bits[3] = 0b10000000000111000000000000000000;

  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_20) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -12345677.987654345678987654346;
  // src2 = -87654323456.9876545678987653;
  src1.bits[0] = 0b10010001000010101111010011001010;
  src1.bits[1] = 0b11000000010001011101010111110010;
  src1.bits[2] = 0b00100111111001000001101100000000;
  src1.bits[3] = 0b10000000000101010000000000000000;
  src2.bits[0] = 0b00010001110011011101000110000101;
  src2.bits[1] = 0b11110101101111000110111111000000;
  src2.bits[2] = 0b00000010110101010000111100111111;
  src2.bits[3] = 0b10000000000100000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11000110100011000111000110111011;
  origin.bits[1] = 0b01000001110101101000100101010000;
  origin.bits[2] = 0b00000000000000010010101001000000;
  origin.bits[3] = 0b00000000000111000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_21) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 12345677.987654345678987654346;
  // src2 = 87654323456.9876545678987653;
  src1.bits[0] = 0b10010001000010101111010011001010;
  src1.bits[1] = 0b11000000010001011101010111110010;
  src1.bits[2] = 0b00100111111001000001101100000000;
  src1.bits[3] = 0b00000000000101010000000000000000;
  src2.bits[0] = 0b00010001110011011101000110000101;
  src2.bits[1] = 0b11110101101111000110111111000000;
  src2.bits[2] = 0b00000010110101010000111100111111;
  src2.bits[3] = 0b00000000000100000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11000110100011000111000110111011;
  origin.bits[1] = 0b01000001110101101000100101010000;
  origin.bits[2] = 0b00000000000000010010101001000000;
  origin.bits[3] = 0b00000000000111000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_22) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -12345677.987654345678987654346;
  // src2 = 87654323456.9876545678987653;
  src1.bits[0] = 0b10010001000010101111010011001010;
  src1.bits[1] = 0b11000000010001011101010111110010;
  src1.bits[2] = 0b00100111111001000001101100000000;
  src1.bits[3] = 0b10000000000101010000000000000000;
  src2.bits[0] = 0b00010001110011011101000110000101;
  src2.bits[1] = 0b11110101101111000110111111000000;
  src2.bits[2] = 0b00000010110101010000111100111111;
  src2.bits[3] = 0b00000000000100000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11000110100011000111000110111011;
  origin.bits[1] = 0b01000001110101101000100101010000;
  origin.bits[2] = 0b00000000000000010010101001000000;
  origin.bits[3] = 0b10000000000111000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_23) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0;
  // src2 = 0;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  code_check = 3;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_24) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0;
  // src2 = 0;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  code_check = 3;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_25) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0;
  // src2 = 0;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  code_check = 3;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_26) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 79228162514264337593543950335;
  // src2 = 1;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11111111111111111111111111111111;
  origin.bits[1] = 0b11111111111111111111111111111111;
  origin.bits[2] = 0b11111111111111111111111111111111;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_27) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 7922816251427554395;
  // src2 = -65645646;
  src1.bits[0] = 0b01011111000010000000010001011011;
  src1.bits[1] = 0b01101101111100110111111101100111;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000011111010011010110001001110;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b01101001110011011000100110011100;
  origin.bits[1] = 0b01100010100101110010111010100011;
  origin.bits[2] = 0b00100110111111110100110000011011;
  origin.bits[3] = 0b10000000000100010000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_28) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 665464545;
  // src2 = -8798232189789785;
  src1.bits[0] = 0b00100111101010100010111011100001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00010000010100101110011001011001;
  src2.bits[1] = 0b00000000000111110100000111110010;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b01010010000011000001010110110110;
  origin.bits[1] = 0b00000000101000000110010011001100;
  origin.bits[2] = 0b00000000000000000000000000101001;
  origin.bits[3] = 0b10000000000111000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_29) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 2.7986531268974139743;
  // src2 = -9.979623121254565121244554;
  src1.bits[0] = 0b11111110100100001101100101011111;
  src1.bits[1] = 0b10000100011001000010000111101000;
  src1.bits[2] = 0b00000000000000000000000000000001;
  src1.bits[3] = 0b00000000000100110000000000000000;
  src2.bits[0] = 0b11010100101011100011110110001010;
  src2.bits[1] = 0b01110100000101000010011110100011;
  src2.bits[2] = 0b00000000000010000100000101000100;
  src2.bits[3] = 0b10000000000110000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b10110000000110101011000111000111;
  origin.bits[1] = 0b10100110110111100111111001101001;
  origin.bits[2] = 0b00001001000011111011011111101101;
  origin.bits[3] = 0b10000000000111000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_30) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -9798956154578676.797564534156;
  // src2 = 2156878451.854764;
  src1.bits[0] = 0b01010100010000000011110110001100;
  src1.bits[1] = 0b10001011010100100000010101011001;
  src1.bits[2] = 0b00011111101010011000000110101101;
  src1.bits[3] = 0b10000000000011000000000000000000;
  src2.bits[0] = 0b01100101111100100100110110101100;
  src2.bits[1] = 0b00000000000001111010100110101011;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000001100000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11110001110100111110110111100000;
  origin.bits[1] = 0b10001101111000011110011001011110;
  origin.bits[2] = 0b10010010110010111100110101011011;
  origin.bits[3] = 0b10000000000101100000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_31) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0.5456465465486476846545465485;
  // src2 = -0.68985125146545154;
  src1.bits[0] = 0b00001011000011110111000010001101;
  src1.bits[1] = 0b10010110101100000101100101010001;
  src1.bits[2] = 0b00010001101000010111101110000001;
  src1.bits[3] = 0b00000000000111000000000000000000;
  src2.bits[0] = 0b00010111001001010100110000000010;
  src2.bits[1] = 0b00000000111101010001010110011011;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000100010000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11111100011000000110000010011111;
  origin.bits[1] = 0b00110010101101100111001011100101;
  origin.bits[2] = 0b00011001100011101011000001111010;
  origin.bits[3] = 0b10000000000111000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_32) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -0.77545545454546589781;
  // src2 = -87894515154546456456;
  src1.bits[0] = 0b10101100001010000001100001010101;
  src1.bits[1] = 0b00110100001010010001111010111100;
  src1.bits[2] = 0b00000000000000000000000000000100;
  src1.bits[3] = 0b10000000000101000000000000000000;
  src2.bits[0] = 0b00010011111011011111011110001000;
  src2.bits[1] = 0b11000011110010000000101111011001;
  src2.bits[2] = 0b00000000000000000000000000000100;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00000101010000100011011110011101;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000111000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_33) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -79228162514264337593543950335;
  // src2 = -1;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11111111111111111111111111111111;
  origin.bits[1] = 0b11111111111111111111111111111111;
  origin.bits[2] = 0b11111111111111111111111111111111;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_34) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 32323465788987654;
  // src2 = -67543278.89765424354657687;
  src1.bits[0] = 0b01011000010000110001100100000110;
  src1.bits[1] = 0b00000000011100101101011000000101;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b10111110000100011000010110010111;
  src2.bits[1] = 0b11011001001111010001001011000110;
  src2.bits[2] = 0b00000000000001011001011001001000;
  src2.bits[3] = 0b10000000000100010000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b10000111000101101000010100101100;
  origin.bits[1] = 0b10011100110000101101111000110000;
  origin.bits[2] = 0b10011010101000010111111111010101;
  origin.bits[3] = 0b10000000000101000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_35) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -784515454.7989898652154545652;
  // src2 = 579895323215489956.67897455465;
  src1.bits[0] = 0b10110001001110110100010111110100;
  src1.bits[1] = 0b00111000100000111010110010000001;
  src1.bits[2] = 0b00011001010110010101110000011000;
  src1.bits[3] = 0b10000000000100110000000000000000;
  src2.bits[0] = 0b11100110001001011001001101101001;
  src2.bits[1] = 0b00111000110110101110001010110100;
  src2.bits[2] = 0b10111011010111111101000100011110;
  src2.bits[3] = 0b00000000000010110000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00000010000110000000100111101110;
  origin.bits[1] = 0b10111011101111110010001011011011;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000111000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_36) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0;
  // src2 = 37986322154988653.784354545765;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b01001111101101000110000001100101;
  src2.bits[1] = 0b01000010001101101011011001100001;
  src2.bits[2] = 0b01111010101111011000110010011010;
  src2.bits[3] = 0b00000000000011000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_37) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 79228162514264337593543950335;
  // src2 = -1;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11111111111111111111111111111111;
  origin.bits[1] = 0b11111111111111111111111111111111;
  origin.bits[2] = 0b11111111111111111111111111111111;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_38) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 79228162514264337593543950300;
  // src2 = -991.2;
  src1.bits[0] = 0b11111111111111111111111111011100;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000010011010111000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000010000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11100010101010010000001110000000;
  origin.bits[1] = 0b01010101111101000000010000100001;
  origin.bits[2] = 0b00011001110100111100100010100000;
  origin.bits[3] = 0b10000000000000100000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_39) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -79228162514264337593543950335;
  // src2 = 1;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11111111111111111111111111111111;
  origin.bits[1] = 0b11111111111111111111111111111111;
  origin.bits[2] = 0b11111111111111111111111111111111;
  origin.bits[3] = 0b10000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_40) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = -79228162514264337593543950300;
  // src2 = 991.2;
  src1.bits[0] = 0b11111111111111111111111111011100;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000010011010111000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000010000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b11100010101010010000001110000000;
  origin.bits[1] = 0b01010101111101000000010000100001;
  origin.bits[2] = 0b00011001110100111100100010100000;
  origin.bits[3] = 0b10000000000000100000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_41) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0.0;
  // src2 = 0.0;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000010000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000010000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  code_check = 3;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_42) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 1;
  // src2 = 0.0;
  src1.bits[0] = 0b00000000000000000000000000000001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000010000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  code_check = 3;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_43) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 1;
  // src2 = 0;
  src1.bits[0] = 0b00000000000000000000000000000001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  code_check = 3;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_44) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0;
  // src2 = 0;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  code_check = 3;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_45) {
  decimal src1, src2;
  int code_check = 0;
  int code = 0;
  // src1 = 79228162514264337593543950335;
  // src2 = 0.0000006251426433759354395034;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000001010110011001100110011010;
  src2.bits[1] = 0b11100011111110000011010000000111;
  src2.bits[2] = 0b00000000000000000000000101010010;
  src2.bits[3] = 0b00000000000111000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  code_check = 1;
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(div_46) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0.0000000000000000000000000001;
  // src2 = 0.0000006251426433759354395034;
  src1.bits[0] = 0b00000000000000000000000000000001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000111000000000000000000;
  src2.bits[0] = 0b00000001010110011001100110011010;
  src2.bits[1] = 0b11100011111110000011010000000111;
  src2.bits[2] = 0b00000000000000000000000101010010;
  src2.bits[3] = 0b00000000000111000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00000000000110000110100010010011;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000111000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_47) {
  decimal src1, src2, origin;
  int code_check = 0;
  int code = 0;
  // src1 = 79228162514264337593543950335;
  // src2 = 79228162514264337593543950335;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b11111111111111111111111111111111;
  src2.bits[1] = 0b11111111111111111111111111111111;
  src2.bits[2] = 0b11111111111111111111111111111111;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  ck_assert_int_eq(code_check, code);
  for (int i = 0; i < 4; i++) ck_assert_int_eq(origin.bits[i], result.bits[i]);
}
END_TEST

START_TEST(div_48) {
  decimal src1, src2;
  int code_check = 0;
  int code = 0;
  // src1 = 79228162514264337593543950335;
  // src2 = 0.7922816251426433759354395034;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b10011001100110011001100110011010;
  src2.bits[1] = 0b10011001100110011001100110011001;
  src2.bits[2] = 0b00011001100110011001100110011001;
  src2.bits[3] = 0b00000000000111000000000000000000;
  decimal result = {0};
  code = div(src1, src2, &result);
  code_check = 1;
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(div_49) {
  // -79228162514264337593543950335
  // 0.5
  decimal src1 = {0};
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  decimal src2 = {0};
  src2.bits[0] = 0b00000000000000000000000000000101;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000010000000000000000;
  decimal res = {0};
  ck_assert_int_eq(div(src1, src2, &res), 2);
}
END_TEST

START_TEST(div_50) {
  // 79228162514264337593543950335
  // -79228162514264337593543950335
  // 1
  decimal src1 = {0};
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  decimal src2 = {0};

  src2.bits[0] = 0b00000000000000000000000000000101;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000010000000000000000;
  decimal res = {0};
  ck_assert_int_eq(div(src1, src2, &res), 1);
}
END_TEST

START_TEST(div_51) {
  decimal src1 = {{2, 0, 0, 0}}, origin;
  // src1 = 2.0;
  // src2 = 2;

  decimal src2 = {{20, 0, 0, 65536}};

  decimal result = {0};
  div(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(sub_20) {
  decimal src1 = {{-1, -1, -1, 0}};  // 79228162514264337593543950335
  decimal origin = {{-1, -1, -1, 0}};
  decimal src2 = {{3, 0, 0, -2147418112}};  // -0.3

  decimal result = {0};
  int code = sub(src1, src2, &result);
  int check = is_equal(origin, result);
  ck_assert_int_eq(0, code);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(sub_21) {
  decimal src1 = {{3, 0, 0, 65536}};  // 0.3
  decimal src2 = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal origin = {{-1, -1, -1, -2147483648}};

  decimal result = {0};
  int code = sub(src1, src2, &result);
  int check = is_equal(origin, result);
  ck_assert_int_eq(0, code);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(sub_22) {
  decimal src1 = {{3, 0, 0, -2147418112}};  // -0.3
  decimal src2 = {{-1, -1, -1, 0}};         // 79228162514264337593543950335
  decimal origin = {{-1, -1, -1, 0}};

  decimal result = {0};
  int code = sub(src1, src2, &result);
  int check = is_equal(origin, result);
  ck_assert_int_eq(0, code);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(sub_24) {
  decimal src1 = {{3, 0, 0, -2147418112}};  // -0.3
  decimal src2 = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal origin = {{-1, -1, -1, -2147483648}};

  decimal result = {0};
  int code = sub(src1, src2, &result);
  int check = is_equal(origin, result);
  ck_assert_int_eq(0, code);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(sub_25) {
  decimal src1 = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  decimal src2 = {{3, 0, 0, -2147418112}};  // -0.3
  decimal origin = {{-1, -1, -1, -2147483648}};

  decimal result = {0};
  int code = sub(src1, src2, &result);
  int check = is_equal(origin, result);
  ck_assert_int_eq(0, code);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_1) {
  decimal src1, src2, origin;
  // src1 = 2;
  // src2 = 3;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000010;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_2) {
  decimal src1, src2, origin;
  // src1 = 3;
  // src2 = 2;
  src1.bits[0] = 0b00000000000000000000000000000011;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);

  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_3) {
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = 3;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_4) {
  decimal src1, src2, origin;
  // src1 = 2;
  // src2 = 0.01;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000100000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000100000000000000000;

  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_5) {
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = 3;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_6) {
  decimal src1, src2, origin;
  // src1 = 2;
  // src2 = -0.72364;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000010001101010101100;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000001010000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000001101011111101000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000001010000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_7) {
  decimal src1, src2, origin;
  // src1 = 6521;
  // src2 = 74121;
  src1.bits[0] = 0b00000000000000000001100101111001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000010010000110001001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000001100101111001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_8) {
  decimal src1, src2, origin;
  // src1 = 4;
  // src2 = 97623323;
  src1.bits[0] = 0b00000000000000000000000000000100;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000101110100011001110100011011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000100;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_9) {
  decimal src1, src2, origin;
  // src1 = 65658654;
  // src2 = 5;
  src1.bits[0] = 0b00000011111010011101111100011110;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000101;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000100;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_10) {
  decimal src1, src2, origin;
  // src1 = -364748;
  // src2 = 1;
  src1.bits[0] = 0b00000000000001011001000011001100;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_11) {
  decimal src1, src2, origin;
  // src1 = 1;
  // src2 = 98745654321;
  src1.bits[0] = 0b00000000000000000000000000000001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b11111101101100110001110000110001;
  src2.bits[1] = 0b00000000000000000000000000010110;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_12) {
  decimal src1, src2, origin;
  // src1 = -9878798789;
  // src2 = -3;
  src1.bits[0] = 0b01001100110100101000000111000101;
  src1.bits[1] = 0b00000000000000000000000000000010;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000010;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_13) {
  decimal src1, src2, origin;
  // src1 = 9999999999999999999;
  // src2 = 1;
  src1.bits[0] = 0b10001001111001111111111111111111;
  src1.bits[1] = 0b10001010110001110010001100000100;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_14) {
  decimal src1, src2, origin;
  // src1 = 18446744073709551615;
  // src2 = 1;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_15) {
  decimal src1, src2, origin;
  // src1 = 18446744073709551615.0;
  // src2 = 965453154;
  src1.bits[0] = 0b11111111111111111111111111110110;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b00000000000000000000000000001001;
  src1.bits[3] = 0b00000000000000010000000000000000;
  src2.bits[0] = 0b00111001100010111010010101100010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b10101010000010000011011000010110;
  origin.bits[1] = 0b00000000000000000000000000000001;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000010000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_16) {
  decimal src1, src2, origin;
  // src1 = -545454512454545.35265454545645;
  // src2 = 54564654;
  src1.bits[0] = 0b10000010111000100101101011101101;
  src1.bits[1] = 0b11111001111010000010010110101101;
  src1.bits[2] = 0b10110000001111101111000010010100;
  src1.bits[3] = 0b10000000000011100000000000000000;
  src2.bits[0] = 0b00000011010000001001011100101110;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b10111101010000100101101011101101;
  origin.bits[1] = 0b00001001111110101010011110000010;
  origin.bits[2] = 0b00000000000000000000000011011001;
  origin.bits[3] = 0b10000000000011100000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_17) {
  decimal src1, src2, result;
  // src1 = 79228162514264337593543950335;
  // src2 = -79228162514264337593543950335;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b11111111111111111111111111111111;
  src2.bits[1] = 0b11111111111111111111111111111111;
  src2.bits[2] = 0b11111111111111111111111111111111;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal origin = {{0, 0, 0, 0}};
  ck_assert_int_eq(0, mod(src1, src2, &result));
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_18) {
  decimal src1, src2, origin;
  // src1 = 7961327845421.879754123131254;
  // src2 = -385.37543;
  src1.bits[0] = 0b01001110111001000011100101110110;
  src1.bits[1] = 0b01001011001101011010000111011001;
  src1.bits[2] = 0b00011001101110010111010010111111;
  src1.bits[3] = 0b00000000000011110000000000000000;
  src2.bits[0] = 0b00000010010011000000100101000111;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000001010000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b01111110001110110011000101110110;
  origin.bits[1] = 0b00000010010111010100011100000110;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000011110000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_19) {
  decimal src1, src2, origin;
  // src1 = 12345677.987654345678987654346;
  // src2 = 87654323456.9876545678987653;
  src1.bits[0] = 0b10010001000010101111010011001010;
  src1.bits[1] = 0b11000000010001011101010111110010;
  src1.bits[2] = 0b00100111111001000001101100000000;
  src1.bits[3] = 0b00000000000101010000000000000000;
  src2.bits[0] = 0b00010001110011011101000110000101;
  src2.bits[1] = 0b11110101101111000110111111000000;
  src2.bits[2] = 0b00000010110101010000111100111111;
  src2.bits[3] = 0b00000000000100000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b10010001000010101111010011001010;
  origin.bits[1] = 0b11000000010001011101010111110010;
  origin.bits[2] = 0b00100111111001000001101100000000;
  origin.bits[3] = 0b00000000000101010000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_20) {
  decimal src1, src2, origin;
  // src1 = -12345677.987654345678987654346;
  // src2 = 87654323456.9876545678987653;
  src1.bits[0] = 0b10010001000010101111010011001010;
  src1.bits[1] = 0b11000000010001011101010111110010;
  src1.bits[2] = 0b00100111111001000001101100000000;
  src1.bits[3] = 0b10000000000101010000000000000000;
  src2.bits[0] = 0b00010001110011011101000110000101;
  src2.bits[1] = 0b11110101101111000110111111000000;
  src2.bits[2] = 0b00000010110101010000111100111111;
  src2.bits[3] = 0b00000000000100000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b10010001000010101111010011001010;
  origin.bits[1] = 0b11000000010001011101010111110010;
  origin.bits[2] = 0b00100111111001000001101100000000;
  origin.bits[3] = 0b10000000000101010000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_21) {
  decimal src1, src2, origin;
  // src1 = 12345677.987654345678987654346;
  // src2 = -87654323456.9876545678987653;
  src1.bits[0] = 0b10010001000010101111010011001010;
  src1.bits[1] = 0b11000000010001011101010111110010;
  src1.bits[2] = 0b00100111111001000001101100000000;
  src1.bits[3] = 0b00000000000101010000000000000000;
  src2.bits[0] = 0b00010001110011011101000110000101;
  src2.bits[1] = 0b11110101101111000110111111000000;
  src2.bits[2] = 0b00000010110101010000111100111111;
  src2.bits[3] = 0b10000000000100000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b10010001000010101111010011001010;
  origin.bits[1] = 0b11000000010001011101010111110010;
  origin.bits[2] = 0b00100111111001000001101100000000;
  origin.bits[3] = 0b00000000000101010000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_22) {
  decimal src1, src2, origin;
  // src1 = -12345677.987654345678987654346;
  // src2 = -87654323456.9876545678987653;
  src1.bits[0] = 0b10010001000010101111010011001010;
  src1.bits[1] = 0b11000000010001011101010111110010;
  src1.bits[2] = 0b00100111111001000001101100000000;
  src1.bits[3] = 0b10000000000101010000000000000000;
  src2.bits[0] = 0b00010001110011011101000110000101;
  src2.bits[1] = 0b11110101101111000110111111000000;
  src2.bits[2] = 0b00000010110101010000111100111111;
  src2.bits[3] = 0b10000000000100000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b10010001000010101111010011001010;
  origin.bits[1] = 0b11000000010001011101010111110010;
  origin.bits[2] = 0b00100111111001000001101100000000;
  origin.bits[3] = 0b10000000000101010000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_23) {
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = 49;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000110001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_24) {
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = 0.284;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000100011100;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000110000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_25) {
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = -3.3;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000100001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000010000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_26) {
  decimal src1, src2, origin;
  // src1 = 79228162514264337593543950335;
  // src2 = -1;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_27) {
  decimal src1, src2, origin;
  // src1 = 7922816251427554395;
  // src2 = 65645646;
  src1.bits[0] = 0b01011111000010000000010001011011;
  src1.bits[1] = 0b01101101111100110111111101100111;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000011111010011010110001001110;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000001001111101101001101110111;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_28) {
  decimal src1, src2, origin;
  // src1 = 665464545;
  // src2 = 8798232189789785;
  src1.bits[0] = 0b00100111101010100010111011100001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00010000010100101110011001011001;
  src2.bits[1] = 0b00000000000111110100000111110010;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00100111101010100010111011100001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_29) {
  decimal src1, src2, origin;
  // src1 = 2.7986531268974139743;
  // src2 = 9.979623121254565121244554;
  src1.bits[0] = 0b11111110100100001101100101011111;
  src1.bits[1] = 0b10000100011001000010000111101000;
  src1.bits[2] = 0b00000000000000000000000000000001;
  src1.bits[3] = 0b00000000000100110000000000000000;
  src2.bits[0] = 0b11010100101011100011110110001010;
  src2.bits[1] = 0b01110100000101000010011110100011;
  src2.bits[2] = 0b00000000000010000100000101000100;
  src2.bits[3] = 0b00000000000110000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b11111110100100001101100101011111;
  origin.bits[1] = 0b10000100011001000010000111101000;
  origin.bits[2] = 0b00000000000000000000000000000001;
  origin.bits[3] = 0b00000000000100110000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_30) {
  decimal src1, src2, origin;
  // src1 = -9798956154578676.797564534156;
  // src2 = -2156878451.854764;
  src1.bits[0] = 0b01010100010000000011110110001100;
  src1.bits[1] = 0b10001011010100100000010101011001;
  src1.bits[2] = 0b00011111101010011000000110101101;
  src1.bits[3] = 0b10000000000011000000000000000000;
  src2.bits[0] = 0b01100101111100100100110110101100;
  src2.bits[1] = 0b00000000000001111010100110101011;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000001100000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b10101100101110100101000010001100;
  origin.bits[1] = 0b11010010101110000010010011000101;
  origin.bits[2] = 0b00000000000000000000000000100100;
  origin.bits[3] = 0b10000000000011000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_31) {
  decimal src1, src2, origin;
  // src1 = 0.5456465465486476846545465485;
  // src2 = 0.68985125146545154;
  src1.bits[0] = 0b00001011000011110111000010001101;
  src1.bits[1] = 0b10010110101100000101100101010001;
  src1.bits[2] = 0b00010001101000010111101110000001;
  src1.bits[3] = 0b00000000000111000000000000000000;
  src2.bits[0] = 0b00010111001001010100110000000010;
  src2.bits[1] = 0b00000000111101010001010110011011;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000100010000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00001011000011110111000010001101;
  origin.bits[1] = 0b10010110101100000101100101010001;
  origin.bits[2] = 0b00010001101000010111101110000001;
  origin.bits[3] = 0b00000000000111000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_32) {
  decimal src1, src2, origin;
  // src1 = -0.77545545454546589781;
  // src2 = 87894515154546456456;
  src1.bits[0] = 0b10101100001010000001100001010101;
  src1.bits[1] = 0b00110100001010010001111010111100;
  src1.bits[2] = 0b00000000000000000000000000000100;
  src1.bits[3] = 0b10000000000101000000000000000000;
  src2.bits[0] = 0b00010011111011011111011110001000;
  src2.bits[1] = 0b11000011110010000000101111011001;
  src2.bits[2] = 0b00000000000000000000000000000100;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b10101100001010000001100001010101;
  origin.bits[1] = 0b00110100001010010001111010111100;
  origin.bits[2] = 0b00000000000000000000000000000100;
  origin.bits[3] = 0b10000000000101000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_33) {
  decimal src1, src2, origin;
  // src1 = -79228162514264337593543950335;
  // src2 = -1;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000000000001;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b10000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_34) {
  decimal src1, src2, result;
  // src1 = 79228162514264337593543950335;
  // src2 = 0.02222;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000100010101110;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000001010000000000000000;
  ck_assert_int_eq(1, mod(src1, src2, &result));
}
END_TEST

START_TEST(mod_35) {
  decimal src1, src2, origin;
  // src1 = -784515454.7989898652154545652;
  // src2 = -579895323215489956.67897455465;
  src1.bits[0] = 0b10110001001110110100010111110100;
  src1.bits[1] = 0b00111000100000111010110010000001;
  src1.bits[2] = 0b00011001010110010101110000011000;
  src1.bits[3] = 0b10000000000100110000000000000000;
  src2.bits[0] = 0b11100110001001011001001101101001;
  src2.bits[1] = 0b00111000110110101110001010110100;
  src2.bits[2] = 0b10111011010111111101000100011110;
  src2.bits[3] = 0b10000000000010110000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b10110001001110110100010111110100;
  origin.bits[1] = 0b00111000100000111010110010000001;
  origin.bits[2] = 0b00011001010110010101110000011000;
  origin.bits[3] = 0b10000000000100110000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_36) {
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = -37986322154988653.784354545765;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b01001111101101000110000001100101;
  src2.bits[1] = 0b01000010001101101011011001100001;
  src2.bits[2] = 0b01111010101111011000110010011010;
  src2.bits[3] = 0b10000000000011000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_49) {
  decimal src1, src2, origin;
  // src1 = 0.0;
  // src2 = 0.083;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000010000000000000000;
  src2.bits[0] = 0b00000000000000000000000001010011;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000110000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000010000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_50) {
  decimal src1, src2, origin;
  // src1 = 1;
  // src2 = 0.02222;
  src1.bits[0] = 0b00000000000000000000000000000001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000100010101110;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000001010000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000001010;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000001010000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_51) {
  decimal src1, src2, origin;
  // src1 = 1;
  // src2 = 111111;
  src1.bits[0] = 0b00000000000000000000000000000001;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000011011001000000111;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_52) {
  decimal src1, src2, origin;
  // src1 = 0;
  // src2 = 423;
  src1.bits[0] = 0b00000000000000000000000000000000;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;
  src2.bits[0] = 0b00000000000000000000000110100111;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_53) {
  decimal src1, src2, origin;
  // src1 = 2;
  // src2 = 0;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000000;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_54) {
  decimal src1, src2, origin;
  // src1 = 2;
  // src2 = 0;
  src1.bits[0] = 0b00000000000000000000000000000010;
  src1.bits[1] = 0b00000000000000000000000000000000;
  src1.bits[2] = 0b00000000000000000000000000000000;
  src1.bits[3] = 0b00000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000000000000010;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000000000000000000000000;
  decimal result = {0};
  mod(src1, src2, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(mod_55) {
  decimal src1, src2, result;
  // src1 = -79228162514264337593543950335;
  // src2 = 0.02222;
  src1.bits[0] = 0b11111111111111111111111111111111;
  src1.bits[1] = 0b11111111111111111111111111111111;
  src1.bits[2] = 0b11111111111111111111111111111111;
  src1.bits[3] = 0b10000000000000000000000000000000;

  src2.bits[0] = 0b00000000000000000000100010101110;
  src2.bits[1] = 0b00000000000000000000000000000000;
  src2.bits[2] = 0b00000000000000000000000000000000;
  src2.bits[3] = 0b00000000000001010000000000000000;
  ck_assert_int_eq(2, mod(src1, src2, &result));
}
END_TEST

//|||||||||||||||||||||   \ARITHMETIC     |||||||||||||||||||||

//|||||||||||||||||||||    PARSERS     |||||||||||||||||||||
START_TEST(float_to_dec_1) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 2;

  float num = 2;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000010;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_2) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -2;

  float num = -2;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000010;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_3) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 7.922817E+28;

  float num = 7.922817E+28;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  code_check = 1;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000010000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_4) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 7.922816E+28;

  float num = 7.922816E+28;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00100000000000000000000000000000;
  origin.bits[1] = 0b10110011100100101011001000011010;
  origin.bits[2] = 0b11111111111111111111111101110111;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_5) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -7.922817E+28;

  float num = -7.922817E+28;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  code_check = 1;
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000010000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_6) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -7.922816E+28;

  float num = -7.922816E+28;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00100000000000000000000000000000;
  origin.bits[1] = 0b10110011100100101011001000011010;
  origin.bits[2] = 0b11111111111111111111111101110111;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_7) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 200;

  float num = 200;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000011001000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_8) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -200.3;

  float num = -200.3;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000011111010011;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000010000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_9) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -200;

  float num = -200;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000011001000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_10) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 200;

  float num = 200;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000011001000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_11) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0;

  float num = 0;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_12) {
  decimal origin;
  int code_check = 0;
  int code = 0;

  float num = 0.0;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_13) {
  decimal origin = {{626856384, 546, 0, 0}};
  int code_check = 0;
  int code = 0;

  float num = 2345678654320.567908764;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_14) {
  decimal origin = {{5679088, 0, 0, 458752}};
  int code_check = 0;
  int code = 0;

  float num = 0.567908764;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_15) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 7.922816E+20;

  float num = 7.922816E+20;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00011110001000000000000000000000;
  origin.bits[1] = 0b11110011000111011010110110000011;
  origin.bits[2] = 0b00000000000000000000000000101010;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_16) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -7.922816E+20;

  float num = -7.922816E+20;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00011110001000000000000000000000;
  origin.bits[1] = 0b11110011000111011010110110000011;
  origin.bits[2] = 0b00000000000000000000000000101010;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_17) {
  int code_check = 1;
  int code = 0;
  float num = -7.922916E+28;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_18) {
  int code_check = 1;
  int code = 0;
  float num = -7.922916E+28;
  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_19) {
  decimal origin = {{-964689920, -81569408, 429502136, 0}};
  int code_check = 0;
  int code = 0;
  float num = 0.7922916e+28;
  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  int check = is_equal(origin, result);
  ck_assert_int_eq(code_check, code);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(float_to_dec_21) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 1E-28;

  float num = 1E-28;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000111000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(code_check, code);
  ck_assert_int_eq(1, check);
}
END_TEST

START_TEST(float_to_dec_22) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -1E-28;

  float num = -1E-28;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000111000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_23) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 1E-29;

  float num = 1E-29;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  code_check = 1;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_to_dec_24) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -1E-29;

  float num = -1E-29;

  decimal result = {0};
  code = from_float_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  code_check = 1;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_1) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000010;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 2;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_2) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000010;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = -2;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_5) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000011001000;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 200;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_6) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000011001000;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = -200;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_7) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000000;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 0;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_8) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000000;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 0;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_9) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000011110100001000111111;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 999999;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_10) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000011110100001000111111;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = -999999;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_11) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b01111111111111111111111111111111;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 2147483647;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_12) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b10000000000000000000000000000000;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = -2147483648;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_13) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000001;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 1;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_14) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000001;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = -1;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_15) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000001011;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000010000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 1.1;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_16) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000001011;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000010000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = -1.1;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_17) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b11000001001100001100000011001011;
  num1.bits[1] = 0b00000000000000001011010001101011;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 198374895698123;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_18) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b11000001001100001100000011001011;
  num1.bits[1] = 0b00000000000000001011010001101011;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = -198374895698123;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_19) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b11111111111111111111111111111111;
  num1.bits[1] = 0b11111111111111111111111111111111;
  num1.bits[2] = 0b11111111111111111111111111111111;
  num1.bits[3] = 0b00000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 79228162514264337593543950335.0;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_20) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b11111111111111111111111111111111;
  num1.bits[1] = 0b11111111111111111111111111111111;
  num1.bits[2] = 0b11111111111111111111111111111111;
  num1.bits[3] = 0b10000000000000000000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = -79228162514264337593543950335.0;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_21) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b11111111111111111111111111111111;
  num1.bits[1] = 0b11111111111111111111111111111111;
  num1.bits[2] = 0b11111111111111111111111111111111;
  num1.bits[3] = 0b00000000000011100000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 792281625142643.37593543950335;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_22) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b11111111111111111111111111111111;
  num1.bits[1] = 0b11111111111111111111111111111111;
  num1.bits[2] = 0b11111111111111111111111111111111;
  num1.bits[3] = 0b10000000000011100000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = -792281625142643.37593543950335;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_23) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b01110010010111001101110110100001;
  num1.bits[1] = 0b00000000000000000000000000010010;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000001010000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = 792281.00001;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_float_24) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b01110010010111001101110110100001;
  num1.bits[1] = 0b00000000000000000000000000010010;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000001010000000000000000;
  float result = 0;
  code = from_decimal_to_float(num1, &result);
  float origin = -792281.00001;
  ck_assert_float_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_1) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000010;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = 2;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_2) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000010;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = -2;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_5) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000011001000;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = 200;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_6) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000011001000;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = -200;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_7) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000000;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = 0;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_8) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000000;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = 0;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_9) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000011110100001000111111;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = 999999;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_10) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000011110100001000111111;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = -999999;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_11) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b01111111111111111111111111111111;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = 2147483647;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_12) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b10000000000000000000000000000000;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = -2147483648;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_13) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000001;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = 1;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_14) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000000001;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = -1;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_15) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000001011;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000010000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = 1;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_16) {
  int code_check = 0;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b00000000000000000000000000001011;
  num1.bits[1] = 0b00000000000000000000000000000000;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000010000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  int origin = -1;
  ck_assert_int_eq(origin, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_17) {
  int code_check = 1;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b11000001001100001100000011001011;
  num1.bits[1] = 0b00000000000000001011010001101011;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b00000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_18) {
  int code_check = 1;
  int code = 0;
  decimal num1 = {0};

  num1.bits[0] = 0b11000001001100001100000011001011;
  num1.bits[1] = 0b00000000000000001011010001101011;
  num1.bits[2] = 0b00000000000000000000000000000000;
  num1.bits[3] = 0b10000000000000000000000000000000;
  int result = 0;
  code = from_decimal_to_int(num1, &result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(dec_to_int_19) {
  int code_check = 1;
  int code = 0;
  decimal num1 = {0};
  int* result = NULL;
  code = from_decimal_to_int(num1, result);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_1) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 2;

  int num = 2;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000010;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_2) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -2;

  int num = -2;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000010;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_5) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 200;

  int num = 200;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000011001000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_6) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -200;

  int num = -200;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000011001000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_7) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0;

  int num = 0;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_8) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 0;

  int num = 0;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_9) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 999999;

  int num = 999999;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000011110100001000111111;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_10) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -999999;

  int num = -999999;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000011110100001000111111;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_11) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 2147483647;

  int num = 2147483647;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b01111111111111111111111111111111;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_12) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -2147483648;

  int num = -2147483648;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b10000000000000000000000000000000;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_13) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = 1;

  int num = 1;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b00000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(int_to_dec_14) {
  decimal origin;
  int code_check = 0;
  int code = 0;
  // src1 = -1;

  int num = -1;

  decimal result = {0};
  code = from_int_to_decimal(num, &result);
  origin.bits[0] = 0b00000000000000000000000000000001;
  origin.bits[1] = 0b00000000000000000000000000000000;
  origin.bits[2] = 0b00000000000000000000000000000000;
  origin.bits[3] = 0b10000000000000000000000000000000;
  int check = is_equal(origin, result);
  ck_assert_int_eq(1, check);
  ck_assert_int_eq(code_check, code);
}
END_TEST

START_TEST(float_0) {
  float inp = 2;
  decimal out = {{0, 0, 0, 0}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00000002, 0, 0, 0}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_1) {
  float inp = -321.2;
  decimal out = {{0, 0, 0, 0}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00000C8C, 0, 0, 0x80010000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_2) {
  float inp = 321.2;
  decimal out = {{0, 0, 0, 0}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00000C8C, 0, 0, 0x00010000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_3) {
  float inp = 321213143.8323;
  decimal out = {{0, 0, 0, 0}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x13255310, 0, 0, 0}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_4) {
  float inp = 9732131234575756436473.323123474;
  decimal out = {{3211, 123145, 231, 312441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xC7238000, 0x9472ED75, 0x0000020F, 0}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_5) {
  float inp = 73.329959999;
  decimal out = {{3211, 123145, 231, 312441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x006FE484, 0, 0, 0x00050000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_6) {
  float inp = 0.003;
  decimal out = {{32131, 1231435, 2311, 3121441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00000003, 0, 0, 0x00030000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_7) {
  float inp = 0.00312345;
  decimal out = {{32131, 1231435, 2311, 3121441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0004C419, 0, 0, 0x00080000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_8) {
  float inp = 0.00000003123959595945;
  decimal out = {{32131, 1231435, 2311, 3121441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0004C44C, 0, 0, 0x000D0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_9) {
  float inp = 0.0000000000000000093929383838292;
  decimal out = {{32131, 1231435, 2311, 3121441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x008F532B, 0, 0, 0x00180000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_11) {
  float inp = .00000000000000000000000000000000193929383838292;
  decimal out = {{32131, 1231435, 2311, 3121441}};
  int error = from_float_to_decimal(inp, &out);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(float_12) {
  float inp = 2183182381923912939123.365656f;
  decimal out = {{32131, 1231435, 2311, 3121441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x8ADB0000, 0x59BC8E01, 0x00000076, 0}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_13) {
  float inp = 79215000000000000000000000000.365656;
  decimal out = {{32131, 1231435, 2311, 3121441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xCF000000, 0xAC05AE5D, 0xFFF51CBA, 0}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_14) {
  float inp = 7015000000000000000000000000.365656;
  decimal out = {{32131, 1231435, 2311, 3121441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xC7000000, 0xFFD18396, 0x16AAAC07, 0}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_18) {
  float inp = 0.000000000000000000000099999999999;
  decimal out = {{32131, 1231435, 2311, 3121441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00000001, 0, 0, 0x00160000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(float_19) {
  float inp = 0.00000000000000000000073766666662636;
  decimal out = {{32131, 1231435, 2311, 3121441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00708F1B, 0, 0, 0x001C0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST
START_TEST(float_20) {
  float inp = 0.0000000000000000000073736361612636;
  decimal out = {{32131, 1231435, 2311, 3121441}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00708344, 0, 0, 0x001B0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_22) {
  float inp = -1.307818E+07;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00C78EA4, 0x00000000, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_23) {
  float inp = -30914.76;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x002F2C14, 0x00000000, 0x00000000, 0x80020000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_25) {
  float inp = -750.9904;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00729790, 0x00000000, 0x00000000, 0x80040000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_26) {
  float inp = 72932.2;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x000B20EA, 0x00000000, 0x00000000, 0x00010000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_27) {
  float inp = 7.526432E-06;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0072D820, 0x00000000, 0x00000000, 0x000C0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_29) {
  float inp = -1.391184E+17;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x9DB88000, 0x01EE3F72, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_30) {
  float inp = 3.693497E-09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00385BB9, 0x00000000, 0x00000000, 0x000F0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_31) {
  float inp = 351871.4;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0035B0FA, 0x00000000, 0x00000000, 0x00010000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_32) {
  float inp = 0.1175977;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0011F1A9, 0x00000000, 0x00000000, 0x00070000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_33) {
  float inp = -2.013319E-14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001EB887, 0x00000000, 0x00000000, 0x80140000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_34) {
  float inp = -280.7036;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x002AD4FC, 0x00000000, 0x00000000, 0x80040000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_35) {
  float inp = -3.196059E+18;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x2030B000, 0x2C5AAEBA, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_36) {
  float inp = -1.38892E+14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x50527800, 0x00007E52, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_37) {
  float inp = 6.068951E+09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x69BCD7D8, 0x00000001, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_38) {
  float inp = -1.190043E+24;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xF98C0000, 0x5C3C75F5, 0x0000FC00, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_40) {
  float inp = -7.442655E+23;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x1B360000, 0xB6AE8261, 0x00009D9A, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_41) {
  float inp = 8.763741E+26;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x8CD00000, 0x28107A63, 0x02D4EB6F, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_43) {
  float inp = 1.333345E+13;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x6F8D0680, 0x00000C20, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_44) {
  float inp = -3.85058E-06;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0005E022, 0x00000000, 0x00000000, 0x800B0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_45) {
  float inp = 1.325723E-09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00143A9B, 0x00000000, 0x00000000, 0x000F0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_46) {
  float inp = -5.831514E+23;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x30840000, 0xB3639777, 0x00007B7C, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_47) {
  float inp = -1.337444E-22;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00146864, 0x00000000, 0x00000000, 0x801C0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_48) {
  float inp = 1.230342E+28;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xAD800000, 0xA66BA8BD, 0x27C12686, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_49) {
  float inp = -8.335803E-07;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x007F31BB, 0x00000000, 0x00000000, 0x800D0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_51) {
  float inp = 2.922531E+25;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x9AB80000, 0x3ACC2F60, 0x00182CB3, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_52) {
  float inp = 1.581408E+19;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x319C0000, 0xDB76E5F9, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_54) {
  float inp = 1.290434E-09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0013B0C2, 0x00000000, 0x00000000, 0x000F0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_55) {
  float inp = -1.162378E-20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0011BC8A, 0x00000000, 0x00000000, 0x801A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_56) {
  float inp = -1.609435E+11;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x78FAEAE0, 0x00000025, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_57) {
  float inp = -2.747507E-06;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0029EC73, 0x00000000, 0x00000000, 0x800C0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_59) {
  float inp = -9.564972E-05;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0091F32C, 0x00000000, 0x00000000, 0x800B0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_60) {
  float inp = 6.76729E-11;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x000A5379, 0x00000000, 0x00000000, 0x00100000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_61) {
  float inp = -1.56421E+08;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0952CB88, 0x00000000, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_62) {
  float inp = 1.248784E-11;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00130E10, 0x00000000, 0x00000000, 0x00110000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_64) {
  float inp = -7.518575E+13;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x8BEBC180, 0x00004461, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_65) {
  float inp = 1.759471E+20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0821C000, 0x89C1AB3E, 0x00000009, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_66) {
  float inp = -1.730171E+16;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x468E8C00, 0x003D77D0, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_67) {
  float inp = -0.005833615;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0059038F, 0x00000000, 0x00000000, 0x80090000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_68) {
  float inp = -1.083782E+27;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x27C00000, 0xF1EDC3B6, 0x03807BC3, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_69) {
  float inp = -2.543952E+11;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x3B22B200, 0x0000003B, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_70) {
  float inp = 8.122079E+09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xE41D1718, 0x00000001, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_71) {
  float inp = -1.381688E-13;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00151538, 0x00000000, 0x00000000, 0x80130000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_72) {
  float inp = 5.3419E+17;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x8890E000, 0x0769D2F9, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_73) {
  float inp = -1.899235E+25;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xD8B80000, 0x5B459112, 0x000FB5C9, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_74) {
  float inp = 0.01434358;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0015E2F6, 0x00000000, 0x00000000, 0x00080000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_75) {
  float inp = -8.395913E+08;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x320B2584, 0x00000000, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_76) {
  float inp = 2.963941E-09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x002D39E5, 0x00000000, 0x00000000, 0x000F0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_78) {
  float inp = 1.511913E-20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001711E9, 0x00000000, 0x00000000, 0x001A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_79) {
  float inp = 12.18852;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00129924, 0x00000000, 0x00000000, 0x00050000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_80) {
  float inp = 8.216155E-11;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x007D5E5B, 0x00000000, 0x00000000, 0x00110000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_81) {
  float inp = -0.7325138;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x006FC5D2, 0x00000000, 0x00000000, 0x80070000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_82) {
  float inp = 49394.41;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x004B5EB1, 0x00000000, 0x00000000, 0x00020000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_83) {
  float inp = 0.0002621874;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x002801B2, 0x00000000, 0x00000000, 0x000A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_84) {
  float inp = 5.880856E+15;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x7CDAF000, 0x0014E49B, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_86) {
  float inp = 1.661392E-16;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001959D0, 0x00000000, 0x00000000, 0x00160000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_87) {
  float inp = -3.749618E-17;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x003936F2, 0x00000000, 0x00000000, 0x80170000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_88) {
  float inp = -1.078071E+10;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x82949470, 0x00000002, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_89) {
  float inp = -942.165;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x000E6055, 0x00000000, 0x00000000, 0x80030000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_90) {
  float inp = 8.128622E+24;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0CF80000, 0x7B4932DD, 0x0006B94D, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_91) {
  float inp = 3.478075E-10;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0035123B, 0x00000000, 0x00000000, 0x00100000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_92) {
  float inp = -0.2220105;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0021E049, 0x00000000, 0x00000000, 0x80070000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_93) {
  float inp = -4.057507E-11;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x003DE9A3, 0x00000000, 0x00000000, 0x80110000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_94) {
  float inp = -7.703651E+13;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x75D03380, 0x00004610, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_95) {
  float inp = 4.329224E-09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00420F08, 0x00000000, 0x00000000, 0x000F0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_96) {
  float inp = -3.238849E-06;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00316BC1, 0x00000000, 0x00000000, 0x800C0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_97) {
  float inp = -5.937755E-18;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x005A9A5B, 0x00000000, 0x00000000, 0x80180000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_98) {
  float inp = 9.214601E-09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x008C9A89, 0x00000000, 0x00000000, 0x000F0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_99) {
  float inp = -2.788572E+14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x80595C00, 0x0000FD9E, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_100) {
  float inp = -0.2386963;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00246C13, 0x00000000, 0x00000000, 0x80070000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_101) {
  float inp = -4.803305E-20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00494AE9, 0x00000000, 0x00000000, 0x801A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_102) {
  float inp = -6.715998E+27;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xFEC00000, 0x19B4CE7F, 0x15B357E5, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_103) {
  float inp = 1.001074E+27;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xE3400000, 0x47EB885C, 0x033C11AA, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_104) {
  float inp = 1.061498E+12;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x26361280, 0x000000F7, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_105) {
  float inp = -5.398762E+13;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xF8E99100, 0x00003119, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_106) {
  float inp = 2.011937E-05;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001EB321, 0x00000000, 0x00000000, 0x000B0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_107) {
  float inp = 1.208047E+28;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xE9C00000, 0xEEAB1663, 0x2708BB06, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_108) {
  float inp = -3.577309;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x003695DD, 0x00000000, 0x00000000, 0x80060000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_109) {
  float inp = -0.01349936;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00149930, 0x00000000, 0x00000000, 0x80080000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_110) {
  float inp = 2.636795E+21;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x4E1F8000, 0xF0E1AA29, 0x0000008E, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_111) {
  float inp = 1.886762E+28;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xBE800000, 0xAE79E847, 0x3CF6EDF1, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_112) {
  float inp = 1.137225E+20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xED5C4000, 0x2A377B38, 0x00000006, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_114) {
  float inp = -3.082871E+14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xAF159700, 0x00011862, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_115) {
  float inp = -4.087111E-10;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x003E5D47, 0x00000000, 0x00000000, 0x80100000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_116) {
  float inp = -1.082272E+23;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x14400000, 0x021DDBB8, 0x000016EB, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_117) {
  float inp = -9.243464E-20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x008D0B48, 0x00000000, 0x00000000, 0x801A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_118) {
  float inp = -1.351883E-11;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0014A0CB, 0x00000000, 0x00000000, 0x80110000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_119) {
  float inp = 1.067036E+22;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x811C0000, 0x70FDEE02, 0x00000242, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_120) {
  float inp = -1.655359E-07;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0019423F, 0x00000000, 0x00000000, 0x800D0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_121) {
  float inp = -2.818527E-09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x002B01DF, 0x00000000, 0x00000000, 0x800F0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_123) {
  float inp = 2.310628E-10;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x002341E4, 0x00000000, 0x00000000, 0x00100000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_124) {
  float inp = 2968.287;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x002D4ADF, 0x00000000, 0x00000000, 0x00030000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_125) {
  float inp = 1.229755E-18;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0012C3BB, 0x00000000, 0x00000000, 0x00180000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_126) {
  float inp = 1.059234E-14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001029A2, 0x00000000, 0x00000000, 0x00140000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_127) {
  float inp = -64208.49;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0061F971, 0x00000000, 0x00000000, 0x80020000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_128) {
  float inp = 2.186795E+15;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xE32BEE00, 0x0007C4E0, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_129) {
  float inp = -2.191804E+07;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x014E7158, 0x00000000, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_130) {
  float inp = 6.5419E-24;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0000FF8B, 0x00000000, 0x00000000, 0x001C0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_131) {
  float inp = -1.152604E+26;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xF9C00000, 0xCDD8046A, 0x005F5756, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_132) {
  float inp = 6.767653E+21;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0AB08000, 0xE00D62F0, 0x0000016E, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_133) {
  float inp = -4.926002E+14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x6BEFF200, 0x0001C004, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_134) {
  float inp = 1.194919E+26;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x4F700000, 0xE0F3DC90, 0x0062D764, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_135) {
  float inp = 0.2616035;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0027EAE3, 0x00000000, 0x00000000, 0x00070000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_136) {
  float inp = -2.989957E+13;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x893FB080, 0x00001B31, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_138) {
  float inp = -0.003898347;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x003B7BEB, 0x00000000, 0x00000000, 0x80090000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_139) {
  float inp = 11.08692;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0010EAD4, 0x00000000, 0x00000000, 0x00050000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_140) {
  float inp = -0.003970314;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x003C950A, 0x00000000, 0x00000000, 0x80090000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_141) {
  float inp = 1.054338E-15;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00101682, 0x00000000, 0x00000000, 0x00150000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_142) {
  float inp = -32.53767;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0031A607, 0x00000000, 0x00000000, 0x80050000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_143) {
  float inp = 0.06596097;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0064A601, 0x00000000, 0x00000000, 0x00080000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_144) {
  float inp = 8.861377E+25;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xE7E80000, 0x18E914DA, 0x00494CB2, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_146) {
  float inp = 3191.227;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0030B1BB, 0x00000000, 0x00000000, 0x00030000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_147) {
  float inp = 0.938446;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x000E51CE, 0x00000000, 0x00000000, 0x00060000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_148) {
  float inp = -3.894784E-17;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x003B6E00, 0x00000000, 0x00000000, 0x80170000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_149) {
  float inp = 7.679175E+14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xB9CDE700, 0x0002BA6A, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_150) {
  float inp = 9.389545E+08;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x37F74F04, 0x00000000, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_151) {
  float inp = -8.570058E+07;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x051BAFE4, 0x00000000, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_152) {
  float inp = 1.069406E-20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0010515E, 0x00000000, 0x00000000, 0x001A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_153) {
  float inp = 1963320;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001DF538, 0x00000000, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_154) {
  float inp = 1631.409;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0018E4B1, 0x00000000, 0x00000000, 0x00030000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_155) {
  float inp = -9.583782E-09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00923CA6, 0x00000000, 0x00000000, 0x800F0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_156) {
  float inp = 4.934571E+14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xEF284B00, 0x0001C0CB, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_157) {
  float inp = 10.23795;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x000F9F33, 0x00000000, 0x00000000, 0x00050000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_158) {
  float inp = -703088.8;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x006B4868, 0x00000000, 0x00000000, 0x80010000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_159) {
  float inp = -1.061253E+17;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xE82E8800, 0x01790865, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_160) {
  float inp = 0.0001655871;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0019443F, 0x00000000, 0x00000000, 0x000A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_161) {
  float inp = -158.45;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00003DE5, 0x00000000, 0x00000000, 0x80020000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_162) {
  float inp = 2.961945E+11;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xF69165A0, 0x00000044, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_164) {
  float inp = -212.0078;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0020598E, 0x00000000, 0x00000000, 0x80040000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_165) {
  float inp = 1.683534E-20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0019B04E, 0x00000000, 0x00000000, 0x001A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_166) {
  float inp = 1.360221E+25;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x01480000, 0x4818EABE, 0x000B4061, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_167) {
  float inp = 4.304772E-15;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0041AF84, 0x00000000, 0x00000000, 0x00150000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_168) {
  float inp = 2.341019E+14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x18973B00, 0x0000D4EA, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_169) {
  float inp = 2.241625E-06;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00223459, 0x00000000, 0x00000000, 0x000C0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_172) {
  float inp = -6.89053E+23;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xD4540000, 0xA35284BD, 0x000091E9, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_173) {
  float inp = -1.335107E-17;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00145F43, 0x00000000, 0x00000000, 0x80170000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_174) {
  float inp = 4.299471E+28;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xA1C00000, 0x5E84AF70, 0x8AEC63FB, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_175) {
  float inp = 5.439797E-13;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00530135, 0x00000000, 0x00000000, 0x00130000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_176) {
  float inp = 9283997;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x008DA99D, 0x00000000, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_177) {
  float inp = -5.917783E-20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x005A4C57, 0x00000000, 0x00000000, 0x801A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_178) {
  float inp = -4.605351E-08;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x004645A7, 0x00000000, 0x00000000, 0x800E0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_179) {
  float inp = 3.67147E-07;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00059A2B, 0x00000000, 0x00000000, 0x000C0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_180) {
  float inp = 7.805752E-14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00771B38, 0x00000000, 0x00000000, 0x00140000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_182) {
  float inp = 1.029686E+16;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x8A9A1800, 0x002494F0, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_183) {
  float inp = -1.623241E+11;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xCB4533A0, 0x00000025, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_184) {
  float inp = 0.01401084;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001560FC, 0x00000000, 0x00000000, 0x00080000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_185) {
  float inp = 16.07887;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001888CF, 0x00000000, 0x00000000, 0x00050000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_186) {
  float inp = -2.094895E+21;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x45F18000, 0x90821F45, 0x00000071, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_187) {
  float inp = -4.274481E-16;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00413931, 0x00000000, 0x00000000, 0x80160000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_188) {
  float inp = 7.869093E+14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x994C0500, 0x0002CBB0, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_189) {
  float inp = -1.272028E-09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001368DC, 0x00000000, 0x00000000, 0x800F0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_190) {
  float inp = -5.217951E-12;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x004F9E9F, 0x00000000, 0x00000000, 0x80120000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_191) {
  float inp = -386648.3;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x003AFF73, 0x00000000, 0x00000000, 0x80010000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_192) {
  float inp = 3.61813E+09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xD7A84850, 0x00000000, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_193) {
  float inp = -109.3412;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0010AF24, 0x00000000, 0x00000000, 0x80040000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_194) {
  float inp = 4.581238E+18;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x45856000, 0x3F93D3B0, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_195) {
  float inp = -6.603116E-17;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0064C16C, 0x00000000, 0x00000000, 0x80170000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_196) {
  float inp = -1.320119E+10;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x12DA2C70, 0x00000003, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_197) {
  float inp = 2.82232E-12;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00044E78, 0x00000000, 0x00000000, 0x00110000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_198) {
  float inp = -1.841507E+09;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x6DC32AB8, 0x00000000, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_199) {
  float inp = -2.262675E-19;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00228693, 0x00000000, 0x00000000, 0x80190000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_200) {
  float inp = 2.464824E-20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00259C38, 0x00000000, 0x00000000, 0x001A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_201) {
  float inp = 1.511738E-07;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0017113A, 0x00000000, 0x00000000, 0x000D0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_202) {
  float inp = 1.720869E-06;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001A4225, 0x00000000, 0x00000000, 0x000C0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_204) {
  float inp = -10.51995;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00100D5B, 0x00000000, 0x00000000, 0x80050000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_205) {
  float inp = 1.603923E+24;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x896C0000, 0xD70DD7BD, 0x000153A4, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_206) {
  float inp = 119471.1;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00123AD7, 0x00000000, 0x00000000, 0x00010000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_207) {
  float inp = 6.965003E+25;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x44F80000, 0xB125DD81, 0x00399CF7, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_208) {
  float inp = 1.296483E+17;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x233BB800, 0x01CC9A71, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_209) {
  float inp = -5.872053E+13;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xF054E880, 0x00003567, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_210) {
  float inp = -1.481293E+23;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x26820000, 0x1AFE5755, 0x00001F5E, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_211) {
  float inp = -2.851002E-18;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x002B80BA, 0x00000000, 0x00000000, 0x80180000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_212) {
  float inp = 112195.3;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00111EA1, 0x00000000, 0x00000000, 0x00010000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_213) {
  float inp = 1.039634E+08;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x06325B08, 0x00000000, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_214) {
  float inp = 3.40683E+13;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x24EB6B00, 0x00001EFC, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_215) {
  float inp = 6.772869E+11;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xB171F120, 0x0000009D, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_216) {
  float inp = -1.268637E+15;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x91BBE200, 0x000481D1, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_217) {
  float inp = -0.001798262;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001B7076, 0x00000000, 0x00000000, 0x80090000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_218) {
  float inp = -3.38201E-15;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00052919, 0x00000000, 0x00000000, 0x80140000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_219) {
  float inp = -3.263142E-13;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0031CAA6, 0x00000000, 0x00000000, 0x80130000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_221) {
  float inp = 3.812988E+22;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xCF7C0000, 0x06623F3C, 0x00000813, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_222) {
  float inp = 3.308621E+16;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x92039400, 0x00758BBB, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_223) {
  float inp = -1.322553E-20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00142E39, 0x00000000, 0x00000000, 0x801A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_224) {
  float inp = 1.148738E+08;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x06D8D5C8, 0x00000000, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_225) {
  float inp = -1.991151E+10;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xA2D187F0, 0x00000004, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_226) {
  float inp = 884496.5;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0086F6A5, 0x00000000, 0x00000000, 0x00010000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_229) {
  float inp = 9.713084E+17;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xF55A6000, 0x0D7AC7D7, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_230) {
  float inp = -2.557154E+19;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x6BB14000, 0x62E05C25, 0x00000001, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_231) {
  float inp = -0.6512141;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00635E0D, 0x00000000, 0x00000000, 0x80070000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_232) {
  float inp = 3.490169E+16;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xF282C400, 0x007BFEE6, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_233) {
  float inp = 5.061718E-20;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x004D3C56, 0x00000000, 0x00000000, 0x001A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_234) {
  float inp = 6.59684E+22;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xEEE80000, 0x27750C6F, 0x00000DF8, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_235) {
  float inp = -1.918654E-19;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x001D46BE, 0x00000000, 0x00000000, 0x80190000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_236) {
  float inp = -1.215304E-18;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00128B48, 0x00000000, 0x00000000, 0x80180000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_237) {
  float inp = -3.20701E-15;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x0004E4BD, 0x00000000, 0x00000000, 0x80140000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_238) {
  float inp = -2.55141E+17;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x40A35000, 0x038A7163, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_239) {
  float inp = -1.751754E+22;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x684A0000, 0xA0B3FA18, 0x000003B5, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_240) {
  float inp = -1.188452E+26;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xD2400000, 0x3393C802, 0x00624E73, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_242) {
  float inp = 44301.02;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00439916, 0x00000000, 0x00000000, 0x00020000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_244) {
  float inp = -3.094722E-11;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x002F38C2, 0x00000000, 0x00000000, 0x80110000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_245) {
  float inp = -0.0001411886;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00158B2E, 0x00000000, 0x00000000, 0x800A0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_246) {
  float inp = 4.721357E-18;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00480ACD, 0x00000000, 0x00000000, 0x00180000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_247) {
  float inp = -6.247398E+10;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x8BBD4860, 0x0000000E, 0x00000000, 0x80000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_248) {
  float inp = -1.663049E-18;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00196049, 0x00000000, 0x00000000, 0x80180000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_249) {
  float inp = 4.894282E+14;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0xE2010A00, 0x0001BD21, 0x00000000, 0x00000000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(float_250) {
  float inp = 7.748195E-06;
  decimal out = {{0x00009D40, 0x0000EB82, 0x00008442, 0x00001472}};
  int error = from_float_to_decimal(inp, &out);
  decimal res = {{0x00763A63, 0x00000000, 0x00000000, 0x000C0000}};
  int code = is_equal(res, out);
  ck_assert_int_eq(code, 1);
  ck_assert_int_eq(error, 0);
}
END_TEST
//|||||||||||||||||||||   \PARSERS     |||||||||||||||||||||

int main(void) {
  Suite* s = suite_create("decimal");
  SRunner* sr = srunner_create(s);
  int number_failed;

  //|||||||||||||||||||||    COMPARISON    |||||||||||||||||||||
  TCase* tc_comparison = tcase_create("comparison");
  suite_add_tcase(s, tc_comparison);
  tcase_add_test(tc_comparison, equal_test_1);
  tcase_add_test(tc_comparison, equal_test_2);
  tcase_add_test(tc_comparison, equal_test_3);
  tcase_add_test(tc_comparison, equal_test_4);
  tcase_add_test(tc_comparison, not_equal_test_1);
  tcase_add_test(tc_comparison, not_equal_test_2);
  tcase_add_test(tc_comparison, not_equal_test_3);
  tcase_add_test(tc_comparison, not_equal_test_4);
  tcase_add_test(tc_comparison, greater_test_1);
  tcase_add_test(tc_comparison, greater_test_2);
  tcase_add_test(tc_comparison, greater_test_3);
  tcase_add_test(tc_comparison, greater_test_4);
  tcase_add_test(tc_comparison, greater_test_5);
  tcase_add_test(tc_comparison, less_test_1);
  tcase_add_test(tc_comparison, less_test_2);
  tcase_add_test(tc_comparison, less_test_3);
  tcase_add_test(tc_comparison, less_or_equal_test_1);
  tcase_add_test(tc_comparison, less_or_equal_test_2);
  tcase_add_test(tc_comparison, greater_or_equal_test_1);
  tcase_add_test(tc_comparison, greater_or_equal_test_2);
  tcase_add_test(tc_comparison, greater_or_equal_test_3);
  tcase_add_test(tc_comparison, greater_or_equal_test_4);
  //|||||||||||||||||||||    \COMPARISON    |||||||||||||||||||||

  //|||||||||||||||||||||  UTILS  |||||||||||||||||||||
  TCase* tc_utils = tcase_create("utils");
  suite_add_tcase(s, tc_utils);
  tcase_add_test(tc_utils, rescale_test);
  tcase_add_test(tc_utils, rescale_test_2);
  tcase_add_test(tc_utils, div_10_test);
  tcase_add_test(tc_utils, set_scale_test);
  //|||||||||||||||||||||  \UTILS  |||||||||||||||||||||

  //|||||||||||||||||||||    OTHER_FUNC    |||||||||||||||||||||
  TCase* tc_other_func = tcase_create("other_func");
  suite_add_tcase(s, tc_other_func);

  tcase_add_test(tc_other_func, truncate_1);
  tcase_add_test(tc_other_func, truncate_2);
  tcase_add_test(tc_other_func, truncate_3);
  tcase_add_test(tc_other_func, truncate_4);
  tcase_add_test(tc_other_func, truncate_5);
  tcase_add_test(tc_other_func, truncate_6);
  tcase_add_test(tc_other_func, truncate_7);
  tcase_add_test(tc_other_func, truncate_8);
  tcase_add_test(tc_other_func, truncate_9);
  tcase_add_test(tc_other_func, truncate_10);
  tcase_add_test(tc_other_func, truncate_11);
  tcase_add_test(tc_other_func, truncate_12);
  tcase_add_test(tc_other_func, truncate_13);
  tcase_add_test(tc_other_func, truncate_14);
  tcase_add_test(tc_other_func, truncate_15);
  tcase_add_test(tc_other_func, truncate_16);

  tcase_add_test(tc_other_func, negate_1);
  tcase_add_test(tc_other_func, negate_2);
  tcase_add_test(tc_other_func, negate_3);
  tcase_add_test(tc_other_func, negate_4);
  tcase_add_test(tc_other_func, negate_5);
  tcase_add_test(tc_other_func, negate_6);
  tcase_add_test(tc_other_func, negate_7);
  tcase_add_test(tc_other_func, negate_8);
  tcase_add_test(tc_other_func, negate_9);
  tcase_add_test(tc_other_func, negate_10);
  tcase_add_test(tc_other_func, negate_11);
  tcase_add_test(tc_other_func, negate_12);

  tcase_add_test(tc_other_func, round_1);
  tcase_add_test(tc_other_func, round_2);
  tcase_add_test(tc_other_func, round_3);
  tcase_add_test(tc_other_func, round_4);
  tcase_add_test(tc_other_func, round_5);
  tcase_add_test(tc_other_func, round_6);
  tcase_add_test(tc_other_func, round_7);

  tcase_add_test(tc_other_func, floor_1);
  tcase_add_test(tc_other_func, floor_2);
  tcase_add_test(tc_other_func, floor_3);
  tcase_add_test(tc_other_func, floor_4);
  tcase_add_test(tc_other_func, floor_5);
  tcase_add_test(tc_other_func, floor_6);
  tcase_add_test(tc_other_func, floor_7);
  tcase_add_test(tc_other_func, floor_8);
  tcase_add_test(tc_other_func, floor_9);
  tcase_add_test(tc_other_func, floor_10);
  tcase_add_test(tc_other_func, floor_11);
  tcase_add_test(tc_other_func, floor_12);
  tcase_add_test(tc_other_func, floor_13);
  tcase_add_test(tc_other_func, floor_14);
  tcase_add_test(tc_other_func, floor_15);
  tcase_add_test(tc_other_func, floor_16);
  //|||||||||||||||||||||    \OTHER_FUNC    |||||||||||||||||||||

  //|||||||||||||||||||||   ARITHMETIC     |||||||||||||||||||||
  TCase* tc_arithmetic = tcase_create("arithmetic");
  suite_add_tcase(s, tc_arithmetic);
  // |||||||||||   SUB     |||||||||||
  tcase_add_test(tc_arithmetic, sub_0);
  tcase_add_test(tc_arithmetic, sub_1);
  tcase_add_test(tc_arithmetic, sub_2);
  tcase_add_test(tc_arithmetic, sub_3);
  tcase_add_test(tc_arithmetic, sub_4);
  tcase_add_test(tc_arithmetic, sub_5);
  tcase_add_test(tc_arithmetic, sub_6);
  tcase_add_test(tc_arithmetic, sub_7);
  tcase_add_test(tc_arithmetic, sub_8);
  tcase_add_test(tc_arithmetic, sub_9);
  tcase_add_test(tc_arithmetic, sub_10);
  tcase_add_test(tc_arithmetic, sub_11);
  tcase_add_test(tc_arithmetic, sub_12);
  tcase_add_test(tc_arithmetic, sub_13);
  tcase_add_test(tc_arithmetic, sub_14);
  tcase_add_test(tc_arithmetic, sub_15);
  tcase_add_test(tc_arithmetic, sub_16);
  tcase_add_test(tc_arithmetic, sub_17);
  tcase_add_test(tc_arithmetic, sub_18);
  tcase_add_test(tc_arithmetic, sub_19);
  tcase_add_test(tc_arithmetic, sub_20);
  tcase_add_test(tc_arithmetic, sub_21);
  tcase_add_test(tc_arithmetic, sub_22);
  tcase_add_test(tc_arithmetic, sub_24);
  tcase_add_test(tc_arithmetic, sub_25);

  tcase_add_test(tc_arithmetic, sub_0);
  tcase_add_test(tc_arithmetic, sub_1);
  tcase_add_test(tc_arithmetic, sub_2);
  tcase_add_test(tc_arithmetic, sub_3);
  tcase_add_test(tc_arithmetic, sub_4);
  tcase_add_test(tc_arithmetic, sub_5);
  tcase_add_test(tc_arithmetic, sub_6);
  tcase_add_test(tc_arithmetic, sub_7);
  tcase_add_test(tc_arithmetic, sub_8);
  tcase_add_test(tc_arithmetic, sub_9);
  tcase_add_test(tc_arithmetic, sub_10);
  tcase_add_test(tc_arithmetic, sub_11);
  tcase_add_test(tc_arithmetic, sub_12);
  tcase_add_test(tc_arithmetic, sub_13);
  tcase_add_test(tc_arithmetic, sub_14);
  tcase_add_test(tc_arithmetic, sub_15);
  tcase_add_test(tc_arithmetic, sub_16);
  tcase_add_test(tc_arithmetic, sub_17);
  tcase_add_test(tc_arithmetic, sub_18);
  tcase_add_test(tc_arithmetic, sub_19);
  tcase_add_test(tc_arithmetic, sub_20);
  tcase_add_test(tc_arithmetic, sub_21);

  tcase_add_test(tc_arithmetic, sub_max_30);
  tcase_add_test(tc_arithmetic, sub_max_31);
  tcase_add_test(tc_arithmetic, sub_max_32);
  tcase_add_test(tc_arithmetic, sub_max_33);
  tcase_add_test(tc_arithmetic, sub_max_34);
  tcase_add_test(tc_arithmetic, sub_max_35);

  tcase_add_test(tc_arithmetic, subTest1);
  tcase_add_test(tc_arithmetic, subTest2);
  tcase_add_test(tc_arithmetic, subTest3);
  tcase_add_test(tc_arithmetic, subTest4);
  tcase_add_test(tc_arithmetic, subTest5);
  tcase_add_test(tc_arithmetic, subTest6);
  tcase_add_test(tc_arithmetic, subTest7);
  tcase_add_test(tc_arithmetic, subTest8);
  tcase_add_test(tc_arithmetic, subTest9);
  tcase_add_test(tc_arithmetic, subTest10);
  tcase_add_test(tc_arithmetic, subTest11);
  tcase_add_test(tc_arithmetic, subTest12);
  tcase_add_test(tc_arithmetic, subTest13);
  tcase_add_test(tc_arithmetic, subTest14);
  tcase_add_test(tc_arithmetic, subTest15);
  tcase_add_test(tc_arithmetic, subTest16);
  tcase_add_test(tc_arithmetic, subTest17);
  tcase_add_test(tc_arithmetic, subTest18);
  tcase_add_test(tc_arithmetic, subTest19);
  tcase_add_test(tc_arithmetic, subTest20);
  tcase_add_test(tc_arithmetic, subTest21);
  tcase_add_test(tc_arithmetic, subTest22);
  tcase_add_test(tc_arithmetic, subTest23);
  tcase_add_test(tc_arithmetic, subTest24);
  tcase_add_test(tc_arithmetic, subTest25);
  tcase_add_test(tc_arithmetic, subTest26);
  tcase_add_test(tc_arithmetic, subTest27);
  tcase_add_test(tc_arithmetic, subTest28);
  tcase_add_test(tc_arithmetic, subTest29);
  tcase_add_test(tc_arithmetic, subTest30);
  tcase_add_test(tc_arithmetic, subTest31);
  tcase_add_test(tc_arithmetic, subTest32);
  tcase_add_test(tc_arithmetic, subTest33);

  tcase_add_test(tc_arithmetic, test_decimal_sub_simple_0);
  tcase_add_test(tc_arithmetic, test_decimal_sub_simple_1);
  tcase_add_test(tc_arithmetic, test_decimal_sub_simple_2);
  tcase_add_test(tc_arithmetic, test_decimal_sub_simple_3);

  tcase_add_test(tc_arithmetic, test_decimal_sub_simple_7);
  tcase_add_test(tc_arithmetic, test_decimal_sub_simple_8);
  tcase_add_test(tc_arithmetic, test_decimal_sub_simple_10);
  tcase_add_test(tc_arithmetic, test_decimal_sub_simple_11);

  // |||||||||||   ADD     |||||||||||
  tcase_add_test(tc_arithmetic, add_0);
  tcase_add_test(tc_arithmetic, add_1);
  tcase_add_test(tc_arithmetic, add_2);
  tcase_add_test(tc_arithmetic, add_3);
  tcase_add_test(tc_arithmetic, add_4);
  tcase_add_test(tc_arithmetic, add_5);
  tcase_add_test(tc_arithmetic, add_6);
  tcase_add_test(tc_arithmetic, add_7);
  tcase_add_test(tc_arithmetic, add_8);
  tcase_add_test(tc_arithmetic, add_9);
  tcase_add_test(tc_arithmetic, add_10);
  tcase_add_test(tc_arithmetic, add_11);
  tcase_add_test(tc_arithmetic, add_12);
  tcase_add_test(tc_arithmetic, add_13);
  tcase_add_test(tc_arithmetic, add_14);
  tcase_add_test(tc_arithmetic, add_15);
  tcase_add_test(tc_arithmetic, add_16);
  tcase_add_test(tc_arithmetic, add_17);
  tcase_add_test(tc_arithmetic, add_18);
  tcase_add_test(tc_arithmetic, add_19);
  tcase_add_test(tc_arithmetic, add_20);
  tcase_add_test(tc_arithmetic, add_21);
  tcase_add_test(tc_arithmetic, add_22);

  tcase_add_test(tc_arithmetic, add_0);
  tcase_add_test(tc_arithmetic, add_1);
  tcase_add_test(tc_arithmetic, add_2);
  tcase_add_test(tc_arithmetic, add_3);
  tcase_add_test(tc_arithmetic, add_4);
  tcase_add_test(tc_arithmetic, add_5);
  tcase_add_test(tc_arithmetic, add_6);
  tcase_add_test(tc_arithmetic, add_7);
  tcase_add_test(tc_arithmetic, add_8);
  tcase_add_test(tc_arithmetic, add_9);
  tcase_add_test(tc_arithmetic, add_10);
  tcase_add_test(tc_arithmetic, add_11);
  tcase_add_test(tc_arithmetic, add_12);
  tcase_add_test(tc_arithmetic, add_13);
  tcase_add_test(tc_arithmetic, add_14);
  tcase_add_test(tc_arithmetic, add_15);
  tcase_add_test(tc_arithmetic, add_16);
  tcase_add_test(tc_arithmetic, add_17);
  tcase_add_test(tc_arithmetic, add_18);

  tcase_add_test(tc_arithmetic, minus_overflow_test);
  tcase_add_test(tc_arithmetic, overflow_test);

  tcase_add_test(tc_arithmetic, test_decimal_add_0);
  tcase_add_test(tc_arithmetic, test_decimal_add_1);
  tcase_add_test(tc_arithmetic, test_decimal_add_2);
  tcase_add_test(tc_arithmetic, test_decimal_add_3);
  tcase_add_test(tc_arithmetic, test_decimal_add_4);
  tcase_add_test(tc_arithmetic, test_decimal_add_5);
  tcase_add_test(tc_arithmetic, test_decimal_add_6);
  tcase_add_test(tc_arithmetic, test_decimal_add_7);
  tcase_add_test(tc_arithmetic, test_decimal_add_8);
  tcase_add_test(tc_arithmetic, test_decimal_add_8_1);
  tcase_add_test(tc_arithmetic, test_decimal_add_9);
  tcase_add_test(tc_arithmetic, test_decimal_add_10);
  tcase_add_test(tc_arithmetic, test_decimal_add_11);
  tcase_add_test(tc_arithmetic, test_decimal_add_12);
  tcase_add_test(tc_arithmetic, test_decimal_add_13);
  tcase_add_test(tc_arithmetic, test_decimal_add_14);
  tcase_add_test(tc_arithmetic, test_decimal_add_15);
  tcase_add_test(tc_arithmetic, test_decimal_add_16);
  tcase_add_test(tc_arithmetic, test_decimal_add_17);
  tcase_add_test(tc_arithmetic, test_decimal_add_18);
  tcase_add_test(tc_arithmetic, test_decimal_add_19);
  tcase_add_test(tc_arithmetic, test_decimal_add_20);
  tcase_add_test(tc_arithmetic, test_decimal_add_21);
  tcase_add_test(tc_arithmetic, test_decimal_add_25);
  tcase_add_test(tc_arithmetic, test_decimal_add_26);
  tcase_add_test(tc_arithmetic, test_decimal_add_27);
  tcase_add_test(tc_arithmetic, test_decimal_add_28);
  tcase_add_test(tc_arithmetic, test_decimal_add_29);
  tcase_add_test(tc_arithmetic, test_decimal_add_30);

  tcase_add_test(tc_arithmetic, test_decimal_add_simple_0);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_1);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_2);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_3);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_4);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_5);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_6);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_7);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_8);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_9);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_10);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_11);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_12);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_13);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_14);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_20);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_21);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_22);
  tcase_add_test(tc_arithmetic, test_decimal_add_simple_23);

  tcase_add_test(tc_arithmetic, add_max_31);
  tcase_add_test(tc_arithmetic, add_max_32);
  tcase_add_test(tc_arithmetic, add_max_33);
  tcase_add_test(tc_arithmetic, add_max_34);
  tcase_add_test(tc_arithmetic, add_max_35);
  tcase_add_test(tc_arithmetic, add_max_36);
  tcase_add_test(tc_arithmetic, add_test_37);
  // |||||||||||   MUL     |||||||||||
  tcase_add_test(tc_arithmetic, mul_test_1);
  tcase_add_test(tc_arithmetic, mul_test_2);
  tcase_add_test(tc_arithmetic, mul_test_3);
  tcase_add_test(tc_arithmetic, mul_test_4);
  tcase_add_test(tc_arithmetic, mul_test_5);
  tcase_add_test(tc_arithmetic, mul_test_6);
  tcase_add_test(tc_arithmetic, mul_test_7);
  tcase_add_test(tc_arithmetic, mul_test_8);
  tcase_add_test(tc_arithmetic, mul_test_9);
  tcase_add_test(tc_arithmetic, mul_test_10);
  tcase_add_test(tc_arithmetic, mul_test_11);
  tcase_add_test(tc_arithmetic, mul_test_12);
  tcase_add_test(tc_arithmetic, mul_test_13);
  tcase_add_test(tc_arithmetic, mul_test_14);
  tcase_add_test(tc_arithmetic, mul_test_15);
  tcase_add_test(tc_arithmetic, mul_test_16);
  tcase_add_test(tc_arithmetic, mul_test_17);
  tcase_add_test(tc_arithmetic, mul_test_18);
  // |||||||||||   DIV     |||||||||||
  tcase_add_test(tc_arithmetic, div_1);
  tcase_add_test(tc_arithmetic, div_2);
  tcase_add_test(tc_arithmetic, div_3);
  tcase_add_test(tc_arithmetic, div_4);
  tcase_add_test(tc_arithmetic, div_5);
  tcase_add_test(tc_arithmetic, div_7);
  tcase_add_test(tc_arithmetic, div_8);
  tcase_add_test(tc_arithmetic, div_9);
  tcase_add_test(tc_arithmetic, div_10);
  tcase_add_test(tc_arithmetic, div_11);
  tcase_add_test(tc_arithmetic, div_12);
  tcase_add_test(tc_arithmetic, div_13);
  tcase_add_test(tc_arithmetic, div_14);
  tcase_add_test(tc_arithmetic, div_15);
  tcase_add_test(tc_arithmetic, div_16);
  tcase_add_test(tc_arithmetic, div_17);
  tcase_add_test(tc_arithmetic, div_18);
  tcase_add_test(tc_arithmetic, div_19);
  tcase_add_test(tc_arithmetic, div_20);
  tcase_add_test(tc_arithmetic, div_21);
  tcase_add_test(tc_arithmetic, div_22);
  tcase_add_test(tc_arithmetic, div_23);
  tcase_add_test(tc_arithmetic, div_24);
  tcase_add_test(tc_arithmetic, div_25);
  tcase_add_test(tc_arithmetic, div_26);
  tcase_add_test(tc_arithmetic, div_27);
  tcase_add_test(tc_arithmetic, div_28);
  tcase_add_test(tc_arithmetic, div_29);
  tcase_add_test(tc_arithmetic, div_30);
  tcase_add_test(tc_arithmetic, div_31);
  tcase_add_test(tc_arithmetic, div_32);
  tcase_add_test(tc_arithmetic, div_33);
  tcase_add_test(tc_arithmetic, div_34);
  tcase_add_test(tc_arithmetic, div_35);
  tcase_add_test(tc_arithmetic, div_36);
  tcase_add_test(tc_arithmetic, div_37);
  tcase_add_test(tc_arithmetic, div_38);
  tcase_add_test(tc_arithmetic, div_39);
  tcase_add_test(tc_arithmetic, div_40);
  tcase_add_test(tc_arithmetic, div_41);
  tcase_add_test(tc_arithmetic, div_42);
  tcase_add_test(tc_arithmetic, div_43);
  tcase_add_test(tc_arithmetic, div_44);
  tcase_add_test(tc_arithmetic, div_45);
  tcase_add_test(tc_arithmetic, div_46);
  tcase_add_test(tc_arithmetic, div_47);
  tcase_add_test(tc_arithmetic, div_48);
  tcase_add_test(tc_arithmetic, div_49);
  tcase_add_test(tc_arithmetic, div_50);
  tcase_add_test(tc_arithmetic, div_51);
  // |||||||||||   MOD     |||||||||||
  tcase_add_test(tc_arithmetic, mod_1);
  tcase_add_test(tc_arithmetic, mod_2);
  tcase_add_test(tc_arithmetic, mod_3);
  tcase_add_test(tc_arithmetic, mod_4);
  tcase_add_test(tc_arithmetic, mod_5);
  tcase_add_test(tc_arithmetic, mod_6);
  tcase_add_test(tc_arithmetic, mod_7);
  tcase_add_test(tc_arithmetic, mod_8);
  tcase_add_test(tc_arithmetic, mod_9);
  tcase_add_test(tc_arithmetic, mod_10);
  tcase_add_test(tc_arithmetic, mod_11);
  tcase_add_test(tc_arithmetic, mod_12);
  tcase_add_test(tc_arithmetic, mod_13);
  tcase_add_test(tc_arithmetic, mod_14);
  tcase_add_test(tc_arithmetic, mod_15);
  tcase_add_test(tc_arithmetic, mod_16);
  tcase_add_test(tc_arithmetic, mod_17);
  tcase_add_test(tc_arithmetic, mod_18);
  tcase_add_test(tc_arithmetic, mod_19);
  tcase_add_test(tc_arithmetic, mod_20);
  tcase_add_test(tc_arithmetic, mod_21);
  tcase_add_test(tc_arithmetic, mod_22);
  tcase_add_test(tc_arithmetic, mod_23);
  tcase_add_test(tc_arithmetic, mod_24);
  tcase_add_test(tc_arithmetic, mod_25);
  tcase_add_test(tc_arithmetic, mod_26);
  tcase_add_test(tc_arithmetic, mod_27);
  tcase_add_test(tc_arithmetic, mod_28);
  tcase_add_test(tc_arithmetic, mod_29);
  tcase_add_test(tc_arithmetic, mod_30);
  tcase_add_test(tc_arithmetic, mod_31);
  tcase_add_test(tc_arithmetic, mod_32);
  tcase_add_test(tc_arithmetic, mod_33);
  tcase_add_test(tc_arithmetic, mod_34);
  tcase_add_test(tc_arithmetic, mod_35);
  tcase_add_test(tc_arithmetic, mod_36);
  tcase_add_test(tc_arithmetic, mod_49);
  tcase_add_test(tc_arithmetic, mod_50);
  tcase_add_test(tc_arithmetic, mod_51);
  tcase_add_test(tc_arithmetic, mod_52);
  tcase_add_test(tc_arithmetic, mod_53);
  tcase_add_test(tc_arithmetic, mod_54);
  tcase_add_test(tc_arithmetic, mod_55);

  //|||||||||||||||||||||   \ARITHMETIC     ||||||||||||||||||

  // //|||||||||||||||||||||   PARSERS     |||||||||||||||||||
  TCase* tc_parsers = tcase_create("parsers");
  suite_add_tcase(s, tc_parsers);
  // float to decimal
  tcase_add_test(tc_parsers, float_to_dec_1);
  tcase_add_test(tc_parsers, float_to_dec_2);
  tcase_add_test(tc_parsers, float_to_dec_3);
  tcase_add_test(tc_parsers, float_to_dec_4);
  tcase_add_test(tc_parsers, float_to_dec_5);
  tcase_add_test(tc_parsers, float_to_dec_6);
  tcase_add_test(tc_parsers, float_to_dec_7);
  tcase_add_test(tc_parsers, float_to_dec_8);
  tcase_add_test(tc_parsers, float_to_dec_9);
  tcase_add_test(tc_parsers, float_to_dec_10);
  tcase_add_test(tc_parsers, float_to_dec_11);
  tcase_add_test(tc_parsers, float_to_dec_12);
  tcase_add_test(tc_parsers, float_to_dec_13);
  tcase_add_test(tc_parsers, float_to_dec_14);
  tcase_add_test(tc_parsers, float_to_dec_15);
  tcase_add_test(tc_parsers, float_to_dec_16);
  tcase_add_test(tc_parsers, float_to_dec_17);
  tcase_add_test(tc_parsers, float_to_dec_18);
  tcase_add_test(tc_parsers, float_to_dec_19);
  tcase_add_test(tc_parsers, float_to_dec_21);
  tcase_add_test(tc_parsers, float_to_dec_22);
  tcase_add_test(tc_parsers, float_to_dec_23);
  tcase_add_test(tc_parsers, float_to_dec_24);
  tcase_add_test(tc_parsers, float_0);
  tcase_add_test(tc_parsers, float_1);
  tcase_add_test(tc_parsers, float_2);
  tcase_add_test(tc_parsers, float_3);
  tcase_add_test(tc_parsers, float_4);
  tcase_add_test(tc_parsers, float_5);
  tcase_add_test(tc_parsers, float_6);
  tcase_add_test(tc_parsers, float_7);
  tcase_add_test(tc_parsers, float_8);
  tcase_add_test(tc_parsers, float_9);
  tcase_add_test(tc_parsers, float_11);
  tcase_add_test(tc_parsers, float_12);
  tcase_add_test(tc_parsers, float_13);
  tcase_add_test(tc_parsers, float_14);
  tcase_add_test(tc_parsers, float_18);
  tcase_add_test(tc_parsers, float_19);
  tcase_add_test(tc_parsers, float_20);
  tcase_add_test(tc_parsers, float_22);
  tcase_add_test(tc_parsers, float_23);
  tcase_add_test(tc_parsers, float_25);
  tcase_add_test(tc_parsers, float_26);
  tcase_add_test(tc_parsers, float_27);
  tcase_add_test(tc_parsers, float_29);
  tcase_add_test(tc_parsers, float_30);
  tcase_add_test(tc_parsers, float_31);
  tcase_add_test(tc_parsers, float_32);
  tcase_add_test(tc_parsers, float_33);
  tcase_add_test(tc_parsers, float_34);
  tcase_add_test(tc_parsers, float_35);
  tcase_add_test(tc_parsers, float_36);
  tcase_add_test(tc_parsers, float_37);
  tcase_add_test(tc_parsers, float_38);
  tcase_add_test(tc_parsers, float_40);
  tcase_add_test(tc_parsers, float_41);
  tcase_add_test(tc_parsers, float_43);
  tcase_add_test(tc_parsers, float_44);
  tcase_add_test(tc_parsers, float_45);
  tcase_add_test(tc_parsers, float_46);
  tcase_add_test(tc_parsers, float_47);
  tcase_add_test(tc_parsers, float_48);
  tcase_add_test(tc_parsers, float_49);
  tcase_add_test(tc_parsers, float_51);
  tcase_add_test(tc_parsers, float_52);
  tcase_add_test(tc_parsers, float_54);
  tcase_add_test(tc_parsers, float_55);
  tcase_add_test(tc_parsers, float_56);
  tcase_add_test(tc_parsers, float_57);
  tcase_add_test(tc_parsers, float_59);
  tcase_add_test(tc_parsers, float_60);
  tcase_add_test(tc_parsers, float_61);
  tcase_add_test(tc_parsers, float_62);
  tcase_add_test(tc_parsers, float_64);
  tcase_add_test(tc_parsers, float_65);
  tcase_add_test(tc_parsers, float_66);
  tcase_add_test(tc_parsers, float_67);
  tcase_add_test(tc_parsers, float_68);
  tcase_add_test(tc_parsers, float_69);
  tcase_add_test(tc_parsers, float_70);
  tcase_add_test(tc_parsers, float_71);
  tcase_add_test(tc_parsers, float_72);
  tcase_add_test(tc_parsers, float_73);
  tcase_add_test(tc_parsers, float_74);
  tcase_add_test(tc_parsers, float_75);
  tcase_add_test(tc_parsers, float_76);
  tcase_add_test(tc_parsers, float_78);
  tcase_add_test(tc_parsers, float_79);
  tcase_add_test(tc_parsers, float_80);
  tcase_add_test(tc_parsers, float_81);
  tcase_add_test(tc_parsers, float_82);
  tcase_add_test(tc_parsers, float_83);
  tcase_add_test(tc_parsers, float_84);
  tcase_add_test(tc_parsers, float_86);
  tcase_add_test(tc_parsers, float_87);
  tcase_add_test(tc_parsers, float_88);
  tcase_add_test(tc_parsers, float_89);
  tcase_add_test(tc_parsers, float_90);
  tcase_add_test(tc_parsers, float_91);
  tcase_add_test(tc_parsers, float_92);
  tcase_add_test(tc_parsers, float_93);
  tcase_add_test(tc_parsers, float_94);
  tcase_add_test(tc_parsers, float_95);
  tcase_add_test(tc_parsers, float_96);
  tcase_add_test(tc_parsers, float_97);
  tcase_add_test(tc_parsers, float_98);
  tcase_add_test(tc_parsers, float_99);
  tcase_add_test(tc_parsers, float_100);
  tcase_add_test(tc_parsers, float_101);
  tcase_add_test(tc_parsers, float_102);
  tcase_add_test(tc_parsers, float_103);
  tcase_add_test(tc_parsers, float_104);
  tcase_add_test(tc_parsers, float_105);
  tcase_add_test(tc_parsers, float_106);
  tcase_add_test(tc_parsers, float_107);
  tcase_add_test(tc_parsers, float_108);
  tcase_add_test(tc_parsers, float_109);
  tcase_add_test(tc_parsers, float_110);
  tcase_add_test(tc_parsers, float_111);
  tcase_add_test(tc_parsers, float_112);
  tcase_add_test(tc_parsers, float_114);
  tcase_add_test(tc_parsers, float_115);
  tcase_add_test(tc_parsers, float_116);
  tcase_add_test(tc_parsers, float_117);
  tcase_add_test(tc_parsers, float_118);
  tcase_add_test(tc_parsers, float_119);
  tcase_add_test(tc_parsers, float_120);
  tcase_add_test(tc_parsers, float_121);
  tcase_add_test(tc_parsers, float_123);
  tcase_add_test(tc_parsers, float_124);
  tcase_add_test(tc_parsers, float_125);
  tcase_add_test(tc_parsers, float_126);
  tcase_add_test(tc_parsers, float_127);
  tcase_add_test(tc_parsers, float_128);
  tcase_add_test(tc_parsers, float_129);
  tcase_add_test(tc_parsers, float_130);
  tcase_add_test(tc_parsers, float_131);
  tcase_add_test(tc_parsers, float_132);
  tcase_add_test(tc_parsers, float_133);
  tcase_add_test(tc_parsers, float_134);
  tcase_add_test(tc_parsers, float_135);
  tcase_add_test(tc_parsers, float_136);
  tcase_add_test(tc_parsers, float_138);
  tcase_add_test(tc_parsers, float_139);
  tcase_add_test(tc_parsers, float_140);
  tcase_add_test(tc_parsers, float_141);
  tcase_add_test(tc_parsers, float_142);
  tcase_add_test(tc_parsers, float_143);
  tcase_add_test(tc_parsers, float_144);
  tcase_add_test(tc_parsers, float_146);
  tcase_add_test(tc_parsers, float_147);
  tcase_add_test(tc_parsers, float_148);
  tcase_add_test(tc_parsers, float_149);
  tcase_add_test(tc_parsers, float_150);
  tcase_add_test(tc_parsers, float_151);
  tcase_add_test(tc_parsers, float_152);
  tcase_add_test(tc_parsers, float_153);
  tcase_add_test(tc_parsers, float_154);
  tcase_add_test(tc_parsers, float_155);
  tcase_add_test(tc_parsers, float_156);
  tcase_add_test(tc_parsers, float_157);
  tcase_add_test(tc_parsers, float_158);
  tcase_add_test(tc_parsers, float_159);
  tcase_add_test(tc_parsers, float_160);
  tcase_add_test(tc_parsers, float_161);
  tcase_add_test(tc_parsers, float_162);
  tcase_add_test(tc_parsers, float_164);
  tcase_add_test(tc_parsers, float_165);
  tcase_add_test(tc_parsers, float_166);
  tcase_add_test(tc_parsers, float_167);
  tcase_add_test(tc_parsers, float_168);
  tcase_add_test(tc_parsers, float_169);
  tcase_add_test(tc_parsers, float_172);
  tcase_add_test(tc_parsers, float_173);
  tcase_add_test(tc_parsers, float_174);
  tcase_add_test(tc_parsers, float_175);
  tcase_add_test(tc_parsers, float_176);
  tcase_add_test(tc_parsers, float_177);
  tcase_add_test(tc_parsers, float_178);
  tcase_add_test(tc_parsers, float_179);
  tcase_add_test(tc_parsers, float_180);
  tcase_add_test(tc_parsers, float_182);
  tcase_add_test(tc_parsers, float_183);
  tcase_add_test(tc_parsers, float_184);
  tcase_add_test(tc_parsers, float_185);
  tcase_add_test(tc_parsers, float_186);
  tcase_add_test(tc_parsers, float_187);
  tcase_add_test(tc_parsers, float_188);
  tcase_add_test(tc_parsers, float_189);
  tcase_add_test(tc_parsers, float_190);
  tcase_add_test(tc_parsers, float_191);
  tcase_add_test(tc_parsers, float_192);
  tcase_add_test(tc_parsers, float_193);
  tcase_add_test(tc_parsers, float_194);
  tcase_add_test(tc_parsers, float_195);
  tcase_add_test(tc_parsers, float_196);
  tcase_add_test(tc_parsers, float_197);
  tcase_add_test(tc_parsers, float_198);
  tcase_add_test(tc_parsers, float_199);
  tcase_add_test(tc_parsers, float_200);
  tcase_add_test(tc_parsers, float_201);
  tcase_add_test(tc_parsers, float_202);
  tcase_add_test(tc_parsers, float_204);
  tcase_add_test(tc_parsers, float_205);
  tcase_add_test(tc_parsers, float_206);
  tcase_add_test(tc_parsers, float_207);
  tcase_add_test(tc_parsers, float_208);
  tcase_add_test(tc_parsers, float_209);
  tcase_add_test(tc_parsers, float_210);
  tcase_add_test(tc_parsers, float_211);
  tcase_add_test(tc_parsers, float_212);
  tcase_add_test(tc_parsers, float_213);
  tcase_add_test(tc_parsers, float_214);
  tcase_add_test(tc_parsers, float_215);
  tcase_add_test(tc_parsers, float_216);
  tcase_add_test(tc_parsers, float_217);
  tcase_add_test(tc_parsers, float_218);
  tcase_add_test(tc_parsers, float_219);
  tcase_add_test(tc_parsers, float_221);
  tcase_add_test(tc_parsers, float_222);
  tcase_add_test(tc_parsers, float_223);
  tcase_add_test(tc_parsers, float_224);
  tcase_add_test(tc_parsers, float_225);
  tcase_add_test(tc_parsers, float_226);
  tcase_add_test(tc_parsers, float_229);
  tcase_add_test(tc_parsers, float_230);
  tcase_add_test(tc_parsers, float_231);
  tcase_add_test(tc_parsers, float_232);
  tcase_add_test(tc_parsers, float_233);
  tcase_add_test(tc_parsers, float_234);
  tcase_add_test(tc_parsers, float_235);
  tcase_add_test(tc_parsers, float_236);
  tcase_add_test(tc_parsers, float_237);
  tcase_add_test(tc_parsers, float_238);
  tcase_add_test(tc_parsers, float_239);
  tcase_add_test(tc_parsers, float_240);
  tcase_add_test(tc_parsers, float_242);
  tcase_add_test(tc_parsers, float_244);
  tcase_add_test(tc_parsers, float_245);
  tcase_add_test(tc_parsers, float_246);
  tcase_add_test(tc_parsers, float_247);
  tcase_add_test(tc_parsers, float_248);
  tcase_add_test(tc_parsers, float_249);
  tcase_add_test(tc_parsers, float_250);
  // decimal to float
  tcase_add_test(tc_parsers, dec_to_float_1);
  tcase_add_test(tc_parsers, dec_to_float_2);
  tcase_add_test(tc_parsers, dec_to_float_5);
  tcase_add_test(tc_parsers, dec_to_float_6);
  tcase_add_test(tc_parsers, dec_to_float_7);
  tcase_add_test(tc_parsers, dec_to_float_8);
  tcase_add_test(tc_parsers, dec_to_float_9);
  tcase_add_test(tc_parsers, dec_to_float_10);
  tcase_add_test(tc_parsers, dec_to_float_11);
  tcase_add_test(tc_parsers, dec_to_float_12);
  tcase_add_test(tc_parsers, dec_to_float_13);
  tcase_add_test(tc_parsers, dec_to_float_14);
  tcase_add_test(tc_parsers, dec_to_float_15);
  tcase_add_test(tc_parsers, dec_to_float_16);
  tcase_add_test(tc_parsers, dec_to_float_17);
  tcase_add_test(tc_parsers, dec_to_float_18);
  tcase_add_test(tc_parsers, dec_to_float_19);
  tcase_add_test(tc_parsers, dec_to_float_20);
  tcase_add_test(tc_parsers, dec_to_float_21);
  tcase_add_test(tc_parsers, dec_to_float_22);
  tcase_add_test(tc_parsers, dec_to_float_23);
  tcase_add_test(tc_parsers, dec_to_float_24);
  // decimal to int
  tcase_add_test(tc_parsers, dec_to_int_1);
  tcase_add_test(tc_parsers, dec_to_int_2);
  tcase_add_test(tc_parsers, dec_to_int_5);
  tcase_add_test(tc_parsers, dec_to_int_6);
  tcase_add_test(tc_parsers, dec_to_int_7);
  tcase_add_test(tc_parsers, dec_to_int_8);
  tcase_add_test(tc_parsers, dec_to_int_9);
  tcase_add_test(tc_parsers, dec_to_int_10);
  tcase_add_test(tc_parsers, dec_to_int_11);
  tcase_add_test(tc_parsers, dec_to_int_12);
  tcase_add_test(tc_parsers, dec_to_int_13);
  tcase_add_test(tc_parsers, dec_to_int_14);
  tcase_add_test(tc_parsers, dec_to_int_15);
  tcase_add_test(tc_parsers, dec_to_int_16);
  tcase_add_test(tc_parsers, dec_to_int_17);
  tcase_add_test(tc_parsers, dec_to_int_18);
  tcase_add_test(tc_parsers, dec_to_int_19);
  // int to decimal
  tcase_add_test(tc_parsers, int_to_dec_1);
  tcase_add_test(tc_parsers, int_to_dec_2);
  tcase_add_test(tc_parsers, int_to_dec_5);
  tcase_add_test(tc_parsers, int_to_dec_6);
  tcase_add_test(tc_parsers, int_to_dec_7);
  tcase_add_test(tc_parsers, int_to_dec_8);
  tcase_add_test(tc_parsers, int_to_dec_9);
  tcase_add_test(tc_parsers, int_to_dec_10);
  tcase_add_test(tc_parsers, int_to_dec_11);
  tcase_add_test(tc_parsers, int_to_dec_12);
  tcase_add_test(tc_parsers, int_to_dec_13);
  tcase_add_test(tc_parsers, int_to_dec_14);
  // //|||||||||||||||||||||   \PARSERS     ||||||||||||||||||

  srunner_run_all(sr, CK_ENV);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (number_failed == 0) ? 0 : -1;
}
