#include "s21_decimal.h"
#include "s21_helpers.h"

int float_sign(int *i, char *str_float);
int get_num(int *i, int *float_power, char *str_float);
int get_true_pow(int *i, int *pow_sign, int *float_power, char *str_float);
void convert(int num, int float_power, int pow_sign, s21_decimal *dst,
             float src);

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int code = 0;
  if (!dst) {
    code = 1;
  } else if (isinf(src) || isnan(src) || check_float(src) != 0) {
    set_zeroes(dst);
    code = 1;
  } else {
    set_zeroes(dst);
    char *str_float = calloc(64, sizeof(char));
    sprintf(str_float, "%e", src);
    if (src) {
      int float_power = 1;
      int pow_sign = 0;
      int sign = 0;
      int num = 0;
      int i = 0;
      int true_pow = 0;
      sign = float_sign(&i, str_float);
      num = get_num(&i, &float_power, str_float);
      true_pow = get_true_pow(&i, &pow_sign, &float_power, str_float);
      float_power += true_pow;
      i = 0;
      if (true_pow < 29 && true_pow > -29) {
        convert(num, float_power, pow_sign, dst, src);
      }
      set_sign(dst, sign);
    }
    free(str_float);
  }
  return code;
}

int float_sign(int *i, char *str_float) {
  int sign = 0;
  if (str_float[*i] == '-') {
    sign = 1;
    *i += 1;
  }
  return sign;
}

int get_num(int *i, int *float_power, char *str_float) {
  int num = 0;
  for (; str_float[*i] != 'e'; *i += 1) {
    if (str_float[*i] != '.') {
      num *= 10;
      num += str_float[*i] - '0';
      *float_power -= 1;
    }
  }
  *i += 1;
  return num;
}

int get_true_pow(int *i, int *pow_sign, int *float_power, char *str_float) {
  int tmp = 0;
  if (str_float[*i] == '-') {
    *pow_sign = 1;
    *i += 1;
  } else if (str_float[*i] == '+') {
    *i += 1;
  }
  for (; str_float[*i]; *i += 1) {
    tmp *= 10;
    tmp += str_float[*i] - '0';
  }
  if (*pow_sign) *float_power *= -1;
  return tmp;
}

void convert(int num, int float_power, int pow_sign, s21_decimal *dst,
             float src) {
  while (num % 10 == 0 && float_power != 0) {
    num /= 10;
    float_power += pow_sign ? -1 : 1;
  }
  set_zeroes(dst);
  dst->bits[0] = num;
  if ((!pow_sign && float_power > 0)) {
    while (float_power) {
      s21_decimal dec = {{10, 0, 0, 0}};
      s21_mul(*dst, dec, dst);
      float_power--;
    }
    set_power(dst, abs(float_power));
  } else {
    int last = 0;
    if (float_power > 28) {
      char *str_float_tmp = calloc(64, sizeof(char));
      sprintf(str_float_tmp, "%.*e", 28 - (float_power - 7), src);
      int k = 0;
      while (str_float_tmp[k] != 'e') {
        k++;
      }
      last = str_float_tmp[k - 1] - 48;
      free(str_float_tmp);
    }
    while (float_power > 28) {
      last_digit(dst);
      float_power--;
    }
    if (last > 4) dst->bits[0]++;
    if (float_power == 28 && dst->bits[0] == 10) {
      dst->bits[0] = 1;
      float_power = 27;
    }
    set_power(dst, abs(float_power));
  }
}
