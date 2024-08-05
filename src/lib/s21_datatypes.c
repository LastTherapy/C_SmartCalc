/*!
\file s21_datatypes.h
\brief Методы для работы со стеком
*/
#include "s21_datatypes.h"

#include <stdio.h>
#include <stdlib.h>

#include "s21_lexeme_parser.h"

/**
 * @brief Добавляет элемент в стек.
 *
 * @param head Указатель на вершину стека.
 * @param data Данные, которые необходимо добавить в стек.
 * @return Указатель на новый элемент стека. Возвращает NULL в случае ошибки.
 */
stack *st_push(stack *head, stack data) {
  stack *new_element = malloc(sizeof(stack));
  if (new_element != NULL) {
    *new_element = data;  // Копирование данных в новый элемент
    new_element->next = head;
    new_element->prew = NULL;
    if (head != NULL) head->prew = new_element;
  }
  return new_element;
}

/*!
 * \brief Удаляет элемент из стека.
 *
 * \param head Двойной указатель на вершину стека.
 * \return Указатель на удаленный элемент стека. Возвращает NULL, если стек
 * пуст.
 */
stack *st_pop(stack **head) {
  if (*head == NULL) {
    return NULL;
  }
  stack *poped = *head;
  *head = (*head)->next;
  return poped;
}

/*!
 * \brief Удаляет весь стек.
 *
 * Функция проходит по всем элементам стека, начиная с головы, и освобождает
 * память.
 *
 * \param head Двойной указатель на вершину стека.
 */
void remove_stack(stack **head) {
  while ((*head) && (*head)->prew != NULL) (*head) = (*head)->prew;

  while ((*head)) {
    stack *st = st_pop(head);
    free(st);
  }
}

/*!
 * \brief Удаляет элемент из начала стека.
 *
 * Функция возвращает первый элемент стека, обновляя указатель на начало стека.
 *
 * \param root Двойной указатель на начало стека.
 * \return Указатель на удаленный элемент стека. Возвращает NULL, если стек
 * пуст.
 */
stack *st_rpop(stack **root) {
  if (*root == NULL) {
    return NULL;
  }
  stack *poped = *root;
  *root = (*root)->prew;
  return poped;
}

/*!
 * \brief Печатает лексему в строку.
 *
 * Функция преобразует лексему из стека в строковое представление.
 *
 * \param st Указатель на элемент стека (лексему).
 * \param out Указатель на строку, в которую будет записана лексема.
 * \return Количество записанных символов.
 */
int print_lexeme(stack *st, char out[]) {
  int r = 0;
  switch (st->type) {
    case S_INTEGER:
      r = sprintf(out, "%d", st->ival);
      break;
    case S_DOUBLE:
      r = sprintf(out, "%g", st->dval);
      break;
    case S_XOPERAND:
    case S_UOPERAND:
    case S_OPERAND:
      r = sprintf(out, "%c", st->ival);
      break;
    case S_FUNC:
      r = sprintf(out, "%s", s21_tfuncs[st->ival]);
    default:
      break;
  }
  return r;
}

/*!
 * \brief Печатает весь стек в строку.
 *
 * Функция проходит по стеку с конца и печатает каждый его элемент в строку.
 *
 * \param head Указатель на вершину стека.
 * \param out Указатель на строку, в которую будут записаны элементы стека.
 */
void print_rstack(stack *head, char out[]) {
  if (head == NULL || out == NULL) return;
  stack *st = head;
  while (st->next != NULL) st = st->next;
  while (st) {
    out += print_lexeme(st, out);
    if (st->prew != NULL) *out++ = ' ';
    st = st->prew;
  }
}
