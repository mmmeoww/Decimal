#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int code = 0;
  if (correct_dec(src) == 1 || dst == NULL) {
    code = 1;
  } else {
    s21_decimal tmp = {0};
    s21_truncate(src, &tmp);
    *dst = tmp.bits[0];
    if (get_sign(tmp) == 1) {
      *dst *= (-1);
    }
  }
  return code;
}
