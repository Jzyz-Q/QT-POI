#ifndef OPENFILE_H
#define OPENFILE_H

#include <QThread>
#include <QVector>
#include "mainwindow.h"
#include "dialog.h"
#include "ui_dialog.h"

struct Datas;
class MainWindow;
class Openf : public QThread
{
    Q_OBJECT

public:
    Openf(QWidget *parent, QVector<Datas>* &d);
    ~Openf();
    void rec(QString fn);

protected:
    void run();

private:
    QVector<Datas>*& ds;
    QString filename;


signals:
    void SendToMain(int i);
    void isDone();

};

#endif // OPENFILE_H
