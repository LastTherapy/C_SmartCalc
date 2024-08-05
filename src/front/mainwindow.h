#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "creditwindow.h"

#ifdef __cplusplus
extern "C" {
#endif
#include "../lib/s21_datatypes.h"
#include "../lib/s21_lexeme_parser.h"
#include "../lib/s21_polish.h"
#include "../lib/s21_validate.h"
#ifdef __cplusplus
}
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void addOperand();

  void on_pushButton_C_clicked();

  void on_pushButton_eq_clicked();

  void on_pushButton_clicked();

  void on_pushButton_setx_clicked();

  void showWindow();

  void on_pushButton_clear_x_clicked();

  void on_pushButton_last_exp_clicked();

  void on_actionCredit_triggered();
  void setupGraph(const QVector<double> &x, const QVector<double> &y,
                  double x_min, double x_max, double y_min, double y_max);

  void on_pushButton_10_clicked();

 private:
  CreditWindow cw;
  Ui::MainWindow *ui;
  QString lastUsedString = "0";
};
#endif  // MAINWINDOW_H
