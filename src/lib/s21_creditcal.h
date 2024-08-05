#ifndef S21_CREDITCAL_H
#define S21_CREDITCAL_H

#ifdef __cplusplus
extern "C" {
#endif

double* calculateDifferentialPayments(double loanAmount, int loanTerm,
                                      double interestRate);
double calculateAnnuityPayment(double loanAmount, int loanTerm,
                               double interestRate);

#ifdef __cplusplus
}
#endif

#endif  // S21_CREDITCAL_H
