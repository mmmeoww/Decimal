#include "s21_decimal.h"
#include "s21_helpers.h"

s21_decimal shift_left(s21_decimal value_1, int number) {
  for (int i = 0; i < number; i++) {
    unsigned memory = 0;
    for (int i = 0; i < 3; ++i) {
      unsigned temp = value_1.bits[i];
      value_1.bits[i] <<= 1;
      value_1.bits[i] |= memory;
      memory = temp >> (32 - 1);
    }
  }
  return value_1;
}

int mul_by_10(s21_decimal *value) {
  int result = 1;
  s21_decimal original = *value;
  s21_decimal temp = {0};

  int overflow =
      add_binary(shift_left(original, 3), shift_left(original, 1), &temp);

  if (!overflow) {
    *value = temp;
    result = 0;
  }
  return result;
}

void equalize(s21_decimal *a, s21_decimal *b) {
  int scale_a = get_power(*a);
  int scale_b = get_power(*b);
  int over = 0;
  while (scale_a != scale_b && !over) {
    if (scale_a < scale_b) {
      if (mul_by_10(a)) over = 1;
      scale_a++;
    } else {
      if (mul_by_10(b)) over = 1;
      scale_b++;
    }
  }

  set_power(a, scale_b);
  set_power(b, scale_a);
}

s21_big_decimal shift_left_big(s21_big_decimal value_1, int number) {
  for (int i = 0; i < number; i++) {
    unsigned memory = 0;
    for (int i = 0; i < 7; ++i) {
      unsigned temp = value_1.bits[i];
      value_1.bits[i] <<= 1;
      value_1.bits[i] |= memory;
      memory = temp >> (32 - 1);
    }
  }
  return value_1;
}

int ten_power_big(s21_big_decimal *value_1, int power1, int power2) {
  int dif = power1 - power2;
  s21_big_decimal temp1 = *value_1;
  for (int i = 0; i < dif; i++) {
    add_binary_256(shift_left_big(temp1, 3), shift_left_big(temp1, 1), value_1);
    temp1 = *value_1;
  }
  return dif;
}

void equalize_big(s21_big_decimal *value_1, s21_big_decimal *value_2,
                  int power1, int power2) {
  if (power1 > power2) {
    ten_power_big(value_2, power1, power2);
  } else if (power2 > power1) {
    ten_power_big(value_1, power2, power1);
  }
}
