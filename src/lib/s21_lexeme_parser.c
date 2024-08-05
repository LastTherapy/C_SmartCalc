/*!
\file s21_lexeme_parser.h
\brief Методы для считывания лексем из строки и проверки синтаксически
корректного ввода
*/

#include "s21_lexeme_parser.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_datatypes.h"

const char* s21_tfuncs[] = {"sin",  "cos", "tan", "acos", "asin", "atan",
                            "sqrt", "ln",  "log", "mod",  "\0"};

/*!
 * \brief Добавляет лексему в стек.
 *
 * Функция добавляет новую лексему в стек. В случае ошибки выделения памяти
 * выводится сообщение об ошибке.
 *
 * \param head Двойной указатель на вершину стека.
 * \param buffer Лексема для добавления в стек.
 */
void add_lexeme(stack** head, stack buffer) {
  stack* new_head = st_push(*head, buffer);
  if (new_head == NULL) {
    perror("MALLOC ERROR");
  }
  *head = new_head;
}

/*!
 * \brief Анализирует строку и создает стек лексем.
 *
 * Функция обрабатывает входную строку и преобразует ее в стек лексем,
 * представляющих числа, функции и операторы.
 *
 * \param line Входная строка для анализа.
 * \return Указатель на вершину стека лексем или NULL в случае ошибки.
 */
stack* parse_all(const char* line) {
  stack* head = NULL;
  int error = OK;
  error = comma_check(line);
  while (*line != '\0') {
    if (*line == ' ') {
      line++;
      continue;
    }
    // number
    int code = ERROR;

    if (code == ERROR) code = parse_number(line, &head);
    if (code == ERROR) code = parse_func(line, &head);
    if (code == ERROR) code = parse_operator(*line, &head);

    if (code != ERROR)
      line += code;
    else {
      error = ERROR;
      break;
    }
  }
  if (error != OK) remove_stack(&head);
  return error == OK ? head : NULL;
}

/*!
 * \brief Анализирует строку на наличие функций.
 *
 * Функция ищет в строке известные функции (например, sin, cos) и добавляет их в
 * стек.
 *
 * \param line Строка для анализа.
 * \param head Двойной указатель на вершину стека.
 * \return Количество обработанных символов или ERROR в случае ошибки.
 */
int parse_func(const char* line, stack** head) {
  int result = ERROR;
  for (int i = 0; strcmp(s21_tfuncs[i], "\0"); i++) {
    for (int j = 0; s21_tfuncs[i][j]; j++) {
      if (line[j] != s21_tfuncs[i][j]) break;
      if (s21_tfuncs[i][j + 1] == '\0') result = OK;
    }
    if (result == OK) {
      stack buffer = {0};
      // the last func is fmod which is binary operand
      if (strcmp(s21_tfuncs[i + 1], "\0") == 0) {
        buffer.type = S_OPERAND;
        buffer.ival = '%';
      } else {
        buffer.type = S_FUNC;
        buffer.ival = i;
      }
      add_lexeme(head, buffer);
      result = strlen(s21_tfuncs[i]);

      break;
    }
  }
  return result;
}

/*!
 * \brief Проверяет корректность расстановки скобок в выражении.
 *
 * Функция проверяет строку на соответствие правилам расстановки открывающих
 * и закрывающих скобок.
 *
 * \param line Входная строка для проверки.
 * \return OK, если скобки расставлены корректно, иначе ERROR.
 */
int comma_check(const char* line) {
  int start = 0;
  char lastch = 0;
  while (*line) {
    if (*line == '(') {
      if (lastch == ')') return ERROR;
      start++;
    }
    if (*line == ')') {
      if (start == 0 || lastch == '(')
        return ERROR;
      else
        start--;
    }
    lastch = *line;
    line++;
  }
  return start != 0 ? ERROR : OK;
}

/*!
 * \brief Анализирует символ на предмет является ли он оператором.
 *
 * Функция определяет, является ли символ оператором, и добавляет его в стек.
 *
 * \param ch Символ для анализа.
 * \param head Двойной указатель на вершину стека.
 * \return OK, если символ является оператором, иначе ERROR.
 */
