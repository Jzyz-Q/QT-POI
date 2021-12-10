#ifndef SETTING_H
#define SETTING_H

#include <QThread>
#include <QVector>
#include "mainwindow.h"
#include "dialog.h"
#include "ui_dialog.h"

struct Datas;
class MainWindow;
class Setting : public QThread
{
    Q_OBJECT

public:
    Setting(QWidget *parent, QVector<Datas>* &d);
    ~Setting();

protected:
    void run();

private:
    QVector<Datas>*& ds;

signals:

};
#endif // SETTING_H
