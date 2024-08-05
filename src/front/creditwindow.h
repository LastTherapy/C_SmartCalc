#ifndef CREDITWINDOW_H
#define CREDITWINDOW_H

#include <QMainWindow>
#ifdef __cplusplus
extern "C" {
#endif
#include "../lib/s21_creditcal.h"
#ifdef __cplusplus
}
#endif

namespace Ui {
class CreditWindow;
}

class CreditWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit CreditWindow(QWidget *parent = nullptr);
  ~CreditWindow();

 private slots:
  void on_pushButton_clicked();

  void on_actionNormal_triggered();

  void on_pushButton_2_clicked();

 signals:
  void showMain();

 private:
  Ui::CreditWindow *ui;
};

#endif  // CREDITWINDOW_H
