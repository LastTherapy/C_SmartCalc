#include "creditwindow.h"

#include <QTextEdit>

#include "../lib/s21_creditcal.h"
#include "ui_creditwindow.h"

CreditWindow::CreditWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::CreditWindow) {
  ui->setupUi(this);

  ui->gridLayout->addWidget(ui->payment_month_min, 0, 2);
  ui->gridLayout->addWidget(ui->payment_month_max, 0, 3);

  ui->payment_month_min->hide();
  ui->label_dif_payment->hide();
  ui->payment_month_max->hide();

  this->setWindowTitle("Credit Calculator");
}

CreditWindow::~CreditWindow() { delete ui; }

void CreditWindow::on_pushButton_clicked() {
  double S = ui->creditsum->value();
  double P = ui->P->value();
  int N = static_cast<int>(ui->months->value());
  ui->textEdit->clear();

  if (ui->radioButton_an->isChecked()) {
    double result = calculateAnnuityPayment(S, N, P);
    ui->payment_month->show();
    ui->payment_month_min->hide();
    ui->payment_month_max->hide();
    ui->label_month_pay->show();
    ui->label_dif_payment->hide();
    ui->payment_month->setValue(result);
    ui->payment_all->setValue(result * N);
    ui->overpay->setValue(result * N - S);

    for (int month = 1; month <= N; ++month) {
      ui->textEdit->append("Месяц " + QString::number(month) + ": " +
                           QString::number(result, 'f', 2));
    }
  } else if (ui->radioButton_dif->isChecked()) {
    double *payments = calculateDifferentialPayments(S, N, P);
    if (payments != NULL) {
      double total = 0;
      for (int i = 0; i < N; ++i) {
        total += payments[i];
        ui->textEdit->append("Месяц " + QString::number(i + 1) + ": " +
                             QString::number(payments[i], 'f', 2));
      }
      ui->payment_month_max->setValue(payments[0]);
      ui->payment_month_min->setValue(payments[N - 1]);
      free(payments);  // Освобождение памяти
      ui->payment_all->setValue(total);
      ui->overpay->setValue(total - S);
      ui->payment_month_min->show();
      ui->payment_month_max->show();
      ui->payment_month->hide();
      ui->label_month_pay->hide();
      ui->label_dif_payment->show();
    }
  }
}

void CreditWindow::on_actionNormal_triggered() {
  this->hide();
  emit showMain();
}

void CreditWindow::on_pushButton_2_clicked() { on_actionNormal_triggered(); }
