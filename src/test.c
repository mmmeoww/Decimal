#include <check.h>
#include <math.h>
#include <stdio.h>

#include "s21_decimal.h"

#define OK 0
#define ERROR 1

void test_other_ok(s21_decimal result, s21_decimal expected, int code, int i) {
  ck_assert_int_eq(code, 0);
  for (int i = 3; i >= 0; i--) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
  printf("%d) \033[0;32m[PASS]\033[0;0m correct, code = 0\n", i);
}

void test_other_fail(int code, int i) {
  ck_assert_int_eq(code, 1);
  printf("%d) \033[0;32m[PASS]\033[0;0m error, code = 1\n", i);
}

void test_arith_ok(s21_decimal result, s21_decimal expected, int code, int i) {
  ck_assert_int_eq(code, 0);
  for (int i = 3; i >= 0; i--) {
    ck_assert_int_eq(result.bits[i], expected.bits[i]);
  }
  printf("%d) \033[0;32m[PASS]\033[0;0m correct, code = 0\n", i);
}

void test_arith_big(int code, int i) {
  ck_assert_int_eq(code, 1);
  printf("%d) \033[0;32m[PASS]\033[0;0m big overflow, code = 1\n", i);
}

void test_arith_fail(int code, int i) {
  ck_assert_int_eq(code, 4);
  printf("%d) \033[0;32m[PASS]\033[0;0m incorrect values, code = 4)\n", i);
}

void test_arith_small(int code, int i) {
  ck_assert_int_eq(code, 2);
  printf("%d) \033[0;32m[PASS]\033[0;0m small overflow, code = 2\n", i);
}

void test_arith_divbyzero(int code, int i) {
  ck_assert_int_eq(code, 3);
  printf("%d) \033[0;32m[PASS]\033[0;0m division by zero, code = 3\n", i);
}

void test_compr_true(int code) {
  ck_assert_int_eq(code, 1);
  printf("\033[0;32m[PASS]\033[0;0m true comparing, code = 1\n");
}

void test_compr_false(int code) {
  ck_assert_int_eq(code, 0);
  printf("\033[0;32m[PASS]\033[0;0m false comparing, code = 0\n");
}

START_TEST(test_from_int_to_decimal_fail) {
  printf("\nTesting s21_from_int_to_decimal_fail:\n");
  int numbers[] = {0, 1, -0, -1, 20, -20, 2147483647, -2147483648};
  for (int i = 0; i < 8; i++) {
    ck_assert_int_eq(s21_from_int_to_decimal(numbers[i], NULL), ERROR);
    printf("\033[0;32m[PASS]\033[0;0m s21_from_int_to_decimal_fail(%d, NULL)\n",
           numbers[i]);
  }
}
END_TEST

START_TEST(test_from_int_to_decimal_ok) {
  printf("\nTesting s21_from_int_to_decimal_ok:\n");
  int numbers[] = {0,          1,           -1,       20,        -20,
                   2147483647, -2147483648, 21474836, -21474836, 214748,
                   -214748,    2147,        -2147};
  s21_decimal check[] = {{{0x0, 0x0, 0x0, 0x0}},
                         {{0x1, 0x0, 0x0, 0x0}},
                         {{0x1, 0x0, 0x0, 0x80000000}},
                         {{0x14, 0x0, 0x0, 0x0}},
                         {{0x14, 0x0, 0x0, 0x80000000}},
                         {{0x7FFFFFFF, 0x0, 0x0, 0x0}},
                         {{0x80000000, 0x0, 0x0, 0x80000000}},
                         {{0x147AE14, 0x0, 0x0, 0x0}},
                         {{0x147AE14, 0x0, 0x0, 0x80000000}},
                         {{0x346DC, 0x0, 0x0, 0x0}},
                         {{0x346DC, 0x0, 0x0, 0x80000000}},
                         {{0x863, 0x0, 0x0, 0x0}},
                         {{0x863, 0x0, 0x0, 0x80000000}}};
  s21_decimal result;
  // = {{0, 0, 0, 0}};
  for (int i = 0; i < 13; i++) {
    ck_assert_int_eq(s21_from_int_to_decimal(numbers[i], &result), OK);
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(result.bits[j], check[i].bits[j]);
    }
    printf("\033[0;32m[PASS]\033[0;0m s21_from_int_to_decimal_ok(%d)\n",
           numbers[i]);
  }
}
END_TEST

START_TEST(test_from_decimal_to_int_fail) {
  printf("\nTesting s21_from_decimal_to_int_fail:\n");
  s21_decimal zero_decimal = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_decimal_to_int(zero_decimal, NULL), ERROR);
  printf(
      "\033[0;32m[PASS]\033[0;0m s21_from_decimal_to_int_fail(zero_decimal, "
      "NULL)\n");
  s21_decimal incorrect_decimal[] = {
      {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
      {{0x0, 0x0, 0x0, 0xFE000000}},
      {{0x0, 0xFFFFFF, 0x0, 0x9A021E18}},
      {{0x0, 0x0, 0x0, 0x1}}};
  int result = 0;
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(s21_from_decimal_to_int(incorrect_decimal[i], &result),
                     ERROR);
    printf(
        "\033[0;32m[PASS]\033[0;0m "
        "s21_from_decimal_to_int_fail(incorrect_decimal, int)\n");
  }
}
END_TEST

START_TEST(test_from_decimal_to_int_ok) {
  printf("\nTesting s21_from_decimal_to_int_ok:\n");
  s21_decimal correct_dec[] = {
      {{0x0, 0x7FFFFFFF, 0x0, 0x0}},
      {{0x1, 0x0, 0x7FFFFFFF, 0x0}},
      {{0x1, 0x0, 0x0, 0x80000000}},
      {{0x14, 0x7FFFFFFF, 0x0, 0x0}},
      {{0x14, 0x0, 0x0, 0x80000000}},
      {{0x7FFFFFFF, 0x0, 0x0, 0x0}},
      {{0x80000000, 0x0, 0x0, 0x80000000}},
      {{0x147AE14, 0x0, 0x0, 0x0}},
      {{0x147AE14, 0x0, 0x0, 0x80000000}},
      {{0x346DC, 0x7FFFFFFF, 0x0, 0x0}},
      {{0x346DC, 0x0, 0x0, 0x80000000}},
      {{0x863, 0x0, 0x7FFFFFFF, 0x0}},
      {{0x863, 0x0, 0xFFFFFFFF, 0x80000000}},
      {{0x3411BC, 0x0, 0x0, 0x0}},
      {{0x9304, 0x0, 0x0, 0x0}},
      {{0x343E530, 0xFFFFFFFF, 0x0, 0x0}},
      {{0x343E530, 0x0, 0x0, 0x80000000}},
      {{0x1344224, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
      {{0x1344224, 0xFFFFFFFF, 0x0, 0x80000000}},
  };
  int check[] = {0,          1,           -1,       20,        -20,
                 2147483647, -2147483648, 21474836, -21474836, 214748,
                 -214748,    2147,        -2147,    3412412,   37636,
                 54781232,   -54781232,   20202020, -20202020};
  int result = 0;
  for (int i = 0; i < 19; i++) {
    ck_assert_int_eq(s21_from_decimal_to_int(correct_dec[i], &result), OK);
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(result, check[i]);
    }
    printf(
        "\033[0;32m[PASS]\033[0;0m s21_from_decimal_to_int_ok(dec%d, int); int "
        "= %d\n",
        i, result);
  }
}
END_TEST

START_TEST(test_from_float_to_decimal_fail) {
  printf("\nTesting s21_from_float_to_decimal_fail:\n");
  float numbers[] = {0.0f,   1.0f,    -0.231f,     -1.412f,
                     20.20f, -20.20f, 214748.321f, -214.132f};
  for (int i = 0; i < 8; i++) {
    ck_assert_int_eq(s21_from_float_to_decimal(numbers[i], NULL), ERROR);
    printf(
        "\033[0;32m[PASS]\033[0;0m s21_from_float_to_decimal_fail(%f, NULL)\n",
        numbers[i]);
  }

  float error_numbers[] = {
      INFINITY,
      -INFINITY,
      NAN,
      -NAN,
      21649172462124614162421783216381268343.f,
      -21649172462124614162421783216381268343.f,
      0.000000000000000000000000000032199325429f,
      -0.000000000000000000000000000032199325429f,
  };
  s21_decimal zero_dec = {{0, 0, 0, 0}};
  for (int i = 0; i < 8; i++) {
    s21_decimal result;
    ck_assert_int_eq(s21_from_float_to_decimal(error_numbers[i], &result),
                     ERROR);
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(result.bits[j], zero_dec.bits[j]);
    }
    printf(
        "\033[0;32m[PASS]\033[0;0m s21_from_float_to_decimal_fail(%e, NULL)\n",
        error_numbers[i]);
  }
}
END_TEST

START_TEST(test_from_float_to_decimal_ok) {
  printf("\nTesting s21_from_float_to_decimal_ok:\n");
  float numbers[] = {
      2.38582807E-08, -2.38582807E-08, 2.28401628E-19, -2.28401628E-19,
      7.91617864E+11, -7.91617864E+11, 2.1349025E-18,  -2.1349025E-18,
      9.29209423E+24, -9.29209423E+24, 0.000115481133, -0.000115481133,
      1.54636995E-16, -1.54636995E-16, 1.9929916E-26,  -1.9929916E-26,
      9.83161032E+17, -9.83161032E+17, 1.19421885E-18, -1.19421885E-18,
      9.61645806E-28, -9.61645806E-28};
  s21_decimal check[] = {{{0x2467A4, 0x0, 0x0, 0xE0000}},
                         {{0x2467A4, 0x0, 0x0, 0x800E0000}},
                         {{0x22D9F0, 0x0, 0x0, 0x190000}},
                         {{0x22D9F0, 0x0, 0x0, 0x80190000}},
                         {{0x501A8DE0, 0xB8, 0x0, 0x0}},
                         {{0x501A8DE0, 0xB8, 0x0, 0x80000000}},
                         {{0x209376, 0x0, 0x0, 0x180000}},
                         {{0x209376, 0x0, 0x0, 0x80180000}},
                         {{0xE380000, 0x6CD28004, 0x7AFAD, 0x0}},
                         {{0xE380000, 0x6CD28004, 0x7AFAD, 0x80000000}},
                         {{0x119EFB, 0x0, 0x0, 0xA0000}},
                         {{0x119EFB, 0x0, 0x0, 0x800A0000}},
                         {{0x25C0D, 0x0, 0x0, 0x150000}},
                         {{0x25C0D, 0x0, 0x0, 0x80150000}},
                         {{0xC7, 0x0, 0x0, 0x1C0000}},
                         {{0xC7, 0x0, 0x0, 0x801C0000}},
                         {{0x72049000, 0xDA4E3B8, 0x0, 0x0}},
                         {{0x72049000, 0xDA4E3B8, 0x0, 0x80000000}},
                         {{0x1238EB, 0x0, 0x0, 0x180000}},
                         {{0x1238EB, 0x0, 0x0, 0x80180000}},
                         {{0x1, 0x0, 0x0, 0x1B0000}},
                         {{0x1, 0x0, 0x0, 0x801B0000}}};
  for (int i = 0; i < 22; i++) {
    s21_decimal result;
    ck_assert_int_eq(s21_from_float_to_decimal(numbers[i], &result), OK);
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(result.bits[j], check[i].bits[j]);
      printf("\033[0;32m[PASS]\033[0;0m s21_from_float_to_decimal_ok(%e)\n",
             numbers[i]);
    }
  }
}
END_TEST

