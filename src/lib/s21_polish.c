/*!
 * \file s21_polish.h
 * \brief Функции для работы с обратной польской записью
 *
 * Данный файл содержит определения функций, используемых для преобразования
 * выражений в обратную польскую запись и их последующего вычисления.
 * Он включает функции для определения приоритета операторов, преобразования
 * выражений в обратную польскую запись, вычисления выражений, записанных
 * в этой форме, а также дополнительные функции для работы с унарными и
 * бинарными операторами.
 */
#include "s21_polish.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "s21_datatypes.h"

/*!
 * \brief Определяет приоритет оператора в стеке.
 *
 * Функция возвращает числовой приоритет оператора, который используется
 * при преобразовании выражения в обратную польскую запись.
 *
 * \param op Указатель на элемент стека, представляющий оператор.
 * \return Числовой приоритет оператора.
 */
int get_priority(stack *op) {
  int result = -1;
  if (op->type == S_OPERAND) {
    switch (op->ival) {
      case '(':
        result = 0;
        break;
      case '+':
      case '-':
        result = 1;
        break;
      case '/':
      case '*':
      case '%':
        result = 2;
        break;
      case '^':
        result = 3;
      default:
        break;
    }
  } else if (op->type == S_FUNC || op->type == S_UOPERAND) {
    result = 4;
  }
  return result;
}

/*!
 * \brief Преобразует выражение в обратную польскую запись.
 *
 * Функция преобразует выражение, представленное стеком лексем, в обратную
 * польскую запись для дальнейшего вычисления.
 *
 * \param head Двойной указатель на вершину стека лексем.
 * \return Указатель на вершину стека, представляющего выражение в обратной
 * польской записи.
 */
stack *to_polish(stack **head) {
  stack *result = NULL;
  stack **root = head;
  stack *buffer = NULL;

  while (root && (*root)->next != NULL) *root = (*root)->next;

  stack *lex = NULL;

  while ((lex = st_rpop(root)) != NULL) {
    if (lex->type == S_INTEGER || lex->type == S_DOUBLE ||
        lex->type == S_XOPERAND) {
      result = st_push(result, *lex);
    } else if (lex->type == S_OPERAND || lex->type == S_UOPERAND ||
               lex->type == S_FUNC) {
      if (lex->ival == '(') {
        buffer = st_push(buffer, *lex);
      } else if (lex->ival == ')') {
        while (buffer != NULL && buffer->ival != '(') {
          stack *op = st_pop(&buffer);
          result = st_push(result, *op);
          free(op);
        }
        stack *temp = st_pop(&buffer);
        free(temp);
      } else {
        while (buffer && buffer->ival != '(' &&
               get_priority(buffer) >= get_priority(lex)) {
          stack *op = st_pop(&buffer);
          result = st_push(result, *op);
          free(op);
        }
        buffer = st_push(buffer, *lex);
      }
    }
    free(lex);
  }

  while (buffer) {
    stack *op = st_pop(&buffer);
    result = st_push(result, *op);
    free(op);
  }
  return result;
}

/*!
 * \brief Вычисляет значение выражения, представленного в обратной польской
 * записи.
 *
 * Функция вычисляет значение выражения, представленного стеком в обратной
 * польской записи, и записывает результат в переданный указатель на double.
 *
 * \param postfix Указатель на вершину стека в обратной польской записи.
 * \param result Указатель на переменную типа double для записи результата.
 * \return Код ошибки или OK при успешном выполнении.
 */
int calc_polish(stack *postfix, double *result) {
  int error = OK;
  stack *root = postfix;
  while (root && root->next != NULL) root = root->next;

  stack *lex = NULL;
  stack *nums = NULL;
  while ((lex = st_rpop(&root)) != NULL) {
    if (lex->type == S_INTEGER || lex->type == S_DOUBLE ||
        lex->type == S_XOPERAND) {
      nums = st_push(nums, *lex);
    } else if (lex->type == S_OPERAND) {
      stack *b = st_pop(&nums);
      stack *a = st_pop(&nums);
      // divide by 0
      if (lex->ival == '/' && b->dval == 0) error = ERROR;
      nums = st_push(nums, calc_bioperand(a, b, lex));
      free(a);
      free(b);
    } else if (lex->type == S_UOPERAND || lex->type == S_FUNC) {
      calc_uoperand(nums, lex);
    }
    free(lex);
  }

  stack *final = st_pop(&nums);
  *result = final->dval;
  free(final);

  return error;
}

/*!
 * \brief Вычисляет значение унарного оператора или функции для числа в стеке.
 *
 * Функция применяет унарный оператор или функцию к числу в вершине стека.
 *
 * \param num Указатель на стек, содержащий числа.
 * \param op Указатель на стек, содержащий унарный оператор или функцию.
 * \return Код ошибки или OK при успешном выполнении.
 */
int calc_uoperand(stack *num, stack *op) {
  int error = OK;
  if (op->ival == '+') return error;
  switch (op->ival) {
    case '-':
      num->dval *= -1;
      break;
    case 0:
      num->dval = sin(num->dval);
      break;
    case 1:
      num->dval = cos(num->dval);
      break;
    case 2:
      num->dval = tan(num->dval);
      break;
    case 3:
      num->dval = acos(num->dval);
      break;
    case 4:
      num->dval = asin(num->dval);
      break;
    case 5:
      num->dval = atan(num->dval);
      break;
    case 6:
      if (num->dval < 0) error = ERROR;
      num->dval = sqrt(num->dval);
      break;
    case 7:
      num->dval = log(num->dval);
      break;
    case 8:
      num->dval = log10(num->dval);
      break;
    default:
      break;
  }
  return error;
}

/*!
 * \brief Вычисляет результат бинарного оператора для двух чисел.
 *
 * Функция применяет бинарный оператор к двум числам и возвращает результат.
 *
 * \param x Указатель на первое число.
 * \param y Указатель на второе число.
 * \param op Указатель на бинарный оператор.
 * \return Структура stack, содержащая результат операции.
 */
stack calc_bioperand(stack *x, stack *y, stack *op) {
  double a = x->dval;
  double b = y->dval;
  double r = 0;
  switch (op->ival) {
    case '+':
      r = a + b;
      break;
    case '-':
      r = a - b;
      break;
    case '*':
      r = a * b;
      break;
    case '/':
      r = a / b;
      break;
    case '^':
      r = pow(a, b);
      break;
    case '%':
      r = fmod(a, b);
      break;
    default:
      break;
  }
  stack result = {0};
  result.type = S_DOUBLE;
  result.dval = r;
  return result;
}