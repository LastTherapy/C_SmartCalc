/*!
 * \file s21_validate.h
 * \brief Функции для валидации структуры стека
 *
 * Данный файл содержит функции для проверки корректности расположения
 * операторов, функций и чисел в стеке. Эти функции помогают гарантировать, что
 * выражение, представленное в виде стека, соответствует правилам математической
 * грамматики.
 */
#include "s21_validate.h"

#include <stdlib.h>

#include "s21_datatypes.h"

/*!
 * \brief Проверяет корректность выражения, представленного стеком.
 *
 * Функция выполняет валидацию стека, проверяя каждый его элемент на
 * соответствие правилам математической грамматики.
 *
 * \param root Указатель на вершину стека.
 * \return Код ошибки или OK при успешной валидации.
 */
int s21_validate(stack* root) {
  int error = OK;
  while (root != NULL) {
    if (root->type == S_UOPERAND)
      error = check_unar(root);
    else if (root->type == S_OPERAND)
      error = check_binar(root);
    else if (root->type == S_FUNC)
      error = check_func(root);
    else if (root->type == S_INTEGER || root->type == S_DOUBLE ||
             root->type == S_XOPERAND)
      error = check_number(root);

    if (error == ERROR) break;

    root = root->next;
  }
  return error;
}

/*!
 * \brief Проверяет корректность расположения бинарного оператора в стеке.
 *
 * Функция проверяет, что перед и после бинарного оператора расположены
 * корректные элементы стека.
 *
 * \param node Указатель на элемент стека, представляющий бинарный оператор.
 * \return OK, если расположение оператора корректно, иначе ERROR.
 */
int check_binar(stack* node) {
  int error = OK;
  if (node->ival == '(' || node->ival == ')') return OK;
  if (node->prew == NULL || node->next == NULL) return ERROR;

  if (node->next->type == S_OPERAND && node->next->ival != ')')
    error = ERROR;
  else if (node->next->type == S_FUNC)
    error = ERROR;
  else if (node->prew->type == S_OPERAND && node->prew->ival != '(')
    error = ERROR;

  return error;
}

/*!
 * \brief Проверяет корректность расположения унарного оператора в стеке.
 *
 * Функция проверяет, что перед унарным оператором расположены корректные
 * элементы стека.
 *
 * \param node Указатель на элемент стека, представляющий унарный оператор.
 * \return OK, если расположение оператора корректно, иначе ERROR.
 */
int check_unar(stack* node) {
  int error = OK;
  if (node == NULL || node->prew == NULL)
    return ERROR;  // Унарный оператор требует аргумента справа
  // Проверяем, что слева от унарного оператора либо ничего нет, либо другой
  // оператор (не закрывающая скобка)
  if (node->next != NULL &&
      (node->next->type != S_OPERAND || node->next->ival == ')')) {
    error = ERROR;
  }
  // Справа от унарного оператора не должно быть бинарого оператора
  if ((node->prew->type == S_OPERAND && node->prew->ival != '(') ||
      node->prew->type == S_UOPERAND) {
    error = ERROR;
  }
  return error;
}

/*!
 * \brief Проверяет корректность расположения функции в стеке.
 *
 * Функция проверяет, что перед и после функции расположены корректные элементы
 * стека.
 *
 * \param node Указатель на элемент стека, представляющий функцию.
 * \return OK, если расположение функции корректно, иначе ERROR.
 */
int check_func(stack* node) {
  int error = OK;
  if (node == NULL || node->prew == NULL) return ERROR;

  // Проверяем, что перед функцией нет числа, переменной x или закрывающей
  // скобки
  if (node->next != NULL &&
      (node->next->type == S_INTEGER || node->next->type == S_DOUBLE ||
       node->next->type == S_XOPERAND ||
       (node->next->type == S_OPERAND && node->next->ival == ')'))) {
    error = ERROR;
  }

  // Проверяем, что следующий элемент после функции - открывающая скобка
  if (!(node->prew->type == S_OPERAND && node->prew->ival == '(')) {
    error = ERROR;
  }
  return error;
}

/*!
 * \brief Проверяет корректность расположения числа в стеке.
 *
 * Функция проверяет, что перед и после числа расположены корректные элементы
 * стека.
 *
 * \param node Указатель на элемент стека, представляющий число.
 * \return OK, если расположение числа корректно, иначе ERROR.
 */
int check_number(stack* node) {
  int error = ERROR;
  if ((node->next == NULL ||
       (node->next->type == S_OPERAND && node->next->ival != ')') ||
       node->next->type == S_UOPERAND) &&
      (node->prew == NULL ||
       (node->prew->type == S_OPERAND && node->prew->ival != '(')))
    error = OK;
  return error;
}