START_TEST(test_from_decimal_to_float_fail) {
  printf("\nTesting s21_from_decimal_to_float_fail:\n");
  s21_decimal zero_decimal = {{0, 0, 0, 0}};
  ck_assert_int_eq(s21_from_decimal_to_float(zero_decimal, NULL), ERROR);
  printf(
      "\033[0;32m[PASS]\033[0;0m s21_from_decimal_to_float_fail(zero_decimal, "
      "NULL)\n");
  s21_decimal incorrect_decimal[] = {
      {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
      {{0x0, 0x0, 0x0, 0xFE000000}},
      {{0x0, 0xFFFFFF, 0x0, 0x9A021E18}},
      {{0x0, 0x0, 0x0, 0x1}}};
  float result = 0;
  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(s21_from_decimal_to_float(incorrect_decimal[i], &result),
                     ERROR);
    printf(
        "\033[0;32m[PASS]\033[0;0m "
        "s21_from_decimal_to_float_fail(incorrect_decimal, float)\n");
  }
}
END_TEST

START_TEST(test_from_decimal_to_float_ok) {
  printf("\nTesting s21_from_decimal_to_float_ok:\n");
  s21_decimal correct_dec[] = {{{0x2467A4, 0x0, 0x0, 0xE0000}},
                               {{0x2467A4, 0x0, 0x0, 0x800E0000}},
                               {{0x22D9F0, 0x0, 0x0, 0x190000}},
                               {{0x22D9F0, 0x0, 0x0, 0x80190000}},
                               {{0x209376, 0x0, 0x0, 0x180000}},
                               {{0x209376, 0x0, 0x0, 0x80180000}},
                               {{0xE380000, 0x6CD28004, 0x7AFAD, 0x0}},
                               {{0xE380000, 0x6CD28004, 0x7AFAD, 0x80000000}},
                               {{0x119EFB, 0x0, 0x0, 0xA0000}},
                               {{0x119EFB, 0x0, 0x0, 0x800A0000}},
                               {{0x25C0D, 0x0, 0x0, 0x150000}},
                               {{0x25C0D, 0x0, 0x0, 0x80150000}},
                               {{0xC7, 0x0, 0x0, 0x1C0000}},
                               {{0xC7, 0x0, 0x0, 0x801C0000}},
                               {{0x72049000, 0xDA4E3B8, 0x0, 0x0}},
                               {{0x72049000, 0xDA4E3B8, 0x0, 0x80000000}},
                               {{0x1238EB, 0x0, 0x0, 0x180000}},
                               {{0x1238EB, 0x0, 0x0, 0x80180000}}};
  float check[] = {2.38582807E-08,  -2.38582807E-08, 2.28401628E-19,
                   -2.28401628E-19, 2.1349025E-18,   -2.1349025E-18,
                   9.29209423E+24,  -9.29209423E+24, 0.000115481133,
                   -0.000115481133, 1.54636995E-16,  -1.54636995E-16,
                   1.9929916E-26,   -1.9929916E-26,  9.83161032E+17,
                   -9.83161032E+17, 1.19421885E-18,  -1.19421885E-18};
  for (int i = 0; i < 18; i++) {
    float result;
    ck_assert_int_eq(s21_from_decimal_to_float(correct_dec[i], &result), OK);
    ck_assert_float_eq_tol(result, check[i], 1e-7);
    printf(
        "\033[0;32m[PASS]\033[0;0m s21_from_decimal_to_float_ok(correct_dec, "
        "float), float = %e\n",
        check[i]);
  }
}
END_TEST

START_TEST(test_s21_decimal_is_greater) {
  printf("\n\x1b[34mTesting s21_is_greater:\x1b[0m\n\n");

  s21_decimal greater_values[][2] = {
      {{{0x2, 0x0, 0x0, 0x0}}, {{0x1, 0x0, 0x0, 0x0}}},
      {{{0xA, 0x0, 0x0, 0x0}}, {{0x3, 0x0, 0x0, 0x0}}},
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x1, 0x0, 0x0, 0x80000000}}},
      {{{0x5, 0x0, 0x0, 0x0}}, {{0x4, 0x0, 0x0, 0x0}}},
      {{{0x0, 0x0, 0x0, 0x0}}, {{0x1, 0x0, 0x0, 0x80000000}}},
      {{{0x5F5E100, 0x0, 0x0, 0x001C0000}},
       {{0x3B9ACA0, 0x0, 0x0, 0x001C0000}}},
      {{{0x1F4, 0x0, 0x0, 0x00020000}}, {{0xC8, 0x0, 0x0, 0x00020000}}},
      {{{0x5, 0x0, 0x0, 0x80000000}}, {{0x10, 0x0, 0x0, 0x80000000}}},
      {{{0x152F, 0x0, 0x0, 0x00030000}}, {{0x152F, 0x0, 0x0, 0x00040000}}}};

  s21_decimal lesser_or_equal_values[][2] = {
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x2, 0x0, 0x0, 0x0}}},
      {{{0x3, 0x0, 0x0, 0x80000000}}, {{0x2, 0x0, 0x0, 0x80000000}}},
      {{{0x0, 0x0, 0x0, 0x0}}, {{0x0, 0x0, 0x0, 0x0}}},
      {{{0x1, 0x0, 0x0, 0x80000000}}, {{0x1, 0x0, 0x0, 0x80000000}}},
      {{{0x4, 0x0, 0x0, 0x0}}, {{0x5, 0x0, 0x0, 0x0}}},
      {{{0x5, 0x0, 0x0, 0x0}}, {{0x5, 0x0, 0x0, 0x0}}},
      {{{0x0, 0x0, 0x0, 0x0}}, {{0x0, 0x0, 0x0, 0x80000000}}},
      {{{1234, 0x0, 0x0, 0x00000000}}, {{1234000, 0x0, 0x0, 0x00030000}}},
      {{{0xB219E4E3, 0xE34F18DD, 0x68983A95, 0x1C0000}},
       {{0xC833FFFF, 0x8881186E, 0x4A03CE6, 0x120000}}},
      {{{0x80000000, 0x0, 0x0, 0x80000000}},
       {{0x7FFFFFFF, 0x0, 0x0, 0x80000000}}}};

  int result = 0;

  for (int i = 0; i < 9; i++) {
    result = s21_is_greater(greater_values[i][0], greater_values[i][1]);
    test_compr_true(result);
  }

  for (int i = 0; i < 10; i++) {
    result = s21_is_greater(lesser_or_equal_values[i][0],
                            lesser_or_equal_values[i][1]);
    test_compr_false(result);
  }

  result = s21_is_greater((s21_decimal){{0, 0, 0, 0xFFFFFFFF}},
                          greater_values[0][1]);
  test_compr_false(result);
}
END_TEST

START_TEST(test_s21_decimal_is_greater_or_equal) {
  printf("\n\x1b[34mTesting s21_is_greater_or_equal:\x1b[0m\n\n");

  s21_decimal greater_values[][2] = {
      {{{0x2, 0x0, 0x0, 0x0}}, {{0x1, 0x0, 0x0, 0x0}}},
      {{{0x5, 0x0, 0x0, 0x0}}, {{0x5, 0x0, 0x0, 0x0}}},
      {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
       {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}},
      {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
       {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}},
  };

  s21_decimal lesser_or_equal_values[][2] = {
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x2, 0x0, 0x0, 0x0}}},
      {{{0x4, 0x0, 0x0, 0x0}}, {{0x5, 0x0, 0x0, 0x0}}},
      {{{0x3, 0x0, 0x0, 0x80000000}}, {{0x2, 0x0, 0x0, 0x0}}},
      {{{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
       {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}}};

  int result = 0;

  for (int i = 0; i < 4; i++) {
    result =
        s21_is_greater_or_equal(greater_values[i][0], greater_values[i][1]);
    test_compr_true(result);
  }

  for (int i = 0; i < 4; i++) {
    result = s21_is_greater_or_equal(lesser_or_equal_values[i][0],
                                     lesser_or_equal_values[i][1]);

    test_compr_false(result);
  }
}
END_TEST

START_TEST(test_s21_decimal_is_equal) {
  printf("\n\x1b[34mTesting s21_is_equal:\x1b[0m\n\n");

  s21_decimal equal_values[][2] = {
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x1, 0x0, 0x0, 0x0}}},
      {{{0xA, 0x0, 0x0, 0x0}}, {{0xA, 0x0, 0x0, 0x0}}},
      {{{0x5, 0x0, 0x0, 0x0}}, {{0x5, 0x0, 0x0, 0x0}}},
      {{{0x0, 0x0, 0x0, 0x0}}, {{0x0, 0x0, 0x0, 0x0}}},
      {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}},
       {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}}},
      {{{0x1, 0x0, 0x0, 0x80000000}}, {{0x1, 0x0, 0x0, 0x80000000}}},
  };

  s21_decimal not_equal_values[][2] = {
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x2, 0x0, 0x0, 0x0}}},
      {{{0x3, 0x0, 0x0, 0x80000000}}, {{0x2, 0x0, 0x0, 0x80000000}}},
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x1, 0x0, 0x0, 0x80000000}}},
      {{{0xC833FFFF, 0x8881186E, 0x4A03CE6, 0x80120000}},
       {{0xA15E6E57, 0xC0C562E8, 0xA41268B3, 0x160000}}},
      {{{12345, 0, 0, 0x10000}}, {{12345, 0, 0, 0x20000}}},
      {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}, {{0x0, 0x0, 0x0, 0x0}}},
      {{{0x4, 0x0, 0x0, 0x0}}, {{0x5, 0x0, 0x0, 0x0}}},
  };

  int result = 0;

  for (int i = 0; i < 6; i++) {
    result = s21_is_equal(equal_values[i][0], equal_values[i][1]);
    test_compr_true(result);
  }

  for (int i = 0; i < 7; i++) {
    result = s21_is_equal(not_equal_values[i][0], not_equal_values[i][1]);
    test_compr_false(result);
  }
}
END_TEST

