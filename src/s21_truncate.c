#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  if (correct_dec(value) || !result) {
    return 1;
  }
  int power = get_power(value);
  *result = value;
  if (power != 0) {
    unsigned long long temp = 0;
    int remainder = 0;
    for (int i = 0; i < power; i++) {
      temp = result->bits[2];
      for (int j = 2; j >= 0; j--) {
        if (j != 0) {
          remainder = temp % 10;
          result->bits[j] = temp / 10;
          temp = remainder * HIGHER_BIT + result->bits[j - 1];
        } else {
          result->bits[j] = temp / 10;
        }
      }
    }
    set_power(result, 0);
  }
  return 0;
}