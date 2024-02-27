Description of the binary representation of numbers of the Decimal type

Decimal numbers are represented in binary format, consisting of several parts:

Sign bit: 1 bit indicating the sign of a number. 0 represents a positive number and 1 represents a negative number.

     96-bit integer: The integer part of the number occupies 96 bits.
     Scale Factor: This component is used to determine the decimal part of a number and the division of the integer part. It represents a number that is a power of 10 in the range from 0 to 28.

Decimal numbers can be represented as an array of four 32-bit signed integers (int bits[4];), where each element of the array contains a specific part of the number:

bits[0], bits[1], and bits[2] contain the low, middle, and high 32 bits of a 96-bit integer, respectively.
bits[3] consists of several parts:

     Bits 0 to 15 (low word) are not used and must be set to zero.
     Bits 16 through 23 contain an exponent representing a power of 10 for dividing an integer part.
     Bits 24 through 30 are also unused and must be zero.
     Bit 31 contains the sign of the number: 0 for a positive number and 1 for a negative number.

It is important to note that binary notation distinguishes between negative and positive zeros, although they are considered equivalent in all operations.


Arithmetic and comparison operations:

     The functions add, sub, div, mod accept two values of type decimal and return the sum, difference, division result or remainder, respectively. They also write the result to the third argument, result. In case of success, they return 0, and in case of error, they return an error code (1, 2 or 3).

     Comparison functions is_less, is_less_or_equal, is_greater, is_greater_or_equal, is_equal, is_not_equal compare two numbers of type decimal and return 1 if the condition is true, and 0 if it is not true.

Conversions:

     The from_int_to_decimal and from_float_to_decimal functions convert an integer or floating point number to a number of type decimal.

     The functions from_decimal_to_int and from_decimal_to_float perform the inverse conversion, converting a number of type decimal to an integer or floating point number.

Additional functions:

     The functions floor, round, truncate, negate round the number down to the nearest integer, to the truncated value and change the sign, respectively.

When using the library, please note that numbers of type decimal are limited to the range from -79,228,162,514,264,337,593,543,950,335 to +79,228,162,514,264,337,593,543,950,335.