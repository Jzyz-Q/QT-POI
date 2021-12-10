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

Openf::Openf(QWidget *parent, QVector<Datas>*& d)
    : QThread(parent)
    , ds(d)
{
    qDebug()<<"th1 ID:"<<QThread::currentThreadId()<<endl;
}

Openf::~Openf()
{

}

void Openf::rec(QString fn){
    qDebug()<<"th1 " << QThread::currentThreadId()<<endl;
    filename = fn;
}

void Openf::run(){
    qDebug() << "当前子线程ID:" << QThread::currentThreadId();
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "Open Excel file failed!";
            return;
        }

    QStringList list;
    list.clear();
    QTextStream in(&file);
    in.setCodec("GB2312");      //支持读取中文信息

    for(int i = 0; !in.atEnd(); i++) {
        QString fileLine = in.readLine();
        list = fileLine.split(",", QString::SkipEmptyParts);

        Datas tmp;
        tmp.User_Id = list.at(0).toInt();
        tmp.Location_Id = list.at(1).toInt();


        tmp.t.year = list.at(2).mid(0,4).toInt();
        tmp.t.mon = list.at(2).mid(5,2).toInt();
        tmp.t.day = list.at(2).mid(8,2).toInt();
        tmp.t.hour = list.at(2).mid(11,2).toInt();
        tmp.t.minute = list.at(2).mid(14,2).toInt();
        tmp.t.second = list.at(2).mid(17,2).toInt();

        tmp.latitude = list.at(3).toFloat();
        tmp.longitude = list.at(4).toFloat();

        ds->push_back(tmp);
        if (i%15025 == 0) emit SendToMain(i);
    }
    emit SendToMain(1502535);
    file.close();
}