START_TEST(test_s21_decimal_is_not_equal) {
  printf("\n\x1b[34mTesting s21_is_not_equal:\x1b[0m\n\n");

  s21_decimal not_equal_values[][2] = {
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x2, 0x0, 0x0, 0x0}}},
      {{{0x3, 0x0, 0x0, 0x80000000}}, {{0x2, 0x0, 0x0, 0x80000000}}},
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x1, 0x0, 0x0, 0x80000000}}},
      {{{0x4, 0x0, 0x0, 0x0}}, {{0x5, 0x0, 0x0, 0x0}}}};

  s21_decimal equal_values[][2] = {
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x1, 0x0, 0x0, 0x0}}},
      {{{0xA, 0x0, 0x0, 0x0}}, {{0xA, 0x0, 0x0, 0x0}}},
      {{{0x5, 0x0, 0x0, 0x0}}, {{0x5, 0x0, 0x0, 0x0}}},
      {{{0x0, 0x0, 0x0, 0x0}}, {{0x0, 0x0, 0x0, 0x0}}},
      {{{0x1, 0x0, 0x0, 0x80000000}}, {{0x1, 0x0, 0x0, 0x80000000}}},
  };

  int result = 0;

  for (int i = 0; i < 4; i++) {
    result = s21_is_not_equal(not_equal_values[i][0], not_equal_values[i][1]);
    test_compr_true(result);
  }

  for (int i = 0; i < 5; i++) {
    result = s21_is_not_equal(equal_values[i][0], equal_values[i][1]);
    test_compr_false(result);
  }
}
END_TEST

START_TEST(test_s21_decimal_is_less) {
  printf("\n\x1b[34mTesting s21_is_less:\x1b[0m\n\n");

  s21_decimal less_values[][2] = {
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x2, 0x0, 0x0, 0x0}}},
      {{{0x3, 0x0, 0x0, 0x80000000}}, {{0x2, 0x0, 0x0, 0x80000000}}},
      {{{0x1, 0x0, 0x0, 0x80000000}}, {{0x1, 0x0, 0x0, 0x0}}},
      {{{0x4, 0x0, 0x0, 0x0}}, {{0x5, 0x0, 0x0, 0x0}}},
  };

  s21_decimal not_less_values[][2] = {
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x1, 0x0, 0x0, 0x0}}},
      {{{0xA, 0x0, 0x0, 0x0}}, {{0x3, 0x0, 0x0, 0x0}}},
      {{{0x5, 0x0, 0x0, 0x0}}, {{0x4, 0x0, 0x0, 0x0}}},
      {{{0x0, 0x0, 0x0, 0x0}}, {{0x0, 0x0, 0x0, 0x0}}},
      {{{0x1, 0x0, 0x0, 0x80000000}}, {{0x1, 0x0, 0x0, 0x80000000}}},
  };

  int result = 0;

  for (int i = 0; i < 4; i++) {
    result = s21_is_less(less_values[i][0], less_values[i][1]);
    test_compr_true(result);
  }

  for (int i = 0; i < 5; i++) {
    result = s21_is_less(not_less_values[i][0], not_less_values[i][1]);
    test_compr_false(result);
  }
}
END_TEST

START_TEST(test_s21_decimal_is_less_or_equal) {
  printf("\n\x1b[34mTesting s21_is_less_or_equal:\x1b[0m\n\n");

  s21_decimal less_or_equal_values[][2] = {
      {{{0x1, 0x0, 0x0, 0x0}}, {{0x2, 0x0, 0x0, 0x0}}},
      {{{0x3, 0x0, 0x0, 0x80000000}}, {{0x3, 0x0, 0x0, 0x0}}},
      {{{0x1, 0x0, 0x0, 0x80000000}}, {{0x1, 0x0, 0x0, 0x0}}},
      {{{0x4, 0x0, 0x0, 0x0}}, {{0x5, 0x0, 0x0, 0x0}}},
      {{{0x0, 0x0, 0x0, 0x0}}, {{0x0, 0x0, 0x0, 0x0}}},
      {{{0x1, 0x0, 0x0, 0x80000000}}, {{0x1, 0x0, 0x0, 0x80000000}}},
  };

  s21_decimal not_less_or_equal_values[][2] = {
      {{{0xA, 0x0, 0x0, 0x0}}, {{0x1, 0x0, 0x0, 0x0}}},
      {{{0xA, 0x0, 0x0, 0x0}}, {{0x3, 0x0, 0x0, 0x0}}},
      {{{0x5, 0x0, 0x0, 0x0}}, {{0x4, 0x0, 0x0, 0x0}}},
  };

  int result = 0;

  for (int i = 0; i < 6; i++) {
    result = s21_is_less_or_equal(less_or_equal_values[i][0],
                                  less_or_equal_values[i][1]);
    test_compr_true(result);
  }

  for (int i = 0; i < 3; i++) {
    result = s21_is_less_or_equal(not_less_or_equal_values[i][0],
                                  not_less_or_equal_values[i][1]);
    test_compr_false(result);
  }
}
END_TEST