int parse_operator(char ch, stack** head) {
  if (!is_operator(ch)) {
    return ERROR;
  }
  stack buffer = {0};

  if (ch == 'x') {
    buffer.type = S_XOPERAND;
    buffer.dval = get_x();
  } else if (ch == '(' || ch == ')') {
    buffer.type = S_OPERAND;
  }

  else if (ch == '-' || ch == '+') {
    // Унарный оператор, если стек пуст, предыдущий символ - открывающая скобка,
    // или предыдущий символ - оператор умножения, деления, остатка от деления
    // или возведения в степень
    if ((*head) == NULL || ((**head).type == S_OPERAND &&
                            ((**head).ival == '(' || (**head).ival == '*' ||
                             (**head).ival == '/' || (**head).ival == '%' ||
                             (**head).ival == '^'))) {
      buffer.type = S_UOPERAND;
    }
    // В противном случае обрабатываем как бинарный оператор
    else {
      buffer.type = S_OPERAND;
    }
  }
  // Обработка остальных бинарных операторов
  else {
    buffer.type = S_OPERAND;
  }

  buffer.ival = ch;
  // Добавляем лексему в стек
  add_lexeme(head, buffer);
  return OK;
}

/*!
 * \brief Проверяет, является ли символ оператором.
 *
 * Функция проверяет, является ли данный символ одним из допустимых операторов.
 *
 * \param ch Символ для проверки.
 * \return TRUE, если символ является оператором, иначе FALSE.
 */
int is_operator(char ch) {
  char* list = "+-*/()x^";
  while (*list != '\0') {
    if (ch == *list) return TRUE;
    list++;
  }
  return FALSE;
}

/*!
 * \brief Анализирует строку на предмет числа.
 *
 * Функция анализирует строку и извлекает из нее число, добавляя его в стек.
 *
 * \param str Строка для анализа.
 * \param head Двойной указатель на вершину стека.
 * \return Количество обработанных символов или ERROR в случае ошибки.
 */
int parse_number(const char* str, stack** head) {
  if (!is_digit(*str) && *str != '.') return ERROR;
  int point = 0;
  int ivalue = 0;
  int fractional = 0;
  double dvalue = 0;
  const char* s = str;
  stack data = {0};
  while (1) {
    // no point
    if (point == 0) {
      if (is_digit(*s)) {
        ivalue *= 10;
        ivalue += *s - '0';

      } else {
        if (*s == '.') {
          point++;
          s++;
          continue;
        } else
          data.type = 'd';
        data.ival = ivalue;
        data.dval = (double)data.ival;
        break;
      }
      s++;
    }
    // point
    else {
      if (*s == '.') return ERROR;  // two points
      if (!is_digit(*s)) {
        if (ivalue == 0 && fractional == 0)
          return ERROR;  // point without digit
        data.type = 'f';
        dvalue = ivalue + (dvalue / pow(10, fractional));
        data.dval = dvalue;
        break;
      }
      dvalue *= 10;
      dvalue += *s - '0';
      fractional++;
      s++;
    }
  }  // end while
  add_lexeme(head, data);
  return s - str;
}  // end parse_number

/*!
 * \brief Проверяет, является ли символ цифрой.
 *
 * Функция определяет, является ли символ цифрой.
 *
 * \param ch Символ для проверки.
 * \return TRUE, если символ является цифрой, иначе FALSE.
 */
int is_digit(char ch) { return ('0' <= ch && ch <= '9') ? TRUE : FALSE; }

/*!
 * \brief Получает значение переменной x из окружения.
 *
 * Функция извлекает значение переменной x из переменных окружения.
 *
 * \return Значение переменной x.
 */
double get_x() {
  char* s21_x = getenv("S21_X");
  double result = 0;
  if (s21_x) result = strtod(s21_x, NULL);

  return result;
}

/*!
 * \brief Устанавливает значение переменной x в окружении.
 *
 * Функция устанавливает значение переменной x в переменных окружения.
 *
 * \param x Значение для установки.
 * \return OK, если значение успешно установлено, иначе ERROR.
 */
int set_x(double x) {
  // double вмещает около 15 значащих цифр
  int error = OK;
  char out[20] = {0};
  sprintf(out, "%lf", x);
  if (setenv("S21_X", out, 1) != 0) {
    perror("setenv failed");
    error = ERROR;
  }
  return error;
}
