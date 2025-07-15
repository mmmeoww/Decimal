#include "s21_helpers.h"

#include <math.h>
#include <stdio.h>

#include "s21_decimal.h"

int get_bit(s21_decimal num, int index) {
  int bit = 0;
  if (num.bits[index / 32] & (1U << index % 32)) {
    bit = 1;
  }
  return bit;
}

void set_bit(s21_decimal *num, int index, int bit) {
  if (bit > 0) {
    num->bits[index / 32] |= (1 << index % 32);
  } else {
    num->bits[index / 32] &= ~(1 << index % 32);
  }
}

int correct_dec(s21_decimal num) {
  int err_code = 0;
  if ((num.bits[3] & 0x7F00FFFF) != 0) {
    err_code = 1;
  }
  int pow = get_power(num);
  if (pow < 0 || pow > 28) {
    err_code = 1;
  }
  return err_code;
}

int get_sign(s21_decimal num) {
  int sign = get_bit(num, 127);
  return sign;
}

void set_sign(s21_decimal *num, int sign) {
  if (sign == 0 || sign == 1) {
    set_bit(num, 127, sign);
  }
}

int get_power(s21_decimal num) {
  int pow = (num.bits[3] >> 16) & 0xFF;
  return pow;
}

void set_power(s21_decimal *num, int pow) {
  int err_code = 0;
  if (pow < 0 || pow > 28) {
    err_code = 1;
  }
  if (!err_code) {
    int sign = get_sign(*num);
    num->bits[3] = 0;
    int index = 0;
    while (pow > 0) {
      set_bit(num, 112 + index, pow % 2);
      pow /= 2;
      index++;
    }
    set_sign(num, sign);
  }
}

int last_digit(s21_decimal *value) {
  int remainder = 0;
  unsigned long long temp = 0;
  for (int i = 2; i >= 0; i--) {
    temp = value->bits[i] + remainder * 4294967296ULL;
    remainder = temp % 10;
    value->bits[i] = (unsigned int)(temp / 10);
  }
  return remainder;
}

int add_one_mnts(s21_decimal *value) {
  int carry = 1;
  int err_code = 0;
  for (int i = 0; i < 3; i++) {
    unsigned long long temp = (unsigned long long)value->bits[i] + carry;
    value->bits[i] = (unsigned int)(temp & 0xFFFFFFFF);
    carry = (unsigned int)(temp >> 32);
  }
  if (carry != 0) {
    err_code = 1;
  }
  return err_code;
}

void set_zeroes(s21_decimal *value) {
  for (int i = 0; i < 4; i++) {
    value->bits[i] = 0;
  }
}

int check_float(float src) {
  int result = 0;
  if (fabsf(src) >= MAX_FLOAT_DECIMAL) {
    result = 1;
  }
  if (fabsf(src) != 0 && fabsf(src) < MIN_FLOAT_DECIMAL) {
    result = 2;
  }
  return result;
}

int all_zero_after_point(s21_decimal value) {
  int all_zeros = 0;
  s21_decimal truncated = {0};
  s21_truncate(value, &truncated);
  if (s21_is_equal(value, truncated) == 1) {
    all_zeros = 1;
  }

  return all_zeros;
}

int is_whole(s21_decimal value, int pow) {
  int whole = 0;
  if (pow == 0 || all_zero_after_point(value) == 1) {
    whole = 1;
  }
  return whole;
}

int big_power(int power1, int power2) {
  int res;
  if (power1 > power2) {
    res = power1;
  } else if (power2 > power1) {
    res = power2;
  } else {
    res = power1;
  }
  return res;
}

int get_bit_big_decimal(s21_big_decimal num, int index) {
  int bit = 0;
  if (num.bits[index / 32] & (1 << index % 32)) {
    bit = 1;
  }
  return bit;
}

void set_bit_big_decimal(s21_big_decimal *num, int index, int bit) {
  if (bit > 0) {
    num->bits[index / 32] |= (1 << index % 32);
  } else {
    num->bits[index / 32] &= ~(1 << index % 32);
  }
}

int poverka_big_decimal(s21_big_decimal value_1) {
  return ((value_1.bits[3] == 0) & (value_1.bits[4] == 0) &
          (value_1.bits[5] == 0) & (value_1.bits[6] == 0) &
          (value_1.bits[7] == 0))
             ? 1
             : 0;
}

s21_big_decimal convert_D_in_bigD(s21_decimal value_1) {
  int index = 0;
  int bit = 0;
  s21_big_decimal num = {{0, 0, 0, 0, 0, 0, 0, 0}};
  while (index != 96) {
    bit = get_bit(value_1, index);
    set_bit_big_decimal(&num, index, bit);
    index++;
  }
  return num;
}

