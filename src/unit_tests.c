/*!
 * \mainpage
 * \author sageaure
 * \version 1.0
 * \section intro_sec Calculator_v1.0 was done with
 * 1. C language \n
 * 2. Qt Creator, Qt
 * 3. Qcustomplot
 * 4. Qmake6
 * 5. Doxygen
 *
* \file
* \brief Набор unit тестов

Комплекс тестов для проверки корркетной работы программы
*/

#include <check.h>
#include <math.h>
#include <stdlib.h>

#include "lib/s21_creditcal.h"
#include "lib/s21_datatypes.h"
#include "lib/s21_lexeme_parser.h"
#include "lib/s21_polish.h"
#include "lib/s21_validate.h"

START_TEST(test_sum) {
  char *input = "2 + 3 + 0.0 + 5 + 4.3";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, 2 + 3 + 0.0 + 5 + 4.3);
}
END_TEST

START_TEST(test_min) {
  char *input = "2 - 3 - 5 - 4.3";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, 2 - 3 - 5 - 4.3);
}
END_TEST

START_TEST(test_mul) {
  char *input = "2 * 3 * 5 * 4.3";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, 2 * 3 * 5 * 4.3);
}
END_TEST

START_TEST(test_div) {
  char *input = "2 / 3 / 5 / 4.3";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, (double)2 / (double)3 / (double)5 / 4.3);
}
END_TEST

START_TEST(test_pow) {
  char *input = "2  ^ 4.3";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, pow(2, 4.3));
}
END_TEST

START_TEST(test_sqrt) {
  char *input = "sqrt(4.5)";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, sqrt(4.5));
}
END_TEST

START_TEST(test_mod) {
  char *input = "43 mod  4.3";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, fmod(43, 4.3));
}
END_TEST

START_TEST(test_sin) {
  char *input = "sin(21.21)";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, sin(21.21));
}
END_TEST

START_TEST(test_cos) {
  char *input = "cos(21.21)";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, cos(21.21));
}
END_TEST

START_TEST(test_tan) {
  char *input = "tan(21.21)";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_ldouble_eq_tol(result, tan(21.21), 1e-7);
}
END_TEST

START_TEST(test_acos) {
  char *input = "acos(1)";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, acos(1));
}
END_TEST

START_TEST(test_asin) {
  char *input = "asin(1)";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, asin(1));
}
END_TEST

START_TEST(test_atan) {
  char *input = "atan(21.21)";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, atan(21.21));
}
END_TEST

START_TEST(test_ln) {
  char *input = "ln(4.32)";
  stack *st = parse_all(input);
  //   char out[255] = {0};
  //   print_rstack(st, out);
  //   printf("%s", out);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, log(4.32));
}
END_TEST

START_TEST(test_log) {
  char *input = "log(4.32)";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_double_eq(result, log10(4.32));
}
END_TEST

START_TEST(test_polish) {
  char *input = "1/2+(2+3)/(sin(9-2)^2-6/7.4)";
  char out[255] = {0};
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  print_rstack(p, out);
  double result = strcmp(out, "1 2 / 2 3 + 9 2 - sin 2 ^ 6 7.4 / - / +");
  ck_assert_int_eq(result, 0);
  remove_stack(&p);
}
END_TEST

START_TEST(test_complex) {
  char *input =
      "15 / ( 7-(-1+1) )*3 - ( 2+(1+1) ) *15 "
      "/(7-(200+1))*3-(2+(1+1))*(15/(7-(1+1))*3-(2+(1+1))+15/"
      "(7-(1+1))*3-(2+(1+1)))";
  stack *st = parse_all(input);
  stack *p = to_polish(&st);
  double result = 0;
  calc_polish(p, &result);
  ck_assert_ldouble_eq_tol(result, -32.6435935199, 1e-7);
}
END_TEST

START_TEST(test_error_input) {
  char *arr[] = {
      ")5+7(", "(", "()", "()*()*()", "(()*())", "))", "((", "(()", "(()())()",
      "(()()()", "(1)(+)(2)", "((+x-)*(x))", "((1)(2)(3)", "(1)*((-)2)*(3)",
      "(1)(-x)(1)", "aoa", "x.", ".", ",", "--5", ".5.5.", "....", "..",
      "5.5.5", "5.5 5.5", "5   5", "log(x)ln(x)", "a(x)", "co sin", "co s(x)",
      "as(x)", "cos( x + 23 / 3 ) - 255 ln(x)", "cos( x + 23 / 3 ) - 255log(x)",
      "cos( x + 23 / 3 ) - log(x)255", "sin(*8)", "l(x)", "ll(x)",
      //  "",
      "(*5)", "(/5)", "(^5)", "(mod5)", "()", "(555+)", "(555-)", "(555*)",
      "(5555/)", "(555mod)", "(555()", "7(+3)", "*3+3", "())", "lod(33)",

      "(.)(.)", "\0"};

  int error = 0;
  for (int i = 0; strcmp(arr[i], "\0"); i++) {
    stack *st = parse_all(arr[i]);
    if (st == NULL)
      error = ERROR;
    else {
      error = s21_validate(st);
    }

    ck_assert_int_eq(error, ERROR);
    remove_stack(&st);
  }
}
END_TEST

