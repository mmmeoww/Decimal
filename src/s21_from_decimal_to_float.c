#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int code = 0;
  if (correct_dec(src) == 1 || dst == NULL) {
    code = 1;
  } else {
    *dst = 0;
    int sign = get_sign(src);
    double temp = 0;
    for (int i = 0; i < 96; i++) {
      if (get_bit(src, i) == 1) {
        temp += pow(2, i);
      }
    }
    int power = get_power(src);
    double result = temp / pow(10, power);
    *dst = (sign == 0) ? result : -result;
  }
  return code;
}
