#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  arithmetic_result code = OK;
  if (!result) {
    code = ERROR;
  } else if (correct_dec(value_1) || correct_dec(value_2)) {
    set_zeroes(result);
    code = ERROR;
  } else {
    set_zeroes(result);
    s21_big_decimal big_result = {{0, 0, 0, 0, 0, 0, 0, 0}};
    int sign1 = get_sign(value_1);
    int sign2 = get_sign(value_2);
    int power1 = get_power(value_1);
    int power2 = get_power(value_2);
    int sign_res = 0;
    s21_big_decimal big_num1 = convert_D_in_bigD(value_1);
    s21_big_decimal big_num2 = convert_D_in_bigD(value_2);
    code = mul_binary_256(big_num1, big_num2, &big_result);
    if ((sign1 == 0 && sign2 == 0) || (sign1 == 1 && sign2 == 1))
      sign_res = 0;
    else
      sign_res = 1;
    int power_res = 0;
    s21_big_decimal temp = big_result;
    int shift = count_divisions_needed(&temp);
    power_res = power1 + power2 - shift;
    if (power_res < 0 && sign_res == 0) {
      code = BIG;
    } else if (power_res < 0 && sign_res == 1) {
      code = SMALL;
    } else {
      if (shift > 0) {
        while (shift != 0) {
          bank_round_256(&big_result);
          shift--;
        }
      }
    }
    while (power_res > 28) {
      bank_round_256(&big_result);
      power_res--;
    }
    code = convert_bigD_in_D(big_result, power_res, sign_res, result);
    if (is_zero(*result)) {
      set_sign(result, 0);
      set_power(result, 0);
    }
  }
  return code;
}