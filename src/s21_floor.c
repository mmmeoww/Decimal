#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int err_code = 0;
  if (!result || correct_dec(value) == 1) {
    err_code = 1;
  } else {
    int sign = get_sign(value);
    int pow = get_power(value);
    err_code = s21_truncate(value, result);
    if (sign == 1 && err_code == 0 && is_whole(value, pow) != 1) {
      err_code = add_one_mnts(result);
    }
  }
  return err_code;
}
