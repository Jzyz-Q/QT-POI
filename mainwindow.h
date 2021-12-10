#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QVariant>
#include "OpenFile.h"
#include "dialog.h"
#include "Setting.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Time
{
    int year;
    int mon;
    int day;
    int hour;
    int minute;
    int second;

    Time(){
        year = 0;
        mon = 0;
        day = 0;
        hour = 0;
        minute = 0;
        second = 0;
    }
};

struct Datas
{
    int User_Id;
    int Location_Id;
    Time t;
    float latitude;
    float longitude;
};

class Openf;
class Dialog;
class Setting;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Dialog* oui;
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

private:
    Ui::MainWindow *ui;

private:
    QVector<Datas>* Dataset;
    Openf* th1;
    Setting* Set_th2;

    bool userid_flag, locationid_flag, time_flag, lo_flag, la_flag;

signals:
    void SendFile(QString fileName);

};
#endif // MAINWINDOW_H
