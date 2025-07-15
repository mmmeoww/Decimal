#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
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
    equalize_big(&big_num1, &big_num2, power1, power2);
    if ((sign1 == 0) && (sign2 == 0)) {
      add_binary_256(big_num1, big_num2, &big_result);
      sign_res = 0;
    } else if ((sign1 == 1) && (sign2 == 0)) {
      sub_binary_256(big_num1, big_num2, &big_result);
      sign_res = set_add_sign_big(value_1, value_2);
    } else if ((sign1 == 0) && (sign2 == 1)) {
      sub_binary_256(big_num1, big_num2, &big_result);
      sign_res = set_add_sign_big(value_1, value_2);
    } else {
      add_binary_256(big_num1, big_num2, &big_result);
      sign_res = 1;
    }
    s21_big_decimal temp = big_result;
    int shift = count_divisions_needed(&temp);
    int res_power = big_power(power1, power2);
    int dif_power = res_power - shift;
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
      code = convert_bigD_in_D(big_result, dif_power, sign_res, result);
      if (is_zero(*result)) {
        set_sign(result, 0);
        set_power(result, 0);
      }
    }
  }
  return code;
}
