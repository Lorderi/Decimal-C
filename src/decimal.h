#if !defined(DECIMAL_H)
#define DECIMAL_H

#include <limits.h>  // for CHAR_BIT
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum bits { LOW, MID, HIGH, SCALE };
enum sign { PLUS, MINUS };
typedef enum { NO, YES } compare_result;
typedef enum { SUCCESS, ERROR } operations_result;

typedef enum {
  OK,
  INF,
  NEG_INF,
  _NAN  // NAN == division by zero
} arithmetics_result;

typedef struct decimal {
  unsigned int bits[4];
} decimal;

typedef struct decimal_long {
  unsigned int bits[6];
  int sign;
  int scale;
} decimal_long;

#include "decimal_long.h"
#include "decimal_utils.h"

//  comparison

int is_equal(decimal value_1, decimal value_2);
int is_not_equal(decimal value_1, decimal value_2);
int is_less(decimal, decimal);
int is_less_or_equal(decimal, decimal);
int is_greater(decimal, decimal);
int is_greater_or_equal(decimal, decimal);
//  other function

int truncate(decimal value, decimal *result);
int negate(decimal value, decimal *result);
int round(decimal value, decimal *result);
int floor(decimal value, decimal *result);
int add(decimal value_1, decimal value_2, decimal *result);
//  arithmetic

int add(decimal value_1, decimal value_2, decimal *result);
int sub(decimal value_1, decimal value_2, decimal *result);
int mul(decimal value_1, decimal value_2, decimal *result);
int div(decimal value_1, decimal value_2, decimal *result);
int mod(decimal value_1, decimal value_2, decimal *result);
//  parser

int from_int_to_decimal(int src, decimal *dst);
int from_float_to_decimal(float src, decimal *dst);
int from_decimal_to_int(decimal src, int *dst);
int from_decimal_to_float(decimal src, float *dst);

#endif  // DECIMAL_H
