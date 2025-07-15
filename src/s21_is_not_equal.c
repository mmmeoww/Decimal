#include "s21_decimal.h"

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 0;
  if (!s21_is_equal(value_1, value_2)) {
    result = 1;
  }
  return result;
}
