#ifndef S21_DATATYPES_H
#define S21_DATATYPES_H

/*!
 * \defgroup StackMacros Макросы для типов данных стека
 * @{
 * \brief Макросы, определяющие типы данных в стеке и коды состояний.
 */

//! Тип данных стека: целое число.
#define S_INTEGER 'd'

//! Тип данных стека: число с плавающей точкой.
#define S_DOUBLE 'f'

//! Тип данных стека: операнд.
#define S_OPERAND 'o'

//! Тип данных стека: унарный операнд.
#define S_UOPERAND '~'

//! Тип данных стека: переменная 'x'.
#define S_XOPERAND 'x'

//! Тип данных стека: функция.
#define S_FUNC '?'

//! Код ошибки.
#define ERROR -1

//! Булево значение "истина".
#define TRUE 1

//! Булево значение "ложь".
#define FALSE 0

//! Код успешного завершения операции.
#define OK 1

/*! @} */

/*!
 * \struct stack
 * \brief Структура для представления элементов стека.
 *
 * Структура stack используется для хранения элементов стека, включая
 * числа, операнды и функции. Она поддерживает различные типы данных и
 * содержит указатели для организации стека в виде двусвязного списка.
 */
typedef struct stack {
  char type;
  int ival;
  double dval;
  struct stack *prew;
  struct stack *next;
} stack;

stack *st_push(stack *, stack);
stack *parse_all(const char *line);
stack *st_pop(stack **head);
stack *st_rpop(stack **root);
void remove_stack(stack **head);
void print_rstack(stack *head, char out[]);
int print_lexeme(stack *st, char out[]);
#endif