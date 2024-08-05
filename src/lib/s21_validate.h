#ifndef S21_VALIDATE_H
#define S21_VALIDATE_H
#include "s21_datatypes.h"

int s21_validate(stack* root);
int check_unar(stack* node);
int check_binar(stack* node);
int check_func(stack* node);
int check_number(stack* node);

#endif