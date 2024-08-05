#ifndef S21_LEXEME_PARSER_H
#define S21_LEXEME_PARSER_H

#include "s21_datatypes.h"
extern const char* s21_tfuncs[];

int parse_number(const char* str, stack** head);
int is_digit(char ch);
int is_operator(char ch);
int comma_check(const char* line);
int parse_operator(char ch, stack** head);
int parse_func(const char* line, stack** head);
double get_x();
int set_x(double x);
#endif