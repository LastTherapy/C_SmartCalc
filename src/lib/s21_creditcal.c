/**
 *@file s21_creditcal.h
 *@brief Методы для работы с кредитным калькулятором
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Рассчитывает дифференцированные платежи по кредиту.
 *
 * @param loanAmount Общая сумма кредита.
 * @param loanTerm Срок кредита в месяцах.
 * @param interestRate Годовая процентная ставка.
 * @return double* Указатель на массив с размерами ежемесячных платежей.
 */
double* calculateDifferentialPayments(double loanAmount, int loanTerm,
                                      double interestRate) {
  double monthlyInterestRate = interestRate / 12 / 100;
  double principalPayment = loanAmount / loanTerm;
  double* payments = malloc(sizeof(double) * loanTerm);

  if (payments != NULL) {
    for (int month = 0; month < loanTerm; ++month) {
      double remainingAmount = loanAmount - principalPayment * month;
      double interestPayment = remainingAmount * monthlyInterestRate;
      payments[month] = principalPayment + interestPayment;
    }
  }
  return payments;
}

/**
 * @brief Рассчитывает размер аннуитетного платежа по кредиту.
 *
 * @param loanAmount Общая сумма кредита.
 * @param loanTerm Срок кредита в месяцах.
 * @param interestRate Годовая процентная ставка.
 * @return double Размер ежемесячного аннуитетного платежа.
 */
double calculateAnnuityPayment(double loanAmount, int loanTerm,
                               double interestRate) {
  double monthlyInterestRate = interestRate / 12 / 100;
  double annuityFactor = monthlyInterestRate *
                         pow(1 + monthlyInterestRate, loanTerm) /
                         (pow(1 + monthlyInterestRate, loanTerm) - 1);
  return loanAmount * annuityFactor;
}
