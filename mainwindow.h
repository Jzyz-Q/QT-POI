#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QVariant>
#include <QTime>
#include <QDate>
#include "OpenFile.h"
#include "dialog.h"
#include "Setting.h"
#include "function.h"
#include "QDebug"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Time
{

    QDate day;
    QTime moment;

    Time(){}

    Time(QDate d, QTime m){
        day = d;
        moment = m;
    }
};

struct Datas
{
    int User_Id;
    int Location_Id;
    Time t;
    float latitude;
    float longitude;
    int filter_flag;

    Datas(){
        filter_flag = -1;
    }

    Datas(int u, int l, QDate d, QTime m, float la, float lo, int f){
        User_Id = u;
        Location_Id = l;
        t = Time(d, m);
        latitude = la;
        longitude = lo;
        filter_flag = f;
    }

    Datas(int u, int l, Time t1, float la1, float lo1, int f){
        User_Id = u;
        Location_Id = l;
        t = t1;
        latitude = la1;
        longitude = lo1;
        filter_flag = f;
    }

    bool operator<(Datas tmp){
        if ((filter_flag & (1<<6))){
            return User_Id < tmp.User_Id;
        }
        else if ((filter_flag & (1<<5))) {
            return Location_Id < tmp.Location_Id;
        }
        else if ((filter_flag & (1<<4))) {
            return (t.day.year() > tmp.t.day.year())? 0 :
                   (t.day.month() > tmp.t.day.month())? 0 :
                   (t.day.day() > tmp.t.day.day())? 0 :
                                        1 ;
        }
        else if ((filter_flag & (1<<3))) {
            return (t.moment.hour() > tmp.t.moment.hour())? 0 :
                   (t.moment.minute() > tmp.t.moment.minute())? 0 :
                   (t.moment.second() > tmp.t.moment.second())? 0 :
                                              1 ;
        }
        else if ((filter_flag & (1<<2))) {
            return latitude < tmp.latitude;
        }
        else if ((filter_flag & (1<<1))) {
            return longitude < tmp.longitude;
        }
        else if ((filter_flag & 1)){
            return (t.day.year() > tmp.t.day.year())? 0 :
                   (t.day.month() > tmp.t.day.month())? 0 :
                   (t.day.day() > tmp.t.day.day())? 0 :
                   (t.moment.hour() > tmp.t.moment.hour())? 0 :
                   (t.moment.minute() > tmp.t.moment.minute())? 0 :
                   (t.moment.second() > tmp.t.moment.second())? 0 :
                                                                1;
        }
        else if (filter_flag == -1){
            qDebug() << "filter_flag = -1, wrong!" << endl;
        }
        else {
            qDebug() << "filter_flag undesigned" <<endl;
        }
    }
};

class Openf;
class Dialog;
class Setting;
class Function;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Dialog* oui;
    Function* fui;
    void RecFromOp(int i);

public slots:
    void FileOpen();

private slots:
    void begin();
    void SlcUserid();
    void SlcLocationid();
    void SlcTime();
    void SlcLo();
    void SlcLa();
    void setRange();
    void startfunction();

private:
    Ui::MainWindow *ui;

private:
    QVector<Datas>* Dataset;
    Openf* th1;
    Setting* Set_th2;

    bool userid_flag, locationid_flag, time_flag, lo_flag, la_flag;

signals:
    void SendFile(QString fileName);
    void SendUsers(QString users);
    void SendLocations(QString locations);
    void SendLatitude(QString latitude);
    void SendLongitude(QString longitude);
    void SendDays(QDate daystart, QDate dayend);

};
#endif // MAINWINDOW_H
