#ifndef FUNCTION_H
#define FUNCTION_H

#include <QDialog>
#include <QtCharts>
#include <QQuickItem>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "mainwindow.h"
#include "Setting.h"

struct Datas;
struct Time;
class map;
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
    void Trajectory();
    void HeatOfMap();
    void SimilarityOfUsers();
    void handleMarkerClicked();
    void PointHoverd(const QPointF &point, bool state);
    void Delay_MSec(unsigned int msec);

signals:
    void SendMapStart();
    void SendHeatMapStart();
    void SendMapLocation(double lat, double lon);
    void SendHeatMapLocation(int index, int cnt);

public slots:
    void start();
    void rec_data(QVector<Datas> d);
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


    void on_cot_ok_clicked();

    void on_top_ok_clicked();

    void on_ckin_ok_clicked();

    void on_dau_ok_clicked();

    void on_tra_user_box_currentIndexChanged(const QString &arg1);

    void on_pushButton_clicked();

    void on_heat_ok_clicked();

    void on_sml_user_box_currentIndexChanged(const QString &arg1);

    void on_sml_user_box_2_currentIndexChanged(const QString &arg1);

    void on_sml_ok_clicked();

private:
    QVector<Datas> DataSet;
    QScatterSeries *scatterSeries[1000];
    QScatterSeries *smlpieces1[2593];
    QScatterSeries *smlpieces2[2593];
    int max_size;

    QChart* cotchart;
    QChart* smlchart;
    QStringList* xTitle;
    Time* t_start;
    Time* t_end;
    Time* t_start_top;
    Time* t_end_top;
    Time* t_start_cot;
    Time* t_end_cot;
    Time* t_start_ckin;
    Time* t_end_ckin;
    Time* t_start_dau;
    Time* t_end_dau;
    Time* t_start_tra;
    Time* t_end_tra;
    Time* t_start_heat;
    Time* t_end_heat;
    Time* t_start_sml;
    Time* t_end_sml;
    map* qmlmap;
    QObject *item;
    QObject *item_heat;
    int timestep;
    int step;
    int top_la_low;
    int top_la_high;
    int top_lo_low;
    int top_lo_high;
    int ckin_loid;
    int ckin_la_low;
    int ckin_la_high;
    int ckin_lo_low;
    int ckin_lo_high;
    int DAU_loid1;
    int DAU_loid2;
    int cot_user_flag;
    int cot_user[3];
    int top10_user;
    int tra_user;
    int sml_user1;
    int sml_user2;
    int piece[3000];
    int u1_piece[3000];
    int u2_piece[3000];
    bool top_single;
    bool ckin_id_flag;

};

#endif // FUNCTION_H
