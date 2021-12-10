#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include <QTimer>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->Openprogress->setValue(0);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::slotHide()
{
    this->close();
}

void Dialog::on_toolButton_clicked()
{
    QTimer::singleShot(2000, this, SLOT(slotHide()));  // 这里是一个3秒定时器， 且只执行一次。
}


