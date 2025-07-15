#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_decimal.h"

#define MAX_FLOAT_DECIMAL 79228162514264337593543950335.f
#define MIN_FLOAT_DECIMAL 1.0E-28
#define HIGHER_BIT 4294967296ULL
#define MINUS 0x80000000

typedef struct {
  unsigned int bits[8];
} s21_big_decimal;

typedef enum arithmetic_result {
  OK = 0,
  BIG = 1,
  SMALL = 2,
  ZERO_DIV = 3,
  ERROR = 4
} arithmetic_result;

int get_bit(s21_decimal num, int index);
int correct_dec(s21_decimal num);
int get_power(s21_decimal num);
int get_sign(s21_decimal num);
int big_power(int power1, int power2);
int get_bit_big_decimal(s21_big_decimal num, int index);
int poverka_big_decimal(s21_big_decimal value_1);
int compare_absolute_big(s21_big_decimal dec1, s21_big_decimal dec2);
int is_zero_compr(s21_decimal dec1);
int all_zero_after_point(s21_decimal value);
int is_whole(s21_decimal value, int pow);
int check_float(float src);
int convert_bigD_in_D(s21_big_decimal value_1, int power, int sign,
                      s21_decimal *result);
int set_sub_sign_big(s21_decimal value_1, s21_decimal value_2);
int set_add_sign_big(s21_decimal value_1, s21_decimal value_2);
int add_binary(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int add_binary_256(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
int sub_binary_256(s21_big_decimal num1, s21_big_decimal num2,
                   s21_big_decimal *result);
int is_zero_256(s21_big_decimal value_1);
int mul_binary_256(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result);
int s21_big_decimal_compare(s21_big_decimal a, s21_big_decimal b);
int div_256(s21_big_decimal value_1, s21_big_decimal value_2,
            s21_big_decimal *q, s21_big_decimal *r);
int div_brain(s21_big_decimal value_1, s21_big_decimal value_2,
              s21_big_decimal *result, int *iteration);
int ten_power_big(s21_big_decimal *value_1, int power1, int power2);
int greate_binary_big(s21_big_decimal value1, s21_big_decimal value2);
int greate_binary(s21_decimal value1, s21_decimal value2);
int add_one_mnts(s21_decimal *value);
int last_digit(s21_decimal *value);
int add_one_mnts_big(s21_big_decimal *value);
int last_digit_big(s21_big_decimal *value);
int fits_in_decimal(const s21_big_decimal *num);
int count_divisions_needed(s21_big_decimal *num);
int is_zero(s21_decimal num);

void set_bit(s21_decimal *num, int index, int bit);
void set_power(s21_decimal *num, int pow);
void set_sign(s21_decimal *num, int sign);
void set_zeroes(s21_decimal *value);
void set_bit_big_decimal(s21_big_decimal *num, int index, int bit);
void equalize_big(s21_big_decimal *value_1, s21_big_decimal *value_2,
                  int power1, int power2);
void equalize(s21_decimal *value_1, s21_decimal *value_2);
void bank_round_256(s21_big_decimal *value);
void divide_by_10(s21_big_decimal *num);

s21_big_decimal convert_D_in_bigD(s21_decimal value_1);
s21_big_decimal shift_left_big(s21_big_decimal value_1, int number);
s21_decimal shift_left(s21_decimal value_1, int number);

#endif