#include "Setting.h"
#include "OpenFile.h"
#include "mainwindow.h"
#include "ui_dialog.h"
#include "qdebug.h"
#include "QMessageBox"
#include "QFileDialog"
#include "QGridLayout"
#include "QPushButton"
#include "QTextStream"
#include "iostream"

Setting::Setting(QWidget *parent, QVector<Datas>*& d)
    : QThread(parent)
    , ds(d)
{
    qDebug()<<"th2 ID:"<<QThread::currentThreadId()<<endl;
}

Setting::~Setting(){

}

void Setting::run(){

}
