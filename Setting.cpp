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
#include "QList"

Setting::Setting(QWidget *parent, QVector<Datas>*& d)
    : QThread(parent)
    , ds(d)
{
    qRegisterMetaType<Datas>("Datas");
    qDebug()<<"th2 ID:"<<QThread::currentThreadId()<<endl;
    u_f = lct_f = t_f = lo_f = la_f = false;
    Users = new QVector<int>;
    Locations = new QVector<int>;
    Latitude = new QVector<float>;
    Longitude = new QVector<float>;
    Times1 = new QVector<QDate>;

    AfterSelect = new QVector<Datas>;
}

Setting::~Setting(){

}

void Setting::run(){
    AfterSelect->clear();
    for (int i=0; i<ds->size(); i++){
        Datas tmp = ds->at(i);

        if (((u_f && std::find(Users->begin(),Users->end(),tmp.User_Id) != Users->end()) || !u_f) &&
            ((lct_f && std::find(Locations->begin(),Locations->end(),tmp.Location_Id) != Locations->end()) || !lct_f) &&
            ((la_f && tmp.latitude <= Latitude->at(1) && tmp.latitude >= Latitude->at(0)) || !la_f) &&
            ((lo_f && tmp.longitude <= Longitude->at(1) && tmp.longitude >= Longitude->at(0)) || !lo_f) &&
            ((t_f && tmp.t.day <= Times1->at(1) && tmp.t.day >= Times1->at(0)) || !t_f))
        {
            AfterSelect->append(tmp);
            emit SendDataset(tmp);
        }
    }
    qDebug()<< AfterSelect->size() <<endl;
    u_f = lct_f = t_f = lo_f = la_f = false;
}

void Setting::rec_users(QString users){
    QStringList list;
    list.clear();
    list = users.split(";", QString::SkipEmptyParts);

    u_f = true;
    Users->clear();
    for (int i=0; i<list.size(); i++){
        Users->append(list.at(i).toInt());
        //qDebug() << Users->at(i) << endl;
    }
}

void Setting::rec_locations(QString locations){
    QStringList list;
    list.clear();
    list = locations.split(";", QString::SkipEmptyParts);

    lct_f = true;
    Locations->clear();
    for (int i=0; i<list.size(); i++){
        Locations->append(list.at(i).toInt());
        //qDebug() << Locations->at(i) << endl;
    }
}

void Setting::rec_latitude(QString latitude){
    QStringList list;
    list.clear();
    list = latitude.split(";", QString::SkipEmptyParts);

    la_f = true;
    Latitude->clear();
    for (int i=0; i<list.size(); i++){
        Latitude->append(list.at(i).toFloat());
        //qDebug() << Latitude->at(i) << endl;
    }
}

void Setting::rec_longitude(QString longitude){
    QStringList list;
    list.clear();
    list = longitude.split(";", QString::SkipEmptyParts);

    lo_f = true;
    Longitude->clear();
    for (int i=0; i<list.size(); i++){
        Longitude->append(list.at(i).toFloat());
        //qDebug() << Longitude->at(i) << endl;
    }
}

void Setting::rec_days(QDate daystart, QDate dayend){
    t_f = true;
    Times1->clear();
    Times1->append(daystart);
    Times1->append(dayend);
    //qDebug() << daystart.year() << " " << daystart.month() << " " << daystart.day() << endl;
}






