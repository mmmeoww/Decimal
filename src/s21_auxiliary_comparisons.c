#include "s21_decimal.h"
#include "s21_helpers.h"

int compare_absolute_big(s21_big_decimal dec1, s21_big_decimal dec2) {
  int result = 0;
  for (int i = 6; i >= 0; i--) {
    if (dec1.bits[i] > dec2.bits[i] && result == 0) result = 1;
    if (dec1.bits[i] < dec2.bits[i] && result == 0) result = -1;
  }
  return result;
}

int is_zero_compr(s21_decimal dec) {
  int result = 0;
  if (dec.bits[0] == 0 && dec.bits[1] == 0 && dec.bits[2] == 0) {
    result = 1;
  }
  return result;
}