int convert_bigD_in_D(s21_big_decimal value_1, int power, int sign,
                      s21_decimal *result) {
  arithmetic_result code = OK;
  if (poverka_big_decimal(value_1) == 0 && sign == 0) {
    code = BIG;
  } else if (poverka_big_decimal(value_1) == 0 && sign == 1) {
    code = SMALL;
  } else {
    int index = 0;
    int bit = 0;
    while (index != 96) {
      bit = get_bit_big_decimal(value_1, index);
      set_bit(result, index, bit);
      index++;
    }
    set_power(result, power);
    set_sign(result, sign);
  }
  return code;
}

int set_sub_sign_big(s21_decimal value_1, s21_decimal value_2) {
  s21_decimal num1 = value_1;
  s21_decimal num2 = value_2;
  int sign;
  equalize(&num1, &num2);
  if (greate_binary(value_1, value_2) == 1) {
    if (get_sign(value_1) == 1)
      sign = 1;
    else
      sign = 0;
  } else {
    if (get_sign(value_2) == 1)
      sign = 0;
    else
      sign = 1;
  }
  return sign;
}

int set_add_sign_big(s21_decimal value_1, s21_decimal value_2) {
  s21_decimal num1 = value_1;
  s21_decimal num2 = value_2;
  int sign;
  equalize(&num1, &num2);

  if (greate_binary(num1, num2) == 1) {
    if (get_sign(value_1) == 1)
      sign = 1;
    else
      sign = 0;
  } else {
    if (get_sign(value_2) == 0)
      sign = 0;
    else
      sign = 1;
  }
  return sign;
}
int add_binary(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int code = 0;
  int index = 0;
  int sum;
  int carry = 0;

  while (index < 96) {
    sum = get_bit(value_1, index) + get_bit(value_2, index) + carry;
    carry = sum / 2;
    set_bit(result, index++, sum % 2);
  }
  if (carry) {
    code = 1;
  }
  return code;
}

int add_binary_256(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result) {
  arithmetic_result code = OK;
  int index = 0;
  int sum;
  int carry = 0;

  while (index != 223) {
    sum = get_bit_big_decimal(value_1, index) +
          get_bit_big_decimal(value_2, index) + carry;
    carry = sum / 2;
    set_bit_big_decimal(result, index++, sum % 2);
  }
  if (carry) {
    code = BIG;
  }
  return code;
}

int sub_binary_256(s21_big_decimal num1, s21_big_decimal num2,
                   s21_big_decimal *result) {
  arithmetic_result code = OK;
  s21_big_decimal big_num;
  s21_big_decimal small_num;
  if (greate_binary_big(num1, num2) == 1) {
    big_num = num1;
    small_num = num2;
  } else {
    big_num = num2;
    small_num = num1;
  }
  if (result == NULL) {
    code = ERROR;
  }
  int index = 0;
  int diff, borrow = 0;
  while (index < 224) {
    int bit1 = get_bit_big_decimal(big_num, index);
    int bit2 = get_bit_big_decimal(small_num, index);
    diff = bit1 - bit2 - borrow;
    if (diff < 0) {
      diff += 2;
      borrow = 1;
    } else {
      borrow = 0;
    }
    set_bit_big_decimal(result, index++, diff);
  }
  if (borrow != 0) {
    code = ERROR;
  }
  return code;
}

int greate_binary_big(s21_big_decimal value1, s21_big_decimal value2) {
  int res = 1;
  for (int i = 6; i >= 0; i--) {
    if (value1.bits[i] > value2.bits[i]) {
      res = 1;
    } else if (value1.bits[i] < value2.bits[i]) {
      res = 2;
    }
  }
  return res;
}

void bank_round_256(s21_big_decimal *value) {
  int last = last_digit_big(value);
  if (last > 5) {
    add_one_mnts_big(value);
  } else if (last == 5) {
    if (get_bit_big_decimal(*value, 0)) {
      add_one_mnts_big(value);
    }
  }
}

int add_one_mnts_big(s21_big_decimal *value) {
  int carry = 1;
  int err_code = 0;
  for (int i = 0; i < 7; i++) {
    unsigned long long temp = (unsigned long long)value->bits[i] + carry;
    value->bits[i] = (unsigned int)(temp & 0xFFFFFFFF);
    carry = (unsigned int)(temp >> 32);
  }
  if (carry != 0) {
    err_code = 1;
  }
  return err_code;
}

int last_digit_big(s21_big_decimal *value) {
  int remainder = 0;
  unsigned long long temp = 0;
  for (int i = 6; i >= 0; i--) {
    temp = value->bits[i] + remainder * 4294967296ULL;
    remainder = temp % 10;
    value->bits[i] = (unsigned int)(temp / 10);
  }
  return remainder;
}

