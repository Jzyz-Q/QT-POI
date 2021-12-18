#ifndef FUNCTION_H
#define FUNCTION_H

#include <QDialog>
#include <QtCharts>
#include "mainwindow.h"
#include "Setting.h"

struct Datas;
struct Time;
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
        return l_cnt > tmp.l_cnt;
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


    void Top10POIs();
    void ComprasionOfTop();
    void NumberOfCheckin();
    void DAU();
    void handleMarkerClicked();
    void PointHoverd(const QPointF &point, bool state);

public slots:
    void test();
    void start();
    void rec_dataset(Datas da);
    void rec_finished(int a);

private slots:
    void on_locationBox_currentIndexChanged(const QString &arg1);

    void on_cot_user_Box1_currentIndexChanged(const QString &arg1);

    void on_cot_user_Box2_currentIndexChanged(const QString &arg1);

    void on_dau_lo1_textEdited(const QString &arg1);

    void on_dau_lo2_textEdited(const QString &arg1);

    void on_top_user_Box_currentIndexChanged(const QString &arg1);

    void on_step_plus_clicked();

    void on_Timestep_currentIndexChanged(const QString &arg1);

    void on_step_minus_clicked();

    void on_stepEdit_textChanged(const QString &arg1);

private:
    QVector<Datas> DataSet;

    QChart* chart;
    QValueAxis* mAxY;
    QValueAxis* mAxX;
    QLineSeries* mLineSeries;

    Time* t_start;
    Time* t_end;
    int timestep;
    int step;
    int top_la_low;
    int top_la_high;
    int top_lo_low;
    int top_lo_high;
    int ckin_loid;
    int DAU_loid1;
    int DAU_loid2;
    int cot_user_flag;
    int cot_user[3];
    int top10_user;
    bool top_single;

};

#endif // FUNCTION_H
