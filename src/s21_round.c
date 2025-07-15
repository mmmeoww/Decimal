#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_round(s21_decimal value, s21_decimal *result) {
  int err_code = 0;
  if (!result || correct_dec(value)) {
    return 1;
  }
  int power = get_power(value);
  err_code = s21_truncate(value, result);
  int tenth = 0;
  for (int i = 0; i < power; i++) {
    tenth = last_digit(&value);
  }
  if (tenth >= 5) {
    err_code = add_one_mnts(result);
  }
  return err_code;
}