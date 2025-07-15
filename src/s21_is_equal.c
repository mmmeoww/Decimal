#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int result = 1;
  if (is_zero_compr(value_1) && is_zero_compr(value_2)) {
    result = 1;
  } else if (get_sign(value_1) != get_sign(value_2)) {
    result = 0;
  } else {
    int index = 0;
    equalize(&value_1, &value_2);
    while (index < 96 && result != 0) {
      if (get_bit(value_1, index) != get_bit(value_2, index)) {
        result = 0;
      }
      index++;
    }
  }
  return result;
}
