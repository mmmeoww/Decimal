#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  int result = 0;
  if (is_zero_compr(dec1) && is_zero_compr(dec2)) {
    result = 0;
  } else {
    int sign_a = get_sign(dec1);
    int sign_b = get_sign(dec2);
    if (sign_a != sign_b) {
      if (!sign_a && sign_b) {
        result = 1;
      }
    }
    s21_big_decimal big_dec1 = convert_D_in_bigD(dec1);
    s21_big_decimal big_dec2 = convert_D_in_bigD(dec2);
    int power1 = get_power(dec1);
    int power2 = get_power(dec2);
    equalize_big(&big_dec1, &big_dec2, power1, power2);
    int cmp = compare_absolute_big(big_dec1, big_dec2);
    if (cmp > 0) {
      if (!sign_a && !sign_b) {
        result = 1;
      }
    } else if (cmp < 0) {
      if (sign_a && sign_b) {
        result = 1;
      }
    }
  }

  return result;
}
