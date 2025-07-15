#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  arithmetic_result code = OK;
  s21_big_decimal big_result = {{0, 0, 0, 0, 0, 0, 0, 0}};
  if (!result) {
    code = ERROR;
  } else if (correct_dec(value_1) || correct_dec(value_2)) {
    set_zeroes(result);
    code = ERROR;
  } else if (is_zero(value_2) == 1) {
    set_zeroes(result);
    code = ZERO_DIV;
  } else {
    set_zeroes(result);
    int sign1 = get_sign(value_1);
    int sign2 = get_sign(value_2);
    int power1 = get_power(value_1);
    int power2 = get_power(value_2);
    int sign_res = 0;
    int iteration = 0;
    s21_big_decimal big_num1 = convert_D_in_bigD(value_1);
    s21_big_decimal big_num2 = convert_D_in_bigD(value_2);
    equalize_big(&big_num1, &big_num2, power1, power2);
    if ((sign1 == 0 && sign2 == 0) || (sign1 == 1 && sign2 == 1)) {
      sign_res = 0;
    } else {
      sign_res = 1;
    }
    code = div_brain(big_num1, big_num2, &big_result, &iteration);
    int power_res = power1 - power2 + iteration;
    s21_big_decimal temp = big_result;
    int shift = count_divisions_needed(&temp);
    int dif_power = power_res - shift;
    if (dif_power < 0 && sign_res == 0) {
      code = BIG;
    } else if (dif_power < 0 && sign_res == 1) {
      code = SMALL;
    } else {
      if (shift > 0) {
        while (shift != 0) {
          bank_round_256(&big_result);
          shift--;
        }
      }
    }
    code = convert_bigD_in_D(big_result, power_res, sign_res, result);
  }
  return code;
}