void divide_by_10(s21_big_decimal *num) {
  long long carry = 0;
  for (int i = 7; i >= 0; i--) {
    long long temp = (carry << 32) | num->bits[i];
    num->bits[i] = temp / 10;
    carry = temp % 10;
  }
}

int fits_in_decimal(const s21_big_decimal *num) {
  int flag = 1;
  for (int i = 3; i < 6; i++) {
    if (num->bits[i] != 0) {
      flag = 0;
    }
  }
  return flag;
}

int count_divisions_needed(s21_big_decimal *num) {
  int count = 0;
  s21_big_decimal temp = *num;
  while (1) {
    if (fits_in_decimal(&temp)) {
      break;
    }
    divide_by_10(&temp);
    count++;
  }

  return count;
}

int is_zero(s21_decimal num) {
  int res = 1;
  for (int i = 95; i >= 0; i--) {
    if (get_bit(num, i) == 1) {
      res = 0;
    }
  }
  return res;
}

int greate_binary(s21_decimal value1, s21_decimal value2) {
  int res = 1;
  for (int i = 2; i >= 0; i--) {
    if (value1.bits[i] > value2.bits[i]) {
      res = 1;
    } else if (value1.bits[i] < value2.bits[i]) {
      res = 2;
    }
  }
  return res;
}

int is_zero_256(s21_big_decimal value_1) {
  int res = 1;
  if ((value_1.bits[0] == 0) && (value_1.bits[1] == 0) &&
      (value_1.bits[2] == 0) && (value_1.bits[3] == 0) &&
      (value_1.bits[4] == 0) && (value_1.bits[5] == 0) &&
      (value_1.bits[6] == 0) && (value_1.bits[7] == 0))
    res = 0;
  return res;
}

int mul_binary_256(s21_big_decimal value_1, s21_big_decimal value_2,
                   s21_big_decimal *result) {
  arithmetic_result code = OK;
  if ((is_zero_256(value_1) == 0) || (is_zero_256(value_2) == 0)) {
    result = 0;
  } else {
    for (int i = 0; i < 256; i++) {
      if (get_bit_big_decimal(value_1, i) == 1)
        code = add_binary_256(*result, shift_left_big(value_2, i), result);
    }
  }
  return code;
}

int s21_big_decimal_compare(s21_big_decimal a, s21_big_decimal b) {
  int res = 0;
  for (int i = 7; i >= 0; i--) {
    if (a.bits[i] < b.bits[i]) res = -1;
    if (a.bits[i] > b.bits[i]) res = 1;
  }
  return res;
}

int div_256(s21_big_decimal value_1, s21_big_decimal value_2,
            s21_big_decimal *q, s21_big_decimal *r) {
  arithmetic_result code = OK;
  if (is_zero_256(value_2) == 0) {
    code = ZERO_DIV;
  } else {
    s21_big_decimal quotient = {{0, 0, 0, 0, 0, 0, 0, 0}};
    s21_big_decimal remainder = {{0, 0, 0, 0, 0, 0, 0, 0}};
    for (int i = 224; i >= 0; i--) {
      remainder = shift_left_big(remainder, 1);
      set_bit_big_decimal(&remainder, 0, get_bit_big_decimal(value_1, i));

      if (s21_big_decimal_compare(remainder, value_2) >= 0) {
        sub_binary_256(remainder, value_2, &remainder);
        set_bit_big_decimal(&quotient, i, 1);
      }
    }
    *q = quotient;
    *r = remainder;
  }
  return code;
}

int div_brain(s21_big_decimal value_1, s21_big_decimal value_2,
              s21_big_decimal *result, int *iteration) {
  arithmetic_result code = OK;
  s21_big_decimal quotient = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal remainder = {{0, 0, 0, 0, 0, 0, 0, 0}};
  s21_big_decimal res = {{0, 0, 0, 0, 0, 0, 0, 0}};
  int s = 0;
  int count = 0;
  div_256(value_1, value_2, &quotient, &remainder);
  while ((s == 0) & (count < 28)) {
    if (is_zero_256(remainder) == 0) {
      s = 1;
      add_binary_256(res, quotient, &res);
    } else {
      add_binary_256(res, quotient, &res);
      add_binary_256(shift_left_big(res, 3), shift_left_big(res, 1), &res);
      add_binary_256(shift_left_big(remainder, 3), shift_left_big(remainder, 1),
                     &remainder);
      code = div_256(remainder, value_2, &quotient, &remainder);
      (*iteration)++;
      count++;
    }
    if (count == 28) {
      (*iteration)--;
    }
  }
  *result = res;
  return code;
}
