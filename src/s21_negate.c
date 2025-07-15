#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_negate(s21_decimal value, s21_decimal *result) {
  if (!result || correct_dec(value)) {
    return 1;
  }
  *result = value;
  result->bits[3] ^= MINUS;
  return 0;
}
