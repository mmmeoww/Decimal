#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int code = 0;
  if (!dst) {
    code = 1;
  } else {
    set_zeroes(dst);
    if (src < 0) {
      dst->bits[3] = (1 << 31);
      src *= (-1);
    }
    dst->bits[0] = src;
  }
  return code;
}