START_TEST(test_s21_floor_error) {
  printf("\n\x1b[34mTesting s21_floor (error: incorrect values)\x1b[0m\n\n");
  s21_decimal values[] = {{{7, 0, 0, 0x1D0000}},
                          {{0xAF67, 0, 0, 0x11C0000}},
                          {{0xFFFFFFFF, 0x45FED8, 0xFFEE, 0xFFFFFFFF}},
                          {{0xFFEE, 0xFEABC9, 0xEFEF, 0x1C0001}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1404600}},
                          {{0, 0, 0, 0x1D0000}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                          {{432959, 3252345, 3124214, 0x401C0000}},
                          {{0, 0, 0, 100000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_floor(values[i], &res);
    test_other_fail(code, i + 1);
  }
  s21_decimal example = {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x40000}};
  code = s21_floor(example, NULL);
  test_other_fail(code, (int)(sizeof(values) / sizeof(s21_decimal)) + 1);
}
END_TEST

START_TEST(test_s21_floor_ok1) {
  printf("\n\x1b[34mTesting s21_floor (ok: correct values):\x1b[0m\n\n");
  s21_decimal values[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}},
                          {{0x1, 0x0, 0x1, 0x80010000}},
                          {{0x1, 0x0, 0x1, 0x80130000}},
                          {{0x0, 0x1, 0x0, 0x10000}},
                          {{0x1, 0x0, 0x0, 0xE0000}},
                          {{0x1, 0x0, 0x0, 0x800E0000}},
                          {{0x80000000, 0x80000000, 0x80000000, 0x80000000}},
                          {{0x88000000, 0x1F128130, 0x1027E72F, 0x801C0000}},
                          {{0xEFE99B1F, 0x9755E57A, 0xF04F1D48, 0x150000}},
                          {{0x24506134, 0xB7AFB101, 0x49FDFCA5, 0x1A0000}},
                          {{0x0, 0x0, 0x0, 0x0}},
                          {{0x0, 0x0, 0x0, 0x80000000}},
                          {{12340000, 0x0, 0x0, 0x80030000}},
                          {{0x10000000, 0x3E250261, 0x204FCE5E, 0x801C0000}},
                          {{0x540BE400, 0x2, 0x0, 0x800A0000}}};
  s21_decimal expected[] = {{{0x9999999A, 0x99999999, 0x19999999, 0x80000000}},
                            {{0x9999999A, 0x19999999, 0x0, 0x80000000}},
                            {{0x2, 0x0, 0x0, 0x80000000}},
                            {{0x19999999, 0x0, 0x0, 0x0}},
                            {{0x0, 0x0, 0x0, 0x0}},
                            {{0x1, 0x0, 0x0, 0x80000000}},
                            {{0x80000000, 0x80000000, 0x80000000, 0x80000000}},
                            {{0x1, 0x0, 0x0, 0x80000000}},
                            {{0x46ED3CD, 0x0, 0x0, 0x0}},
                            {{0xE4, 0x0, 0x0, 0x0}},
                            {{0x0, 0x0, 0x0, 0x0}},
                            {{0x0, 0x0, 0x0, 0x80000000}},
                            {{12340, 0x0, 0x0, 0x80000000}},
                            {{0x1, 0x0, 0x0, 0x80000000}},
                            {{0x1, 0x0, 0x0, 0x80000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_floor(values[i], &res);
    test_other_ok(res, expected[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_floor_ok2) {
  printf("\n\x1b[34mTesting s21_floor (ok: correct values):\x1b[0m\n\n");
  s21_decimal values[] = {{{0x9CDB1C8F, 0x55BD91CF, 0xC51774F7, 0x800B0000}},
                          {{0xA2E46736, 0xF63C579F, 0x59BAC66A, 0x800D0000}},
                          {{0x7CF7B0AF, 0xBA1E0F96, 0x87132F00, 0x1A0000}},
                          {{0x64, 0x0, 0x0, 0x20000}},
                          {{0x0, 0x0, 0x0, 0x801C0000}},
                          {{0x80000000, 0x80000000, 0x0, 0x80120000}},
                          {{0x811800, 0x811800, 0x811800, 0x801A0000}},
                          {{0x55555555, 0x55555555, 0x55555555, 0x80000000}},
                          {{0x0, 0x0, 0x0, 0x1C0000}},
                          {{0x9654F575, 0x46D837B7, 0x7CD04BE8, 0x90000}},
                          {{0x431C139E, 0x67299B35, 0xF2952A3, 0x801B0000}},
                          {{0x71A2C1A4, 0x57EFFA09, 0x6599516, 0x50000}},
                          {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x801C0000}},
                          {{0x3E8, 0x0, 0x0, 0x80030000}},
                          {{0x540BE400, 0x2, 0x0, 0xA0000}}};
  s21_decimal expected[] = {{{0x8E492857, 0x8770B9D, 0x0, 0x80000000}},
                            {{0xD67861A7, 0x9DDA9, 0x0, 0x80000000}},
                            {{0x1A2, 0x0, 0x0, 0x0}},
                            {{0x1, 0x0, 0x0, 0x0}},
                            {{0x0, 0x0, 0x0, 0x80000000}},
                            {{0xA, 0x0, 0x0, 0x80000000}},
                            {{0x2, 0x0, 0x0, 0x80000000}},
                            {{0x55555555, 0x55555555, 0x55555555, 0x80000000}},
                            {{0x0, 0x0, 0x0, 0x0}},
                            {{0xDA197F11, 0x1812118C, 0x2, 0x0}},
                            {{0x5, 0x0, 0x0, 0x80000000}},
                            {{0xCCE7F5EE, 0x5777413F, 0x429, 0x0}},
                            {{0x1, 0x0, 0x0, 0x80000000}},
                            {{0x1, 0x0, 0x0, 0x80000000}},
                            {{0x1, 0x0, 0x0, 0x0}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_floor(values[i], &res);
    test_other_ok(res, expected[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_round_error) {
  printf("\n\x1b[34mTesting s21_round (error: incorrect values):\x1b[0m\n\n");
  s21_decimal values[] = {{{7, 0, 0, 0x1D0000}},
                          {{0xAF67, 0, 0, 0x11C0000}},
                          {{0xFFFFFFFF, 0x45FED8, 0xFFEE, 0xFFFFFFFF}},
                          {{0xFFEE, 0xFEABC9, 0xEFEF, 0x1C0001}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1404600}},
                          {{0, 0, 0, 0x1D0000}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                          {{432959, 3252345, 3124214, 0x401C0000}},
                          {{0, 0, 0, 100000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_round(values[i], &res);
    test_other_fail(code, i + 1);
  }
  s21_decimal example = {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x40000}};
  code = s21_round(example, NULL);
  test_other_fail(code, (int)(sizeof(values) / sizeof(s21_decimal)) + 1);
}
END_TEST

START_TEST(test_s21_round_ok1) {
  printf("\n\x1b[34mTesting s21_round (ok: correct values):\x1b[0m\n\n");
  s21_decimal values[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xA0000}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x801C0000}},
                          {{0x0, 0x80000000, 0x0, 0x10000}},
                          {{0x811800, 0x811800, 0x811800, 0x80140000}},
                          {{0x0, 0x811800, 0x811800, 0x80000000}},
                          {{0x811800, 0x0, 0x811800, 0x10000}},
                          {{0x0, 0x0, 0x811800, 0x801A0000}},
                          {{0x449AEF8D, 0x886E6706, 0x1B1B4C40, 0x130000}},
                          {{0xB8CF61F3, 0xA4D11E22, 0xC7C50C60, 0x80040000}},
                          {{0x224AB663, 0x289551F5, 0xFF45652, 0x80060000}},
                          {{0x61E7D194, 0xA162A112, 0x997E4C83, 0x80010000}},
                          {{0xDED2AF13, 0x18A058D4, 0xBDCDC1F3, 0x1C0000}},
                          {{0x501ED51F, 0xC4084E21, 0x6B5BA353, 0x80030000}},
                          {{0x7055400E, 0xF7463C3D, 0x619A609E, 0x1A0000}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80110000}}};
  s21_decimal expected[] = {{{0x5EF6EADF, 0x6DF37F67, 0x0, 0x0}},
                            {{0x8, 0x0, 0x0, 0x80000000}},
                            {{0xCCCCCCCD, 0xCCCCCCC, 0x0, 0x0}},
                            {{0x17D048, 0x0, 0x0, 0x80000000}},
                            {{0x0, 0x811800, 0x811800, 0x80000000}},
                            {{0xCCD9B59A, 0xCCCCCCCC, 0xCE8CC, 0x0}},
                            {{0x2, 0x0, 0x0, 0x80000000}},
                            {{0x3200BEC3, 0x0, 0x0, 0x0}},
                            {{0x9E66BB6, 0xF9767F6A, 0x51D35, 0x80000000}},
                            {{0x8027223F, 0xABCEA96A, 0x10B, 0x80000000}},
                            {{0x7030C828, 0x29BD101B, 0xF596E0D, 0x80000000}},
                            {{0x6, 0x0, 0x0, 0x0}},
                            {{0x87810E0A, 0xB323D92A, 0x1B7BCF, 0x80000000}},
                            {{0x12E, 0x0, 0x0, 0x0}},
                            {{0x77AA3237, 0xB8, 0x0, 0x80000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_round(values[i], &res);
    test_other_ok(res, expected[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_round_ok2) {
  printf("\n\x1b[34mTesting s21_round (ok: correct values):\x1b[0m\n\n");
  s21_decimal values[] = {{{0xE9C6C9AC, 0x7F868D8F, 0xE41050CA, 0x10000}},
                          {{0x0, 0x0, 0x0, 0x1C0000}},
                          {{0x0, 0x0, 0x0, 0x801C0000}},
                          {{0x0, 0x0, 0xFFFED, 0x0}},
                          {{0xD6E6D2C, 0xE50FC370, 0x6D9864CF, 0x80170000}},
                          {{0xD1554DA9, 0x15E89EB9, 0x8FBAA09C, 0x70000}},
                          {{0x8B0AFAD0, 0x928F97BF, 0x3E2DDDC7, 0x80020000}},
                          {{0xA05815EB, 0x11724DD9, 0xCDE5F479, 0x80100000}},
                          {{0x6416CDF7, 0x9B0AB7C9, 0x9002FAC3, 0x80110000}},
                          {{0x3B9AC9FF, 0x0, 0x0, 0x90000}},
                          {{0x3B9AC9FF, 0x0, 0x0, 0x80090000}},
                          {{0xA763FFFF, 0xDE0B6B3, 0x0, 0x80090000}},
                          {{0xF91EAFF2, 0x6D795225, 0x3FD35EB, 0x800E0000}},
                          {{0x63, 0x0, 0x0, 0x80000000}},
                          {{0xE9C6C9AC, 0x7F868D8F, 0xE41050CA, 0x80010000}}};
  s21_decimal expected[] = {{{0x30FA475E, 0xA65A415B, 0x16CE6E7A, 0x0}},
                            {{0x0, 0x0, 0x0, 0x0}},
                            {{0x0, 0x0, 0x0, 0x80000000}},
                            {{0x0, 0x0, 0xFFFED, 0x0}},
                            {{0x52CED, 0x0, 0x0, 0x80000000}},
                            {{0x7A83B4DD, 0x23242095, 0xF1, 0x0}},
                            {{0xD5DED46E, 0xE02F8413, 0x9F2DBC, 0x80000000}},
                            {{0xA740F380, 0x5CB, 0x0, 0x80000000}},
                            {{0xC5755D62, 0x67, 0x0, 0x80000000}},
                            {{0x1, 0x0, 0x0, 0x0}},
                            {{0x1, 0x0, 0x0, 0x80000000}},
                            {{0x3B9ACA00, 0x0, 0x0, 0x80000000}},
                            {{0x73CE2FF2, 0xB3A, 0x0, 0x80000000}},
                            {{0x63, 0x0, 0x0, 0x80000000}},
                            {{0x30FA475E, 0xA65A415B, 0x16CE6E7A, 0x80000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_round(values[i], &res);
    test_other_ok(res, expected[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_truncate_error) {
  printf(
      "\n\x1b[34mTesting s21_truncate (error: incorrect values):\x1b[0m\n\n");
  s21_decimal values[] = {{{7, 0, 0, 0x1D0000}},
                          {{0xAF67, 0, 0, 0x11C0000}},
                          {{0xFFFFFFFF, 0x45FED8, 0xFFEE, 0xFFFFFFFF}},
                          {{0xFFEE, 0xFEABC9, 0xEFEF, 0x1C0001}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1404600}},
                          {{0, 0, 0, 0x1D0000}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                          {{432959, 3252345, 3124214, 0x401C0000}},
                          {{0, 0, 0, 100000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_truncate(values[i], &res);
    test_other_fail(code, i + 1);
  }
  s21_decimal example = {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x40000}};
  code = s21_truncate(example, NULL);
  test_other_fail(code, (int)(sizeof(values) / sizeof(s21_decimal)) + 1);
}
END_TEST

START_TEST(test_s21_truncate_ok1) {
  printf("\n\x1b[34mTesting s21_truncate (ok: correct values):\x1b[0m\n\n");
  s21_decimal values[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x30000}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80080000}},
                          {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x1C0000}},
                          {{0x0, 0xAAAAAAAA, 0x0, 0x80000000}},
                          {{0x0, 0x0, 0x1, 0x800B0000}},
                          {{0x0, 0x0, 0x1, 0x80130000}},
                          {{0x1, 0x0, 0x0, 0xD0000}},
                          {{0x0, 0x0, 0x0, 0x1B0000}},
                          {{0x80000000, 0x80000000, 0x80000000, 0x10000}},
                          {{0x80000000, 0x80000000, 0x80000000, 0x801C0000}},
                          {{0x0, 0x811800, 0x0, 0x80010000}},
                          {{0x540BE3FF, 0x2, 0x0, 0x50000}},
                          {{0x0, 0x0, 0x0, 0x800E0000}},
                          {{0x0, 0x0, 0x0, 0xE0000}},
                          {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x10000}}};
  s21_decimal expected[] = {{{0x9DB22D0E, 0x4BC6A7EF, 0x418937, 0x0}},
                            {{0x1873BF3F, 0xF31DC461, 0x2A, 0x80000000}},
                            {{0x5, 0x0, 0x0, 0x0}},
                            {{0x0, 0xAAAAAAAA, 0x0, 0x80000000}},
                            {{0xAFEBFF0, 0x0, 0x0, 0x80000000}},
                            {{0x1, 0x0, 0x0, 0x80000000}},
                            {{0x0, 0x0, 0x0, 0x0}},
                            {{0x0, 0x0, 0x0, 0x0}},
                            {{0xA6666666, 0xD9999999, 0xCCCCCCC, 0x0}},
                            {{0x3, 0x0, 0x0, 0x80000000}},
                            {{0xCCCCCCCC, 0xCE8CC, 0x0, 0x80000000}},
                            {{0x1869F, 0x0, 0x0, 0x0}},
                            {{0x0, 0x0, 0x0, 0x80000000}},
                            {{0x0, 0x0, 0x0, 0x0}},
                            {{0x0, 0x11111111, 0x11111111, 0x0}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_truncate(values[i], &res);
    test_other_ok(res, expected[i], code, i + 1);
  }
}
END_TEST
START_TEST(test_s21_truncate_ok2) {
  printf("\n\x1b[34mTesting s21_truncate (ok: correct values):\x1b[0m\n\n");
  s21_decimal values[] = {{{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}},
                          {{0xFFFFFFF, 0x3E250261, 0x204FCE5E, 0x1C0000}},
                          {{0x98000001, 0x5D378391, 0x3077B58D, 0x1C0000}},
                          {{0x29BD0E86, 0x789AE890, 0xF37BF67A, 0x0}},
                          {{0x3E55BD95, 0xA7D45A16, 0x9EC9011, 0x30000}},
                          {{0xD8415F6E, 0xF0B28E5E, 0x85F6D222, 0x80150000}},
                          {{0x21D50F21, 0x99AB7C96, 0xBAC78E9E, 0x801C0000}},
                          {{0x80AADE5, 0x194A5D29, 0x342C88FA, 0xC0000}},
                          {{0x5F6C656, 0xBE29140, 0x1B4EEC04, 0x150000}},
                          {{0xEBC01C17, 0x3C92064E, 0x6A3D4FC2, 0x1C0000}},
                          {{0x6ECAF8B9, 0xDBF8B808, 0xC5FC3D61, 0x80110000}},
                          {{0xCA5A0248, 0xB388AE53, 0x2A3FA330, 0x60000}},
                          {{0x80FB6C46, 0x95AED177, 0xA2C1DED9, 0x80180000}},
                          {{0x88F5E715, 0x2CDFC557, 0x63F011D0, 0x800E0000}},
                          {{0x3B9AC9FF, 0x0, 0x0, 0x90000}}};
  s21_decimal expected[] = {{{0x0, 0x0, 0x0, 0x0}},
                            {{0x0, 0x0, 0x0, 0x0}},
                            {{0x1, 0x0, 0x0, 0x0}},
                            {{0x29BD0E86, 0x789AE890, 0xF37BF67A, 0x0}},
                            {{0xC8411C17, 0x52580535, 0x28A62, 0x0}},
                            {{0x278A0B6, 0x0, 0x0, 0x80000000}},
                            {{0x5, 0x0, 0x0, 0x80000000}},
                            {{0x658DCE27, 0x395DAA, 0x0, 0x0}},
                            {{0x80F5B2, 0x0, 0x0, 0x0}},
                            {{0x3, 0x0, 0x0, 0x0}},
                            {{0xA9D7FF34, 0x8E, 0x0, 0x80000000}},
                            {{0x94DEBE3D, 0xD0492F23, 0x2C4, 0x0}},
                            {{0xC4C2, 0x0, 0x0, 0x80000000}},
                            {{0xC0FF91F7, 0x1194C, 0x0, 0x80000000}},
                            {{0x0, 0x0, 0x0, 0x0}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_truncate(values[i], &res);
    test_other_ok(res, expected[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_add_fail) {
  printf("\n\x1b[34mTesting s21_add (incorrect values):\x1b[0m\n");
  s21_decimal input_1[] = {{{0, 0, 0, 1000000000}},
                           {{0, 0, 0, 0x1D0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{-1, 0, 0, 0x11C0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0x10000000, 0x3E250261, 0x204FCE5E, 0x1C8000}}};
  s21_decimal input_2[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{0x1, 0x0, 0x0, 0x80000000}}};
  s21_decimal res;
  // = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_add(input_1[i], input_2[i], &res);
    test_arith_fail(code, i + 1);
  }
  code = s21_add(input_1[1], input_2[1], NULL);
  test_arith_fail(code, 7);
}
END_TEST

START_TEST(test_s21_decimal_add_big) {
  printf("\n\x1b[34mTesting s21_add (big overflow):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
                           {{0xFFFFFFFA, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
                           {{0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
                           {{0xFFFFFFFF, 0xFE, 0x0, 0x0}}};
  s21_decimal input_2[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0x99999999, 0x99999999, 0x19999999, 0x0}},
                           {{0x1, 0x0, 0x0, 0x0}},
                           {{2, 0x0, 0x0, 0x0}},
                           {{346778, 0x0, 0x0, 0x00040000}},
                           {{0xFFFFFFFF, 0xFF, 0x0, 0x00020000}},
                           {{0xFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_add(input_1[i], input_2[i], &res);
    test_arith_big(code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_add_small) {
  printf("\n\x1b[34mTesting s21_add (small overflow):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFA, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFF, 0xFE, 0x0, 0x80000000}}};
  s21_decimal input_2[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0x99999999, 0x99999999, 0x19999999, 0x80000000}},
                           {{0x1, 0x0, 0x0, 0x80000000}},
                           {{2, 0x0, 0x0, 0x80000000}},
                           {{346778, 0x0, 0x0, 0x80040000}},
                           {{0xFFFFFFFF, 0xFF, 0x0, 0x80020000}},
                           {{0xFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_add(input_1[i], input_2[i], &res);
    test_arith_small(code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_add_ok1) {
  printf("\n\x1b[34mTesting s21_add (correct values):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0x122233EE, 0x5675EBE6, 0xD1C83484, 0x140000}},
                           {{0x8, 0x0, 0x0, 0x80000000}},
                           {{0x811800, 0x811800, 0x0, 0x100000}},
                           {{0x0, 0x0, 0x0, 0x801C0000}},
                           {{0x0, 0x0, 0x0, 0x801C0000}},
                           {{0x0, 0x0, 0x0, 0x80070000}},
                           {{0x122233EE, 0x5675EBE6, 0xD1C83484, 0x140000}},
                           {{0x122233EE, 0x5675EBE6, 0xD1C83484, 0x140000}},
                           {{0x7F7B8E5F, 0x2B4271A6, 0xA09A2FB4, 0x80120000}},
                           {{0x0, 0x0, 0x0, 0x00030000}},
                           {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}};
  s21_decimal input_2[] = {{{0x88000001, 0x1F128130, 0x1027E72F, 0x801C0000}},
                           {{0xF555887, 0x553F1641, 0xB5597F0B, 0x60000}},
                           {{0x55555555, 0x55555555, 0x0, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}},
                           {{0x9999999B, 0x99999999, 0x19999999, 0x80000000}},
                           {{0x5, 0x0, 0x0, 0x80010000}},
                           {{0x0, 0x0, 0x0, 0x80040000}},
                           {{0x811800, 0x811800, 0x0, 0x100000}},
                           {{0x55555555, 0x0, 0x0, 0x0}},
                           {{0x122233EE, 0x5675EBE6, 0xD1C83484, 0x80140000}},
                           {{0x0, 0x0, 0x0, 0x80090000}},
                           {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x1C0000}}};
  s21_decimal expect[] = {{{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                          {{0xF5752326, 0x554164BC, 0xB5597F0B, 0x60000}},
                          {{0x5555554D, 0x55555555, 0x0, 0x0}},
                          {{0xFFFFFFDB, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}},
                          {{0x9999999B, 0x99999999, 0x19999999, 0x80000000}},
                          {{0x88000000, 0x1F128130, 0x1027E72F, 0x801C0000}},
                          {{0x0, 0x0, 0x0, 0x0}},
                          {{0xC4DBB3EE, 0x92F6BF9, 0xD1C83498, 0x140000}},
                          {{0x6D71D1FE, 0x2A7D2584, 0x433CCCA8, 0x130000}},
                          {{0xC2395692, 0xD677691, 0xA2B33A78, 0x80120000}},
                          {{0x0, 0x0, 0x0, 0x0}},
                          {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}};
  s21_decimal res;
  // = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_add(input_1[i], input_2[i], &res);
    test_arith_ok(res, expect[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_negate_error) {
  printf(
      "\n\x1b[34mTesting s21_truncate (error: incorrect values):\x1b[0m\n\n");
  s21_decimal values[] = {{{7, 0, 0, 0x1D0000}},
                          {{0xAF67, 0, 0, 0x11C0000}},
                          {{0xFFFFFFFF, 0x45FED8, 0xFFEE, 0xFFFFFFFF}},
                          {{0xFFEE, 0xFEABC9, 0xEFEF, 0x1C0001}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1404600}},
                          {{0, 0, 0, 0x1D0000}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                          {{432959, 3252345, 3124214, 0x401C0000}},
                          {{0, 0, 0, 100000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_negate(values[i], &res);
    test_other_fail(code, i + 1);
  }
  s21_decimal example = {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x40000}};
  code = s21_negate(example, NULL);
  test_other_fail(code, (int)(sizeof(values) / sizeof(s21_decimal)) + 1);
}
END_TEST

START_TEST(test_s21_negate_ok1) {
  printf("\n\x1b[34mTesting s21_negate (ok: correct values):\x1b[0m\n\n");
  s21_decimal values[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x30000}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80080000}},
                          {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x1C0000}},
                          {{0x0, 0xAAAAAAAA, 0x0, 0x80000000}},
                          {{0x0, 0x0, 0x1, 0x800B0000}},
                          {{0x0, 0x0, 0x1, 0x80130000}},
                          {{0x1, 0x0, 0x0, 0xD0000}},
                          {{0x0, 0x0, 0x0, 0x1B0000}},
                          {{0x80000000, 0x80000000, 0x80000000, 0x10000}},
                          {{0x80000000, 0x80000000, 0x80000000, 0x801C0000}},
                          {{0x0, 0x811800, 0x0, 0x80010000}},
                          {{0x540BE3FF, 0x2, 0x0, 0x50000}},
                          {{0x0, 0x0, 0x0, 0x800E0000}},
                          {{0x0, 0x0, 0x0, 0xE0000}},
                          {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x10000}}};
  s21_decimal expected[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80030000}},
                            {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000}},
                            {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x801C0000}},
                            {{0x0, 0xAAAAAAAA, 0x0, 0x0}},
                            {{0x0, 0x0, 0x1, 0xB0000}},
                            {{0x0, 0x0, 0x1, 0x130000}},
                            {{0x1, 0x0, 0x0, 0x800D0000}},
                            {{0x0, 0x0, 0x0, 0x801B0000}},
                            {{0x80000000, 0x80000000, 0x80000000, 0x80010000}},
                            {{0x80000000, 0x80000000, 0x80000000, 0x1C0000}},
                            {{0x0, 0x811800, 0x0, 0x10000}},
                            {{0x540BE3FF, 0x2, 0x0, 0x80050000}},
                            {{0x0, 0x0, 0x0, 0xE0000}},
                            {{0x0, 0x0, 0x0, 0x800E0000}},
                            {{0x0, 0xAAAAAAAA, 0xAAAAAAAA, 0x80010000}}};
  ;
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_negate(values[i], &res);
    test_other_ok(res, expected[i], code, i + 1);
  }
}
END_TEST
START_TEST(test_s21_decimal_add_ok2) {
  printf("\n\x1b[34mTesting s21_add (correct values):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0x1FFFFC18, 0x7C4A04C2, 0x409F9CBC, 0x1C0000}},
                           {{0x10000001, 0x3E250261, 0x204FCE5E, 0x1C0000}},
                           {{0x1B755555, 0x2CE411B0, 0x55555560, 0x150000}},
                           {{0x1B755555, 0x2CE411B0, 0x55555560, 0x80150000}},
                           {{0x8, 0x0, 0x0, 0x80000000}},
                           {{0x811800, 0x811800, 0x0, 0x100000}},
                           {{0xF555887, 0x553F1641, 0xB5597F0B, 0x80060000}},
                           {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x801C0000}},
                           {{0x0, 0x0, 0x0, 0x801C0000}},
                           {{0x55555555, 0x55555555, 0x55555555, 0x80000000}},
                           {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}};
  s21_decimal input_2[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0x0, 0x0, 0x0, 0x1C0000}},
                           {{0x192F9AB, 0x0, 0x0, 0x80000000}},
                           {{0x99999999, 0x99999999, 0x19999999, 0x0}},
                           {{0x10000001, 0x3E250261, 0x204FCE5E, 0x1C0000}},
                           {{0x9999999B, 0x99999999, 0x19999999, 0x80000000}},
                           {{0x99999999, 0x99999999, 0x19999999, 0x80000000}},
                           {{0x99999999, 0x99999999, 0x19999999, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}},
                           {{0x0, 0x0, 0x0, 0x001C0000}},
                           {{0x55555555, 0x0, 0x55555555, 0x80150000}},
                           {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x1C0000}}};
  s21_decimal expect[] = {{{0x0, 0x0, 0x0, 0x0}},
                          {{0x1FFFFC18, 0x7C4A04C2, 0x409F9CBC, 0x1C0000}},
                          {{0x76400000, 0xC2AA722F, 0x55555503, 0x80150000}},
                          {{0x9B2C9345, 0x99999999, 0x19999999, 0x0}},
                          {{0x3CD55555, 0xF71A63EA, 0x55555529, 0x80150000}},
                          {{0x999999A3, 0x99999999, 0x19999999, 0x80000000}},
                          {{0xFFFFFFD6, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}},
                          {{0x4BB0FD62, 0x9636572A, 0xFFFF8926, 0x10000}},
                          {{0x78000000, 0xE0ED7ECF, 0xEFD818D0, 0x1C0000}},
                          {{0x0, 0x0, 0x0, 0x0}},
                          {{0x56E84F00, 0x55555555, 0x55555555, 0x80000000}},
                          {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_add(input_1[i], input_2[i], &res);
    test_arith_ok(res, expect[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_negate_ok2) {
  printf("\n\x1b[34mTesting s21_negate (ok: correct values):\x1b[0m\n\n");
  s21_decimal values[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xA0000}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x801C0000}},
                          {{0x0, 0x80000000, 0x0, 0x10000}},
                          {{0x811800, 0x811800, 0x811800, 0x80140000}},
                          {{0x0, 0x811800, 0x811800, 0x80000000}},
                          {{0x811800, 0x0, 0x811800, 0x10000}},
                          {{0x0, 0x0, 0x811800, 0x801A0000}},
                          {{0x449AEF8D, 0x886E6706, 0x1B1B4C40, 0x130000}},
                          {{0xB8CF61F3, 0xA4D11E22, 0xC7C50C60, 0x80040000}},
                          {{0x224AB663, 0x289551F5, 0xFF45652, 0x80060000}},
                          {{0x61E7D194, 0xA162A112, 0x997E4C83, 0x80010000}},
                          {{0xDED2AF13, 0x18A058D4, 0xBDCDC1F3, 0x1C0000}},
                          {{0x501ED51F, 0xC4084E21, 0x6B5BA353, 0x80030000}},
                          {{0x7055400E, 0xF7463C3D, 0x619A609E, 0x1A0000}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80110000}}};
  s21_decimal expected[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x800A0000}},
                            {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}},
                            {{0x0, 0x80000000, 0x0, 0x80010000}},
                            {{0x811800, 0x811800, 0x811800, 0x140000}},
                            {{0x0, 0x811800, 0x811800, 0x0}},
                            {{0x811800, 0x0, 0x811800, 0x80010000}},
                            {{0x0, 0x0, 0x811800, 0x1A0000}},
                            {{0x449AEF8D, 0x886E6706, 0x1B1B4C40, 0x80130000}},
                            {{0xB8CF61F3, 0xA4D11E22, 0xC7C50C60, 0x40000}},
                            {{0x224AB663, 0x289551F5, 0xFF45652, 0x60000}},
                            {{0x61E7D194, 0xA162A112, 0x997E4C83, 0x10000}},
                            {{0xDED2AF13, 0x18A058D4, 0xBDCDC1F3, 0x801C0000}},
                            {{0x501ED51F, 0xC4084E21, 0x6B5BA353, 0x30000}},
                            {{0x7055400E, 0xF7463C3D, 0x619A609E, 0x801A0000}},
                            {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x110000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(values) / sizeof(s21_decimal)); i++) {
    code = s21_negate(values[i], &res);
    test_other_ok(res, expected[i], code, i + 1);
  }
}
END_TEST
START_TEST(test_s21_decimal_mul_fail) {
  printf("\n\x1b[34mTesting s21_mul (incorrect values):\x1b[0m\n");
  s21_decimal input_1[] = {{{0, 0, 0, 1000000000}},
                           {{0, 0, 0, 0x1D0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{-1, 0, 0, 0x11C0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0x10000000, 0x3E250261, 0x204FCE5E, 0x1C8000}}};
  s21_decimal input_2[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{0x1, 0x0, 0x0, 0x80000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_mul(input_1[i], input_2[i], &res);
    test_arith_fail(code, i + 1);
  }
  code = s21_mul(input_1[1], input_2[1], NULL);
  test_arith_fail(code, 7);
}
END_TEST

START_TEST(test_s21_decimal_mul_big) {
  printf("\n\x1b[34mTesting s21_mul (big overflow):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}},
                           {{0x55555555, 0x55555555, 0x55555555, 0x0}},
                           {{0x55555555, 0x55555555, 0x55555555, 0x80010000}},
                           {{0x4F4FB9DA, 0xC076B0FB, 0x8163398E, 0xE0000}},
                           {{0x8, 0x0, 0x0, 0x80000000}},
                           {{0xD42B4895, 0x339BF28D, 0xB9B77ADA, 0x90000}},
                           {{0x55555555, 0x0, 0x55555555, 0x0}}};

  s21_decimal input_2[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0x10000001, 0x3E250261, 0x204FCE5E, 0x1B0000}},
                           {{0x55555555, 0x55555555, 0x55555555, 0x0}},
                           {{0x55555555, 0x0, 0x55555555, 0x80150000}},
                           {{0x4F4FB9EF, 0xC076B0FB, 0x8163398E, 0xE0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0x55555555, 0x55555555, 0x0, 0x0}},
                           {{0x8, 0x0, 0x0, 0x0}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_mul(input_1[i], input_2[i], &res);
    test_arith_big(code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_mul_small) {
  printf("\n\x1b[34mTesting s21_mul (small overflow):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFF8, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}},
                           {{0x55555555, 0x55555555, 0x55555555, 0x0}},
                           {{0x55555555, 0x55555555, 0x55555555, 0x80010000}},
                           {{0x4F4FB9DA, 0xC076B0FB, 0x8163398E, 0xE0000}},
                           {{0x8, 0x0, 0x0, 0x80000000}},
                           {{0xD42B4895, 0x339BF28D, 0xB9B77ADA, 0x90000}},
                           {{0x55555555, 0x0, 0x55555555, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xE0000}}};

  s21_decimal input_2[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0x10000001, 0x3E250261, 0x204FCE5E, 0x801B0000}},
                           {{0x55555555, 0x55555555, 0x55555555, 0x80000000}},
                           {{0x55555555, 0x0, 0x55555555, 0x00150000}},
                           {{0x4F4FB9EF, 0xC076B0FB, 0x8163398E, 0x800E0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0x55555555, 0x55555555, 0x0, 0x80000000}},
                           {{0x8, 0x0, 0x0, 0x80000000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_mul(input_1[i], input_2[i], &res);
    test_arith_small(code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_mul_ok1) {
  printf("\n\x1b[34mTesting s21_mul (correct values_1):\x1b[0m\n");
  s21_decimal input_1[] = {{{0x69168DB5, 0xDCB4F4B1, 0x16CC701F, 0x80180000}},
                           {{1, 0, 0, 0x1C0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0x1, 0}},
                           {{0x192F9AB, 0x0, 0x0, 0x0}},
                           {{0x192F9AB, 0x0, 0x0, 0x80000000}},
                           {{0x1, 0x0, 0x0, 0x0}},
                           {{0x1FFFFC18, 0x7C4A04C2, 0x409F9CBC, 0x801C0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x801C0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x1C0000}},
                           {{0x19, 0x0, 0x0, 0x100000}}};
  s21_decimal input_2[] = {{{0x5, 0x0, 0x0, 0x10000}},
                           {{10, 0, 0, 0}},
                           {{2, 0, 0, 0}},
                           {{0x8, 0x0, 0x0, 0x80000000}},
                           {{0x55555555, 0x55555555, 0x0, 0x0}},
                           {{0x69168DB5, 0xDCB4F4B1, 0x16CC701F, 0x80180000}},
                           {{0x5, 0x0, 0x0, 0x10000}},
                           {{0x55555555, 0x0, 0x0, 0x80000000}},
                           {{0x19, 0x0, 0x0, 0x100000}},
                           {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}},
                           {{0x0, 0x55555555, 0x0, 0x80000000}}};
  s21_decimal expect[] = {{{0xD70C489, 0x4F88C777, 0x71FE309F, 0x80190000}},
                          {{0xA, 0, 0, 0x1C0000}},
                          {{0xFFFFFFFE, 0xFFFFFFFF, 0x3, 0}},
                          {{0xC97CD58, 0x0, 0x0, 0x80000000}},
                          {{0xFF79ACC7, 0xFFFFFFFF, 0x865338, 0x80000000}},
                          {{0x69168DB5, 0xDCB4F4B1, 0x16CC701F, 0x80180000}},
                          {{0xFFFFE0C, 0x3E250261, 0x204FCE5E, 0x801C0000}},
                          {{0xA940E7B8, 0xFB0123D2, 0x24A67FCC, 0x120000}},
                          {{0x0, 0x0, 0x40000000, 0xE0000}},
                          {{0x7, 0x0, 0x80000000, 0x1C0000}},
                          {{0x0, 0x5555554D, 0x8, 0x80100000}}};
  s21_decimal res;
  // = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_mul(input_1[i], input_2[i], &res);
    test_arith_ok(res, expect[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_mul_ok2) {
  printf("\n\x1b[34mTesting s21_mul (correct values_2):\x1b[0m\n");
  s21_decimal input_1[] = {{{0x0, 0x0, 0x0, 0x0}},
                           {{0x0, 0x0, 0x0, 0x801C0000}},
                           {{0x4F4FB9DA, 0xC076B0FB, 0x8163398E, 0xE0000}},
                           {{0x4F4FB9DA, 0xC076B0FB, 0x8163398E, 0xE0000}},
                           {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}},
                           {{0x816D8019, 0x18DE76, 0x0, 0x800F0000}},
                           {{0x5, 0x0, 0x0, 0x100000}}};
  s21_decimal input_2[] = {{{0x55555555, 0x0, 0x0, 0x0}},
                           {{0xA, 0x0, 0x0, 0x801C0000}},
                           {{0x88000001, 0x1F128130, 0x1027E72F, 0x801C0000}},
                           {{0x69168DB5, 0xDCB4F4B1, 0x16CC701F, 0x180000}},
                           {{0x55555555, 0x55555555, 0x55555555, 0x0}},
                           {{0x5, 0x0, 0x0, 0x800D0000}},
                           {{0x55555555, 0x0, 0x55555555, 0x80000000}}};
  s21_decimal expect[] = {{{0x0, 0x0, 0x0, 0x0}},
                          {{0x0, 0x0, 0x0, 0x0}},
                          {{0xA7A7DCF1, 0x603B587D, 0x40B19CC7, 0x800E0000}},
                          {{0x109AC90D, 0xBE2CD8E0, 0x5B4B2733, 0xA0000}},
                          {{0xAAAAAAAD, 0xAAAAAAAA, 0x2AAAAAAA, 0x0}},
                          {{0x8723807D, 0x7C5850, 0x0, 0x1C0000}},
                          {{0x2AAAAAAA, 0x80000000, 0x2AAAAAAA, 0x800F0000}}};
  s21_decimal res;
  // = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_mul(input_1[i], input_2[i], &res);
    test_arith_ok(res, expect[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_div_fail) {
  printf("\n\x1b[34mTesting s21_div (incorrect values):\x1b[0m\n");
  s21_decimal input_1[] = {{{0, 0, 0, 1000000000}},
                           {{0, 0, 0, 0x1D0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{-1, 0, 0, 0x11C0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0x10000000, 0x3E250261, 0x204FCE5E, 0x1C8000}}};
  s21_decimal input_2[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{0x1, 0x0, 0x0, 0x80000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_div(input_1[i], input_2[i], &res);
    test_arith_fail(code, i + 1);
  }
  code = s21_div(input_1[1], input_2[1], NULL);
  test_arith_fail(code, 7);
}
END_TEST

START_TEST(test_s21_decimal_div_big) {
  printf("\n\x1b[34mTesting s21_div (big overflow):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x0}},
                           {{0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0x0, 0x1, 0x0, 0x0}},
                           {{0x80000000, 0x0, 0x0, 0x0}},
                           {{0x60000005, 0x74DE0E46, 0xC1DED635, 0x801B0000}},
                           {{0x80000000, 0x0, 0x80000000, 0x0}}};
  s21_decimal input_2[] = {
      {{0x8, 0x0, 0x0, 0x1C0000}},   {{0x13, 0x0, 0x0, 0x30000}},
      {{0x8, 0x0, 0x0, 0x801C0000}}, {{0xA, 0x0, 0x0, 0x1C0000}},
      {{0x8, 0x0, 0x0, 0x1C0000}},   {{0x1, 0x0, 0x0, 0x801C0000}},
      {{0xA, 0x0, 0x0, 0x1C0000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_div(input_1[i], input_2[i], &res);
    test_arith_big(code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_div_small) {
  printf("\n\x1b[34mTesting s21_div (small overflow):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x80000000}},
                           {{0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0x0, 0x1, 0x0, 0x80000000}},
                           {{0x80000000, 0x0, 0x0, 0x80000000}},
                           {{0x60000005, 0x74DE0E46, 0xC1DED635, 0x1B0000}},
                           {{0x80000000, 0x0, 0x80000000, 0x80000000}}};
  s21_decimal input_2[] = {
      {{0x8, 0x0, 0x0, 0x1C0000}},   {{0x13, 0x0, 0x0, 0x30000}},
      {{0x8, 0x0, 0x0, 0x801C0000}}, {{0xA, 0x0, 0x0, 0x1C0000}},
      {{0x8, 0x0, 0x0, 0x1C0000}},   {{0x1, 0x0, 0x0, 0x801C0000}},
      {{0xA, 0x0, 0x0, 0x1C0000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_div(input_1[i], input_2[i], &res);
    test_arith_small(code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_div_ok1) {
  printf("\n\x1b[34mTesting s21_div (correct values):\x1b[0m\n");
  s21_decimal input_1[] = {{{125, 0, 0, 0x0}},
                           {{0x0, 0x0, 0x80000000, 0x80000000}},
                           {{0x4E2, 0x0, 0x0, 0x0}},
                           {{0x2A, 0x0, 0x0, 0x20000}},
                           {{0x1, 0x0, 0x0, 0x0}},
                           {{0xA, 0x0, 0x0, 0x0}},
                           {{0x64, 0x0, 0x0, 0x80000000}},
                           {{0x0, 0x0, 0x80000000, 0x80000000}},
                           {{0x0, 0x0, 0x0, 0x0}},
                           {{0x0, 0x1, 0x0, 0x0}}};
  s21_decimal input_2[] = {
      {{0x64, 0x0, 0x0, 0x0}},    {{0x8, 0x0, 0x0, 0x0}},
      {{0x19, 0x0, 0x0, 0x0}},    {{0x2A, 0x0, 0x0, 0x20000}},
      {{0x1F4, 0x0, 0x0, 0x0}},   {{0x5, 0x0, 0x0, 0x0}},
      {{0x5, 0x0, 0x0, 0x10000}}, {{0xA, 0x0, 0x0, 0x0}},
      {{0x1E240, 0x0, 0x0, 0x0}}, {{0x1, 0x0, 0x0, 0x30000}}};
  s21_decimal expect[] = {
      {{0x7D, 0x0, 0x0, 0x20000}},    {{0x0, 0x0, 0x10000000, 0x80000000}},
      {{0x32, 0x0, 0x0, 0x0}},        {{0x1, 0x0, 0x0, 0x0}},
      {{0x2, 0x0, 0x0, 0x30000}},     {{0x2, 0x0, 0x0, 0x0}},
      {{0xC8, 0x0, 0x0, 0x80000000}}, {{0x0, 0x0, 0x80000000, 0x80010000}},
      {{0x0, 0x0, 0x0, 0x0}},         {{0x0, 0x3E8, 0x0, 0x0}}};
  s21_decimal res;
  // = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_div(input_1[i], input_2[i], &res);
    test_arith_ok(res, expect[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_div_ok2) {
  printf("\n\x1b[34mTesting s21_div (correct values):\x1b[0m\n");
  s21_decimal input_1[] = {{{0x5, 0x0, 0x0, 0x0}},
                           {{0x5, 0x0, 0x0, 0x80000000}},
                           {{0x5, 0x0, 0x0, 0x80010000}},
                           {{0xFFFFFFFF, 0x0, 0x0, 0x80000}},
                           {{0x1, 0x0, 0x0, 0x80030000}},
                           {{0x1, 0x0, 0x0, 0x80030000}},
                           {{0x811800, 0x0, 0x0, 0x30000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0000000}}};
  s21_decimal input_2[] = {
      {{0x2, 0x0, 0x0, 0x0}},        {{0x8, 0x0, 0x0, 0x801C0000}},
      {{0x13, 0x0, 0x0, 0x30000}},   {{0x5, 0x0, 0x0, 0x0}},
      {{0x0, 0x1, 0x0, 0x80000000}}, {{0xA, 0x0, 0x0, 0x801C0000}},
      {{0x13, 0x0, 0x0, 0x30000}},   {{1, 0x0, 0x0, 0x000000}}};
  s21_decimal expect[] = {{{0x19, 0x0, 0x0, 0x10000}},
                          {{0xAA000000, 0xE6D7217C, 0x1431E0FA, 0x0}},
                          {{0x5286BCA1, 0xE6E82135, 0x5507E92D, 0x80190000}},
                          {{0x33333333, 0x0, 0x0, 0x00100000}},
                          {{0xB8E38E3A, 0x373D5772, 0x1CB8B770, 0x0}},
                          {{0xA1000000, 0x1BCECCED, 0xD3C2, 0x0}},
                          {{0x286BCA1B, 0xB2BB3923, 0x8FE08F1A, 0x1B0000}},
                          {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}};
  s21_decimal res;
  // = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_div(input_1[i], input_2[i], &res);
    // print_decimal(res);
    // print_decimal(expect[i]);
    test_arith_ok(res, expect[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_div_divbyzero) {
  printf("\n\x1b[34mTesting s21_div (big overflow):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFF, 0x0, 0xFFFFFFFF, 0x0}},
                           {{0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0x0, 0x1, 0x0, 0x0}},
                           {{0x80000000, 0x0, 0x0, 0x0}},
                           {{0x60000005, 0x74DE0E46, 0xC1DED635, 0x801B0000}},
                           {{0x80000000, 0x0, 0x80000000, 0x0}}};
  s21_decimal input_2[] = {
      {{0x0, 0x0, 0x0, 0x80000000}}, {{0x0, 0x0, 0x0, 0x0}},
      {{0x0, 0x0, 0x0, 0x1C0000}},   {{0x0, 0x0, 0x0, 0x801C0000}},
      {{0x0, 0x0, 0x0, 0x100000}},   {{0x0, 0x0, 0x0, 0x801A0000}},
      {{0x0, 0x0, 0x0, 0x1B0000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_div(input_1[i], input_2[i], &res);
    test_arith_divbyzero(code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_sub_fail) {
  printf("\n\x1b[34mTesting s21_sub (incorrect values):\x1b[0m\n");
  s21_decimal input_1[] = {{{0, 0, 0, 1000000000}},
                           {{0, 0, 0, 0x1D0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{-1, 0, 0, 0x11C0000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0x10000000, 0x3E250261, 0x204FCE5E, 0x1C8000}}};
  s21_decimal input_2[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x140000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}},
                           {{0x1, 0x0, 0x0, 0x80000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_sub(input_1[i], input_2[i], &res);
    test_arith_fail(code, i + 1);
  }
  code = s21_sub(input_1[1], input_2[1], NULL);
  test_arith_fail(code, 7);
}
END_TEST

START_TEST(test_s21_decimal_sub_big) {
  printf("\n\x1b[34mTesting s21_sub (big overflow):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
                           {{0xFFFFFFFA, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
                           {{0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0}},
                           {{0xFFFFFFFF, 0xFE, 0x0, 0x0}}};
  s21_decimal input_2[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0x99999999, 0x99999999, 0x19999999, 0x80000000}},
                           {{0x1, 0x0, 0x0, 0x80000000}},
                           {{2, 0x0, 0x0, 0x80000000}},
                           {{346778, 0x0, 0x0, 0x80040000}},
                           {{0xFFFFFFFF, 0xFF, 0x0, 0x80020000}},
                           {{0xFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_sub(input_1[i], input_2[i], &res);
    test_arith_big(code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_sub_small) {
  printf("\n\x1b[34mTesting s21_sub (small overflow):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFA, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0x0, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFF, 0xFE, 0x0, 0x80000000}}};
  s21_decimal input_2[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}},
                           {{0x99999999, 0x99999999, 0x19999999, 0x0}},
                           {{0x1, 0x0, 0x0, 0x0}},
                           {{2, 0x0, 0x0, 0x0}},
                           {{346778, 0x0, 0x0, 0x40000}},
                           {{0xFFFFFFFF, 0xFF, 0x0, 0x20000}},
                           {{0xFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_sub(input_1[i], input_2[i], &res);
    test_arith_small(code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_sub_ok1) {
  printf("\n\x1b[34mTesting s21_sub (correct values):\x1b[0m\n");
  s21_decimal input_1[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0x5, 0x0, 0x0, 0x10000}},
                           {{0x5, 0x0, 0x0, 0x10000}},
                           {{0x55555555, 0x0, 0x55555555, 0x80000000}},
                           {{0x55555555, 0x55555555, 0x55555555, 0x150000}},
                           {{0x55555555, 0x0, 0x55555555, 0x150000}},
                           {{0x811800, 0x811800, 0x0, 0x80100000}},
                           {{0x811800, 0x811800, 0x0, 0x100000}},
                           {{0x0, 0x0, 0x0, 0x801C0000}},
                           {{0xB2C6F4A5, 0xA11CA39F, 0xD0D7B8CF, 0x1A0000}},
                           {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}};
  s21_decimal input_2[] = {{{0x0, 0x0, 0x0, 0x0}},
                           {{0x192F9AB, 0x0, 0x0, 0x0}},
                           {{0x8, 0x0, 0x0, 0x80000000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                           {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x10000}},
                           {{0xA, 0x0, 0x0, 0x801C0000}},
                           {{0xB2C6F4A5, 0xA11CA39F, 0xD0D7B8CF, 0x801A0000}},
                           {{0x811800, 0x811800, 0x0, 0x100000}},
                           {{0x0, 0x0, 0x0, 0x80000000}},
                           {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x801C0000}},
                           {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x1C0000}}};
  s21_decimal expect[] = {{{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80000000}},
                          {{0xFBDC0A9, 0x0, 0x0, 0x80010000}},
                          {{0x55, 0x0, 0x0, 0x10000}},
                          {{0xAAAAAAAA, 0xFFFFFFFF, 0xAAAAAAAA, 0x0}},
                          {{0xF0423F4C, 0xFFFFFFFF, 0xFFFFFFFF, 0x80010000}},
                          {{0x55555555, 0x0, 0x55555555, 0x150000}},
                          {{0xB166F4A5, 0x9E9011C0, 0xCFAB26F0, 0x1A0000}},
                          {{0x0, 0x0, 0x0, 0x0}},
                          {{0x0, 0x0, 0x0, 0x0}},
                          {{0x24C6F4A5, 0xF80AA09, 0xD10114B9, 0x1A0000}},
                          {{0xFFFFFFFE, 0xFFFFFFFF, 0xFFFFFFFF, 0x0}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_sub(input_1[i], input_2[i], &res);
    test_arith_ok(res, expect[i], code, i + 1);
  }
}
END_TEST

START_TEST(test_s21_decimal_sub_ok2) {
  printf("\n\x1b[34mTesting s21_add (correct values):\x1b[0m\n");
  s21_decimal input_1[] = {{{0x1, 0x0, 0x0, 0x80000000}},
                           {{0x55555555, 0x55555555, 0x0, 0x80000000}},
                           {{0x55555555, 0x0, 0x55555555, 0x80150000}},
                           {{0x811800, 0x811800, 0x0, 0x100000}},
                           {{0x55555555, 0x55555555, 0x55555555, 0x80000000}},
                           {{0x0, 0x0, 0x0, 0x801C0000}},
                           {{0x87FFFFFF, 0x1F128130, 0x1027E72F, 0x1C0000}},
                           {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}},
                           {{0x55555555, 0x55555555, 0x0, 0x80000000}},
                           {{0x1FFFFC18, 0x7C4A04C2, 0x409F9CBC, 0x801C0000}}};
  s21_decimal input_2[] = {{{0x1FFFFC18, 0x7C4A04C2, 0x409F9CBC, 0x1C0000}},
                           {{0x9999999A, 0x99999999, 0x19999999, 0x80000000}},
                           {{0x55555555, 0x0, 0x55555555, 0x150000}},
                           {{0x1, 0x0, 0x0, 0x801C0000}},
                           {{0x0, 0x0, 0x0, 0x80000000}},
                           {{0x8, 0x0, 0x0, 0x0}},
                           {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}},
                           {{0x0, 0x0, 0x0, 0x1C0000}},
                           {{0xB2C6F4A5, 0xA11CA39F, 0xD0D7B8CF, 0x801A0000}},
                           {{0x55555555, 0x55555555, 0x55555555, 0x0}}};
  s21_decimal expect[] = {{{0x2FFFFC18, 0xBA6F0723, 0x60EF6B1A, 0x801C0000}},
                          {{0x44444445, 0x44444444, 0x19999999, 0x0}},
                          {{0xAAAAAAAA, 0x0, 0xAAAAAAAA, 0x80150000}},
                          {{0x89800001, 0xFEE8FB1C, 0x7568FB1C, 0x1C0000}},
                          {{0x55555555, 0x55555555, 0x55555555, 0x80000000}},
                          {{0x40000000, 0xFE8401E7, 0x19D971E4, 0x801B0000}},
                          {{0x2, 0x0, 0x0, 0x801C0000}},
                          {{0x88000001, 0x1F128130, 0x1027E72F, 0x1C0000}},
                          {{0xAFEF286A, 0x55554F73, 0xC6AEA155, 0x000A0000}},
                          {{0x55555557, 0x55555555, 0x55555555, 0x80000000}}};
  s21_decimal res = {0};
  int code = 0;
  for (int i = 0; i < (int)(sizeof(input_1) / sizeof(s21_decimal)); i++) {
    code = s21_sub(input_1[i], input_2[i], &res);
    test_arith_ok(res, expect[i], code, i + 1);
  }
}
END_TEST

Suite *s21_decimal_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("test_s21_decimal");
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_decimal_add_fail);
  tcase_add_test(tc_core, test_s21_decimal_add_big);
  tcase_add_test(tc_core, test_s21_decimal_add_small);
  tcase_add_test(tc_core, test_s21_decimal_add_ok1);
  tcase_add_test(tc_core, test_s21_decimal_add_ok2);

  tcase_add_test(tc_core, test_s21_decimal_is_greater);
  tcase_add_test(tc_core, test_s21_decimal_is_greater_or_equal);
  tcase_add_test(tc_core, test_s21_decimal_is_equal);
  tcase_add_test(tc_core, test_s21_decimal_is_not_equal);
  tcase_add_test(tc_core, test_s21_decimal_is_less);
  tcase_add_test(tc_core, test_s21_decimal_is_less_or_equal);

  tcase_add_test(tc_core, test_from_int_to_decimal_fail);
  tcase_add_test(tc_core, test_from_int_to_decimal_ok);
  tcase_add_test(tc_core, test_from_decimal_to_int_fail);
  tcase_add_test(tc_core, test_from_decimal_to_int_ok);
  tcase_add_test(tc_core, test_from_float_to_decimal_fail);
  tcase_add_test(tc_core, test_from_float_to_decimal_ok);
  tcase_add_test(tc_core, test_from_decimal_to_float_fail);
  tcase_add_test(tc_core, test_from_decimal_to_float_ok);

  tcase_add_test(tc_core, test_s21_floor_error);
  tcase_add_test(tc_core, test_s21_floor_ok1);
  tcase_add_test(tc_core, test_s21_floor_ok2);

  tcase_add_test(tc_core, test_s21_round_error);
  tcase_add_test(tc_core, test_s21_round_ok1);
  tcase_add_test(tc_core, test_s21_round_ok2);

  tcase_add_test(tc_core, test_s21_truncate_error);
  tcase_add_test(tc_core, test_s21_truncate_ok1);
  tcase_add_test(tc_core, test_s21_truncate_ok2);

  tcase_add_test(tc_core, test_s21_negate_error);
  tcase_add_test(tc_core, test_s21_negate_ok1);
  tcase_add_test(tc_core, test_s21_negate_ok2);

  tcase_add_test(tc_core, test_s21_decimal_mul_fail);
  tcase_add_test(tc_core, test_s21_decimal_mul_big);
  tcase_add_test(tc_core, test_s21_decimal_mul_small);
  tcase_add_test(tc_core, test_s21_decimal_mul_ok1);
  tcase_add_test(tc_core, test_s21_decimal_mul_ok2);

  tcase_add_test(tc_core, test_s21_decimal_div_fail);
  tcase_add_test(tc_core, test_s21_decimal_div_big);
  tcase_add_test(tc_core, test_s21_decimal_div_small);
  tcase_add_test(tc_core, test_s21_decimal_div_ok1);
  tcase_add_test(tc_core, test_s21_decimal_div_ok2);
  tcase_add_test(tc_core, test_s21_decimal_div_divbyzero);

  tcase_add_test(tc_core, test_s21_decimal_sub_fail);
  tcase_add_test(tc_core, test_s21_decimal_sub_big);
  tcase_add_test(tc_core, test_s21_decimal_sub_small);
  tcase_add_test(tc_core, test_s21_decimal_sub_ok1);
  tcase_add_test(tc_core, test_s21_decimal_sub_ok2);

  suite_add_tcase(s, tc_core);
  return s;
}

int main() {
  Suite *s = s21_decimal_suite();
  SRunner *runner = srunner_create(s);
  srunner_run_all(runner, CK_NORMAL);
  srunner_free(runner);
  return 0;
}
