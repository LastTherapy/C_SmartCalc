#ifndef S21_POLISH_H
#define S21_POLISH_H

#include "s21_datatypes.h"
stack *to_polish(stack **head);
int calc_polish(stack *postfix, double *result);
stack calc_bioperand(stack *x, stack *y, stack *op);
int calc_uoperand(stack *num, stack *op);
#endif