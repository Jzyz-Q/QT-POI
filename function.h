#ifndef FUNCTION_H
#define FUNCTION_H

#include <QDialog>
#include <QtCharts>
#include "mainwindow.h"
#include "Setting.h"

struct Datas;
namespace Ui {
class Function;
}

struct lct_cnt
{
    int l_id;
    int l_cnt;
    float lo;
    float la;

    lct_cnt() {}

    lct_cnt(int id, float a, float o){
        l_id = id;
        l_cnt = 0;
        la = a;
        lo = o;
    }

    lct_cnt(int id, int cnt, float a, float o){
        l_id = id;
        l_cnt = cnt;
        la = a;
        lo = o;
    }

    bool operator <(lct_cnt tmp){
        return l_cnt < tmp.l_cnt;
    }

    bool operator ==(lct_cnt tmp){
        return l_id == tmp.l_id;
    }

};

class Function : public QDialog
{
    Q_OBJECT

public:
    Ui::Function *ui;
    explicit Function(QWidget *parent);
    ~Function();

    void test();
    void linetest();
    void Top10POIs();
    void NumberOfCheckin();
    void checkNumberOfCheckin();
    void DAU();
    void paintDAU();
    void handleMarkerClicked();
    void PointHoverd(const QPointF &point, bool state);

public slots:
    void start();
    void rec_dataset(Datas da);

private slots:
    void on_locationBox_currentIndexChanged(const QString &arg1);

    void on_DAUBox1_currentIndexChanged(const QString &arg1);

    void on_DAUBox_2_currentIndexChanged(const QString &arg1);

private:
    QVector<Datas> DataSet;

    QChart* chart;
    QValueAxis* mAxY;
    QValueAxis* mAxX;
    QLineSeries* mLineSeries;

    int ckin_loid;
    int DAU_loid1;
    int DAU_loid2;

};

#endif // FUNCTION_H
