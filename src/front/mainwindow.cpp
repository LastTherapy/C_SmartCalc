#include "mainwindow.h"

#include "../lib/s21_datatypes.h"
#include "../lib/s21_lexeme_parser.h"
#include "../lib/s21_polish.h"
#include "../lib/s21_validate.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  connect(ui->pushButton_0, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_1, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_8, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_9, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_dot, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_lcoma, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_rcoma, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_plus, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_minus, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_mult, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_div, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_pow, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_sin, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_cos, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_tan, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_sqrt, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_ln, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_log, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_acos, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_asin, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_atan, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_mod, SIGNAL(clicked()), this, SLOT(addOperand()));
  connect(ui->pushButton_x, SIGNAL(clicked()), this, SLOT(addOperand()));

  connect(&cw, &CreditWindow::showMain, this, &MainWindow::show);
}

void MainWindow::showWindow() { this->show(); }

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_pushButton_C_clicked() {
  ui->outputEdit->setText(QString("0"));
}

/**
 * @brief Вычисляет значение математического выражения.
 *
 * Функция принимает строку, содержащую математическое выражение, преобразует её
 * в обратную польскую нотацию, затем вычисляет результат. В случае ошибки
 * парсинга или вычисления возвращает NaN.
 *
 * @param input Строка, содержащая математическое выражение.
 * @return double Результат вычисления выражения, или NaN в случае ошибки.
 */
double calculateExpression(const QString &input) {
  double result = NAN;
  stack *st = parse_all(input.toStdString().c_str());
  if (st != NULL && s21_validate(st) != ERROR) {
    stack *p = to_polish(&st);
    calc_polish(p, &result);
  }
  return result;
}

/**
 * @brief Обрабатывает нажатие на кнопку "=".
 *
 * Функция считывает текущее математическое выражение из текстового поля,
 * вычисляет его и выводит результат обратно в текстовое поле. В случае ошибки
 * отображает сообщение об ошибке.
 */
void MainWindow::on_pushButton_eq_clicked() {
  QString input = ui->outputEdit->text();
  double result = calculateExpression(input);

  if (std::isnan(result)) {
    ui->outputEdit->setText("ERROR");
  } else {
    ui->outputEdit->setText(QString::number(result, 'g', 7));
  }
}

/**
 * @brief Обрабатывает нажатие на кнопку построения графика.
 *
 * Функция считывает значения X и Y из соответствующих полей интерфейса,
 * вычисляет значения функции в заданном диапазоне и строит график функции. В
 * случае ошибки в выражении или его вычислении пропускает соответствующую
 * точку.
 */
void MainWindow::on_pushButton_clicked() {
  double x_min = ui->doubleSpinBox_minx->value();
  double x_max = ui->doubleSpinBox_maxx->value();
  double y_min = ui->doubleSpinBox_miny->value();
  double y_max = ui->doubleSpinBox_maxy->value();
  int numPoints = 9000;  // over 9000

  QVector<double> x, y;
  QString input = ui->outputEdit->text();

  double h = (x_max - x_min) / (numPoints);

  for (int i = 0; i <= numPoints; ++i) {
    double X = x_min + i * h;
    x.push_back(X);
    set_x(X);
    double result = calculateExpression(input);
    if (std::isnan(result)) {
      // NaN  в векторе чтобы были пропуски в графике в таком случае
      y.push_back(std::numeric_limits<double>::quiet_NaN());
    } else {
      y.push_back(result);
    }
  }

  setupGraph(x, y, x_min, x_max, y_min, y_max);
}

/**
 * @brief Настраивает и отображает график.
 *
 * Функция устанавливает данные для графика, конфигурирует внешний вид, задает
 * диапазон отображения и отрисовывает график.
 *
 * @param x Вектор координат X.
 * @param y Вектор координат Y.
 * @param x_min Минимальное значение по оси X.
 * @param x_max Максимальное значение по оси X.
 * @param y_min Минимальное значение по оси Y.
 * @param y_max Максимальное значение по оси Y.
 */
void MainWindow::setupGraph(const QVector<double> &x, const QVector<double> &y,
                            double x_min, double x_max, double y_min,
                            double y_max) {
  ui->graph->addGraph();
  ui->graph->graph(0)->setData(x, y);
  ui->graph->xAxis->setRange(x_min, x_max);
  ui->graph->yAxis->setRange(y_min, y_max);
  ui->graph->xAxis->setLabel("Ось X");
  ui->graph->yAxis->setLabel("Ось Y");
  ui->graph->setInteraction(QCP::iRangeZoom, true);
  ui->graph->graph(0)->setPen(QColor(61, 82, 62, 255));
  QPen graphPen = ui->graph->graph(0)->pen();
  graphPen.setWidth(2);
  ui->graph->graph(0)->setPen(graphPen);
  ui->graph->replot();
  set_x(ui->lcdNumber->value());
}

void MainWindow::addOperand() {
  QPushButton *button = (QPushButton *)sender();
  if (ui->pushButton_eq->isChecked()) ui->outputEdit->setText("");
  ui->pushButton_eq->setChecked(false);
  if (ui->outputEdit->text() != '0' || button->text() == '.' ||
      button->text() == '/')
    ui->outputEdit->setText(ui->outputEdit->text() + button->text());
  else

    ui->outputEdit->setText(button->text());
}

/**
 * @brief Обрабатывает нажатие на кнопку установки значения X.
 *
 * Функция считывает число из текстового поля и устанавливает его в качестве
 * текущего значения X. Отображает установленное значение на LCD-дисплее.
 */
void MainWindow::on_pushButton_setx_clicked() {
  double x = ui->outputEdit->text().toDouble();
  set_x(x);
  ui->lcdNumber->display(x);
}

/**
 * @brief Обрабатывает нажатие на кнопку очистки значения X.
 *
 * Функция устанавливает значение X равным 0 и обновляет отображение на
 * LCD-дисплее.
 */
void MainWindow::on_pushButton_clear_x_clicked() {
  set_x(0);
  ui->lcdNumber->display(0);
}

/**
 * @brief Обрабатывает нажатие на кнопку для повторного использования последнего
 * выражения.
 *
 * Функция устанавливает в текстовом поле последнее использованное
 * математическое выражение.
 */
void MainWindow::on_pushButton_last_exp_clicked() {
  ui->outputEdit->setText(lastUsedString);
}

/**
 * @brief Обрабатывает активацию пункта меню "Credits".
 *
 * Функция скрывает основное окно приложения и отображает окно с информацией о
 * создателях (Credits Window).
 */
void MainWindow::on_actionCredit_triggered() {
  this->hide();
  cw.show();
}

void MainWindow::on_pushButton_10_clicked() { on_actionCredit_triggered(); }