START_TEST(test_validate_ok) {
  char *arr[] = {
      "-4 + 4", "(+5) + (-5)", "3 * (-2)", "4 / +2", "(-3) * 5",
      "2 + 3 * 4 - 5", "(3 + 5) * (2 - 1)", "10 / (5 - 3)", "-(4 * 3) + 2",
      "3 mod 2 - 1", "2 ^ -3", "(-5) * 2 + 3 ^ 2", "3 + 4 * (2 - 1)",
      "-1 * (6 / 2)", "8 / (3 mod 2)", "+4 - (3 + 2) * 5", "((3 + 2) * 5) / 2",
      "(3 ^ 2) * 4 + 6", "(-2 * 4) + 3 - 1", "4 * (2 + 3) - 6 / 2",

      "sin(30) + cos(60)", "tan(45) - 2", "sin(x) * 3", "4 / cos(90)",
      "3 ^ sin(45)", "(sin(30) + cos(45)) * 2", "2 mod 3", "4 * tan(45) + 6",
      "sin(x) mod 4", "(cos(60) + 3) * 5", "3 mod sin(30)", "sin(90) / 2",
      "5 + cos(45) * sin(30)", "2 * (sin(60) + cos(30))", "sin(x) mod cos(x)",
      "4 * sin(30) mod 2", "tan(45) + 3 ^ 2", "sin(30) * cos(45) / tan(60)",
      "(sin(30) + cos(45)) mod 4", "-sin(30) + 3",
      //
      "\0"};

  int error = 0;
  for (int i = 0; strcmp(arr[i], "\0"); i++) {
    stack *st = parse_all(arr[i]);
    if (st != NULL) {
      error = s21_validate(st);
      ck_assert_int_eq(error, OK);
      remove_stack(&st);
    }
  }
}
END_TEST

START_TEST(test_differential_payments) {
  double loanAmount = 10000;
  int loanTerm = 12;
  double interestRate = 12;
  double *payments =
      calculateDifferentialPayments(loanAmount, loanTerm, interestRate);

  ck_assert(payments != NULL);
  for (int i = 0; i < loanTerm; ++i) {
    ck_assert(payments[i] > 0);
  }
  free(payments);
}
END_TEST

START_TEST(test_annuity_payment) {
  double loanAmount = 10000;
  int loanTerm = 12;
  double interestRate = 12;
  double expected_payment =
      calculateAnnuityPayment(loanAmount, loanTerm, interestRate);

  ck_assert(expected_payment > 0);
}
END_TEST

Suite *sample_suite() {
  Suite *s = suite_create("calc logic testing");

  TCase *tc_core = tcase_create("add");
  // tcase_add_checked_fixture(tc_core, setup, teardown);
  tcase_add_test(tc_core, test_sum);
  tcase_add_test(tc_core, test_min);
  tcase_add_test(tc_core, test_mul);
  tcase_add_test(tc_core, test_div);
  tcase_add_test(tc_core, test_pow);
  tcase_add_test(tc_core, test_sqrt);
  tcase_add_test(tc_core, test_mod);
  tcase_add_test(tc_core, test_sin);
  tcase_add_test(tc_core, test_cos);
  tcase_add_test(tc_core, test_tan);
  tcase_add_test(tc_core, test_acos);
  tcase_add_test(tc_core, test_asin);
  tcase_add_test(tc_core, test_atan);
  tcase_add_test(tc_core, test_ln);
  tcase_add_test(tc_core, test_log);
  tcase_add_test(tc_core, test_polish);
  tcase_add_test(tc_core, test_complex);
  tcase_add_test(tc_core, test_error_input);
  tcase_add_test(tc_core, test_validate_ok);
  tcase_add_test(tc_core, test_differential_payments);
  tcase_add_test(tc_core, test_annuity_payment);
  suite_add_tcase(s, tc_core);

  return s;
}

/// \brief запуск всех тестов
int main() {
  SRunner *sr = srunner_create(sample_suite());
  srunner_run_all(sr, CK_NORMAL);
  srunner_ntests_failed(sr);
  srunner_free(sr);

  return 0;
}
