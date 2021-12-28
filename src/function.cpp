#include "mainwindow.h"
#include "Setting.h"
#include "function.h"
#include "ui_function.h"
#include "QtCharts"
#include "iostream"
#include "cmath"
#include <synchapi.h>
#include <QOpenGLFramebufferObject>

struct lct_cnt;
struct Time;
Function::Function(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Function)
{
    qRegisterMetaType<QVector<Datas>>("QVector<Datas>");
    qRegisterMetaType<Time>("Time");
    ui->setupUi(this);
    ui->mapWidget->setFocus();
    ui->mapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->mapWidget->setSource(QUrl("qrc:/map.qml"));
    ui->heatmapWidget->setFocus();
    ui->heatmapWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->heatmapWidget->setSource(QUrl("qrc:/map.qml"));
    item = new QObject;
    item_heat = new QObject;
    item = ui->mapWidget->rootObject();
    item_heat = ui->heatmapWidget->rootObject();

    xTitle = new QStringList;
    cotchart  = new QChart;
    smlchart = new QChart;
    for (int i=0; i<1000; i++){
        scatterSeries[i] = new QScatterSeries;
        //cotchart->addSeries(scatterSeries[i]);
        connect(scatterSeries[i],&QScatterSeries::hovered, this,&Function::PointHoverd);
    }
    for (int i=0; i<2592; i++){
        smlpieces1[i] = new QScatterSeries;
        smlpieces2[i] = new QScatterSeries;
    }

    t_start = new Time;
    t_end = new Time;
    t_start->day.setDate(2009,2,4);
    t_end->day.setDate(2010,10,23);
    DAU_loid1 = 0;
    DAU_loid2 = 0;
    top_la_low = -90;
    top_la_high = 90;
    top_lo_low = -180;
    top_lo_high = 180;
    ckin_la_low = -90;
    ckin_la_high = 90;
    ckin_lo_low = -180;
    ckin_lo_high = 180;
    ckin_id_flag = false;
    cot_user_flag = 3;
    cot_user[0] = cot_user[1] = -1;
    top_single = false;
    step = 1;
    timestep = 626;

//Top10 connect
    //time slider
    connect(ui->top_start_time,&QSlider::valueChanged,[=](int d){
        if (d > ui->top_end_time->value()){
            d = ui->top_end_time->value();
            ui->top_start_time->setSliderPosition(d);
        }
        ui->top_start_edit->setDate(t_start->day.addDays(d));
    });
    connect(ui->top_end_time,&QSlider::valueChanged,[=](int d){
        if (d < ui->top_start_time->value()){
            d = ui->top_start_time->value();
        }
        ui->top_end_edit->setDate(t_start->day.addDays(d));
    });
    connect(ui->top_start_edit,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day > ui->top_end_edit->date()){
            day = ui->top_end_edit->date();
        }
        ui->top_start_time->setValue(t_start->day.daysTo(day));
    });
    connect(ui->top_end_edit,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day < ui->top_start_edit->date()){
            day = ui->top_start_edit->date();
        }
        ui->top_end_time->setValue(t_start->day.daysTo(day));
    });


//ComprasionTop10 connect
    //time slider
    connect(ui->cot_start_time,&QSlider::valueChanged,[=](int d){
        if (d > ui->cot_end_time->value()){
            d = ui->cot_end_time->value();
        }
        ui->cot_start_edit->setDate(t_start->day.addDays(d));
    });
    connect(ui->cot_end_time,&QSlider::valueChanged,[=](int d){
        if (d < ui->cot_start_time->value()){
            d = ui->cot_start_time->value();
        }
        ui->cot_end_edit->setDate(t_start->day.addDays(d));
    });
    connect(ui->cot_start_edit,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day > ui->cot_end_edit->date()){
            day = ui->cot_end_edit->date();
        }
        ui->cot_start_time->setValue(t_start->day.daysTo(day));
    });
    connect(ui->cot_end_edit,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day < ui->cot_start_edit->date()){
            day = ui->cot_start_edit->date();
        }
        ui->cot_end_time->setValue(t_start->day.daysTo(day));
    });


//Checkin connect
    //time
    connect(ui->ckin_start_time_slider,&QSlider::valueChanged,[=](int d){
        if (d > ui->ckin_end_time_slider->value()){
            d = ui->ckin_end_time_slider->value();
        }
        ui->dateTimeEdit->setDate(t_start->day.addDays(d));
    });
    connect(ui->ckin_end_time_slider,&QSlider::valueChanged,[=](int d){
        if (d < ui->ckin_start_time_slider->value()){
            d = ui->ckin_start_time_slider->value();
        }
        ui->dateTimeEdit_2->setDate(t_start->day.addDays(d));
    });
    connect(ui->dateTimeEdit,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day > ui->dateTimeEdit_2->date()){
            day = ui->dateTimeEdit_2->date();
        }
        ui->ckin_start_time_slider->setValue(t_start->day.daysTo(day));
    });
    connect(ui->dateTimeEdit_2,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day < ui->dateTimeEdit->date()){
            day = ui->dateTimeEdit->date();
        }
        ui->ckin_end_time_slider->setValue(t_start->day.daysTo(day));
    });

    //latitude & longitude
    connect(ui->la_low,&QSlider::valueChanged,[=](float d){
        if (d > ui->la_high->value()){
            d = ui->la_high->value();
        }
        ckin_la_low = d/10.0;
        ui->ckin_la_low->setText(QString::number(d/10.0));
    });

    connect(ui->la_high,&QSlider::valueChanged,[=](float d){
        if (d < ui->la_low->value()){
            d = ui->la_low->value();
        }
        ckin_la_high = d/10.0;
        ui->ckin_la_high->setText(QString::number(d/10.0));
    });
    connect(ui->lo_low,&QSlider::valueChanged,[=](float d){
        if (d > ui->lo_high->value()){
            d = ui->lo_high->value();
        }
        ckin_lo_low = d/10.0;
        ui->ckin_lo_low->setText(QString::number(d/10.0));
    });

    connect(ui->lo_high,&QSlider::valueChanged,[=](float d){
        if (d < ui->lo_low->value()){
            d = ui->lo_low->value();
        }
        ckin_lo_high = d/10.0;
        ui->ckin_lo_high->setText(QString::number(d/10.0));
    });

// DAU connect
    //time
    connect(ui->dau_time_low,&QSlider::valueChanged,[=](int d){
        if (d > ui->dau_time_high->value()){
            d = ui->dau_time_high->value();
        }
        ui->dateTimeEdit_3->setDate(t_start->day.addDays(d));
    });
    connect(ui->dau_time_high,&QSlider::valueChanged,[=](int d){
        if (d < ui->dau_time_low->value()){
            d = ui->dau_time_low->value();
        }
        ui->dateTimeEdit_4->setDate(t_start->day.addDays(d));
    });
    connect(ui->dateTimeEdit_3,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day > ui->dateTimeEdit_4->date()){
            day = ui->dateTimeEdit_4->date();
        }
        ui->dau_time_low->setValue(t_start->day.daysTo(day));
    });
    connect(ui->dateTimeEdit_4,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day < ui->dateTimeEdit_3->date()){
            day = ui->dateTimeEdit_3->date();
        }
        ui->dau_time_high->setValue(t_start->day.daysTo(day));
    });


// qml map connect
    //send start signal
    connect(this,SIGNAL(SendMapStart()),item,SIGNAL(start_map()));
    connect(this,SIGNAL(SendHeatMapStart()),item_heat,SIGNAL(start_heatmap()));
    //send center(user_id)
    connect(this,SIGNAL(SendMapLocation(double,double)),item,SIGNAL(rec_center(double,double)));
    connect(this,SIGNAL(SendHeatMapLocation(int, int)),item_heat,SIGNAL(rec_heatlocation(int, int)));

    //connect
    connect(ui->tra_str_sld,&QSlider::valueChanged,[=](int d){
        if (d > ui->tra_end_sld->value()){
            d = ui->tra_end_sld->value();
            ui->tra_str_sld->setSliderPosition(d);
        }
        ui->tra_str_edit->setDate(t_start->day.addDays(d));
    });
    connect(ui->tra_end_sld,&QSlider::valueChanged,[=](int d){
        if (d < ui->tra_str_sld->value()){
            d = ui->tra_str_sld->value();
        }
        ui->tra_end_edit->setDate(t_start->day.addDays(d));
    });
    connect(ui->tra_str_edit,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day > ui->tra_end_edit->date()){
            day = ui->tra_end_edit->date();
        }
        ui->tra_str_sld->setValue(t_start->day.daysTo(day));
    });
    connect(ui->tra_end_edit,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day < ui->tra_str_edit->date()){
            day = ui->tra_str_edit->date();
        }
        ui->tra_end_sld->setValue(t_start->day.daysTo(day));
    });


// Heat Map
    //connect
    connect(ui->heat_str_sld,&QSlider::valueChanged,[=](int d){
        if (d > ui->heat_end_sld->value()){
            d = ui->heat_end_sld->value();
            ui->heat_str_sld->setSliderPosition(d);
        }
        ui->heat_str_edit->setDate(t_start->day.addDays(d));
    });
    connect(ui->heat_end_sld,&QSlider::valueChanged,[=](int d){
        if (d < ui->heat_str_sld->value()){
            d = ui->heat_str_sld->value();
        }
        ui->heat_end_edit->setDate(t_start->day.addDays(d));
    });
    connect(ui->heat_str_edit,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day > ui->heat_end_edit->date()){
            day = ui->heat_end_edit->date();
        }
        ui->heat_str_sld->setValue(t_start->day.daysTo(day));
    });
    connect(ui->heat_end_edit,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day < ui->heat_str_edit->date()){
            day = ui->heat_str_edit->date();
        }
        ui->heat_end_sld->setValue(t_start->day.daysTo(day));
    });


// Similarity
    //connect
    connect(ui->sml_str_sld,&QSlider::valueChanged,[=](int d){
        if (d > ui->sml_end_sld->value()){
            d = ui->sml_end_sld->value();
            ui->sml_str_sld->setSliderPosition(d);
        }
        ui->sml_str_edit->setDate(t_start->day.addDays(d));
    });
    connect(ui->sml_end_sld,&QSlider::valueChanged,[=](int d){
        if (d < ui->sml_str_sld->value()){
            d = ui->sml_str_sld->value();
        }
        ui->sml_end_edit->setDate(t_start->day.addDays(d));
    });
    connect(ui->sml_str_edit,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day > ui->sml_end_edit->date()){
            day = ui->sml_end_edit->date();
        }
        ui->sml_str_sld->setValue(t_start->day.daysTo(day));
    });
    connect(ui->sml_end_edit,&QDateTimeEdit::dateChanged,[=](QDate day){
        if (day < ui->sml_str_edit->date()){
            day = ui->sml_str_edit->date();
        }
        ui->sml_end_sld->setValue(t_start->day.daysTo(day));
    });

}

Function::~Function()
{
    delete ui;
}

void Function::rec_dataset(Datas da){
    DataSet.append(da);
}

void Function::rec_data(QVector<Datas> d){
    DataSet = d;
    qDebug() << DataSet.size();
}

void Function::rec_finished(int a){
    if (a == -1){
        qDebug()<<"receive success";
        qDebug() << DataSet.size();
        start();
    }
}

void Function::start(){
    max_size = DataSet.size();
    bool fu[25000];
    bool fo[55000];
    for (int i=0; i<55000; i++) {
        if (i < 25000){
            fu[i] = false;
        }
        fo[i] = false;
    }

    ui->cot_user_Box1->addItem("All");
    ui->cot_user_Box2->addItem("None");
    ui->top_user_Box->addItem("All(no more than 10)");
    ui->Timestep->addItem("Daily");
    ui->Timestep->addItem("Ten Days");
    ui->Timestep->addItem("One Month");
    ui->Timestep->addItem("One Year");
    ui->locationBox->addItem("Select by Latitude & Longitude");

    for (int i=0; i<DataSet.size(); i++){
        if (!fu[DataSet.at(i).User_Id]){
            fu[DataSet.at(i).User_Id] = true;
            ui->top_user_Box->addItem(QString::number(DataSet.at(i).User_Id));
            ui->cot_user_Box1->addItem(QString::number(DataSet.at(i).User_Id));
            ui->cot_user_Box2->addItem(QString::number(DataSet.at(i).User_Id));
            ui->tra_user_box->addItem(QString::number(DataSet.at(i).User_Id));
            ui->sml_user_box->addItem(QString::number(DataSet.at(i).User_Id));
            ui->sml_user_box_2->addItem(QString::number(DataSet.at(i).User_Id));
        }
        if (!fo[DataSet.at(i).Location_Id]){
            fo[DataSet.at(i).Location_Id] = true;
            ui->locationBox->addItem(QString::number(DataSet.at(i).Location_Id));
        }
    }
    qDebug() << DataSet.at(DataSet.size()-1).User_Id;

    //Top10POIs();
    //ComprasionOfTop();
    //NumberOfCheckin();
    //DAU();
}

void Function::handleMarkerClicked()
{
    QLegendMarker* marker = qobject_cast<QLegendMarker*> (sender());
    Q_ASSERT(marker);
    switch (marker->type())
    {
        case QLegendMarker::LegendMarkerTypeXY:
        {
            //控序列隐藏/显示
            // Toggle visibility of series
            marker->series()->setVisible(!marker->series()->isVisible());

            // Turn legend marker back to visible, since hiding series also hides the marker
            // and we don't want it to happen now.
            marker->setVisible(true);

            //修改图例
            // Dim the marker, if series is not visible
            qreal alpha = 1.0;

            if (!marker->series()->isVisible())
                alpha = 0.5;

            QColor color;
            QBrush brush = marker->labelBrush();
            color = brush.color();
            color.setAlphaF(alpha);
            brush.setColor(color);
            marker->setLabelBrush(brush);

            brush = marker->brush();
            color = brush.color();
            color.setAlphaF(alpha);
            brush.setColor(color);
            marker->setBrush(brush);

            QPen pen = marker->pen();
            color = pen.color();
            color.setAlphaF(alpha);
            pen.setColor(color);
            marker->setPen(pen);
            break;
        }

        default:
        {
            qDebug() << "Unknown marker type";
            break;
        }
    }
}

void Function::PointHoverd(const QPointF &point, bool state){
    if (state) {
        ui->valuelabel->setText(QString::number(point.x()) + ", " + QString::number(point.y()));
        ui->valuelabel->show();
    }
    else{
        ui->valuelabel->hide();
    }
}

void Function::Top10POIs(){
    if(ui->chartTop->layout()){
        delete ui->chartTop->layout();
    }

    xTitle->clear();

    QVector<lct_cnt> u[55000];
    QVector<lct_cnt> u1[55000];
    QChart* chart = new QChart();
    QBarSeries* barTop = new QBarSeries;
    QBarSet *tmp_bar[10];
    chart->addSeries(barTop);
    t_start_top = new Time(ui->top_start_edit->date(),ui->top_start_edit->time());
    t_end_top = new Time(ui->top_end_edit->date(),ui->top_end_edit->time());

    int max_y = 50;
    int cnt = 0;
    int ans = 0;
    int id = DataSet.at(0).User_Id;

    if (top_single){
        id = top10_user;
    }

    for (int i=0; i<DataSet.size(); i++){
        int tmpid = DataSet.at(i).User_Id;
        if (DataSet.at(i).t.day <= t_end_top->day && DataSet.at(i).t.day >= t_start_top->day && ((top_single && tmpid == id) || (!top_single))){
            ans++;
            if (tmpid == id && ans != 1){
                lct_cnt tmp(DataSet.at(i).Location_Id,DataSet.at(i).latitude,DataSet.at(i).longitude);
                u[cnt].append(tmp);
            }
            else {
                if (ans != 1) {
                    cnt++;
                    if (cnt >= 10){
                        QMessageBox::warning(this, "Warning!", "Too many Users Fot Top10! (At most 10 Users)");
                        return;
                    }
                    if (!top_single) id = tmpid;
                }
                //u[cnt] = new QVector<lct_cnt>;
                //u1[cnt] = new QVector<lct_cnt>;
                //qDebug() << tmpid << DataSet.at(i).Location_Id;
                u[cnt].clear();
                u1[cnt].clear();
                tmp_bar[cnt] = new QBarSet("User" + QString::number(tmpid));
                barTop->append(tmp_bar[cnt]);
                if (ans == 1){
                    lct_cnt tmp(DataSet.at(i).Location_Id,DataSet.at(i).latitude,DataSet.at(i).longitude);
                    u[cnt].append(tmp);
                }
            }
        }
    }

    if (ans == 0) {
        QMessageBox::warning(this, "Warning!", "No user contains!");
        return;
    }

    for (int i=0; i<=cnt; i++){
        for (int j=0; j<u[i].size(); j++){
            int t = std::count(u1[i].begin(),u1[i].end(),u[i].at(j));
            if (t > 0)
                continue;

            else {
                int nt = std::count(u[i].begin(),u[i].end(),u[i].at(j));
                lct_cnt tmp(u[i].at(j).l_id,nt,u[i].at(j).la,u[i].at(j).lo);
                u1[i].append(tmp);
            }
        }
        std::sort(u1[i].begin(),u1[i].end());
        max_y = std::max(max_y,u1[i].at(0).l_cnt);

        for (int k=0; k<(std::min(100,u1[i].size())); k++){
            int s = u1[i].at(k).l_cnt;
            tmp_bar[i]->append(s);
            xTitle->append(QString::number(u1[i].at(k).l_id));
        }

    }

    //set xOy
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(*xTitle);
    chart->createDefaultAxes();
    int xmax = std::min(10,xTitle->size()-1);
    axisX->setRange(xTitle->at(0),xTitle->at(xmax));
    chart->setAxisX(axisX,barTop); //x_range
    if (!top_single) chart->axisX()->hide();

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0,max_y + 5);
    axisY->setTitleText("Times");
    axisY->setTickCount(10);
    axisY->setLabelFormat("%.1f");
    chart->setAxisY(axisY,barTop);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    barTop->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);  //设置柱状图标签显示的位置
    barTop->setLabelsVisible(true);


    //show
    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->QChartView::setRubberBand(QChartView::RectangleRubberBand);
    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(view, 1, 0);
    ui->chartTop->setLayout(baseLayout);
}

void Function::ComprasionOfTop(){
    for (int i=0; i<1000; i++){
        scatterSeries[i]->clear();
    }
    int tmp_size = cotchart->series().size();
    for (int i=0; i<tmp_size; i++){
        cotchart->removeSeries(cotchart->series().at(0));
    }
    if (ui->charts->layout()){
        delete ui->charts->layout();
    }

    QVector<lct_cnt> u[55000];
    QVector<lct_cnt> u1[55000];
    QColor c[1000];

    //QChart *chart = new QChart();

    t_start_cot = new Time(ui->cot_start_edit->date(),ui->cot_start_edit->time());
    t_end_cot = new Time(ui->cot_end_edit->date(),ui->cot_end_edit->time());

    float max_la = -90;
    float min_la = 90;
    float max_lo = -180;
    float min_lo = 180;
    int cnt = 0;
    int ans = 0;
    int id = DataSet.at(0).User_Id;

    if (cot_user_flag != 3){
        if (cot_user_flag == 2) id = std::min(cot_user[0],cot_user[1]);
        else if (cot_user_flag == 0) id = cot_user[0];
    }

    //qDebug() << cot_user_flag << cot_user[0] << cot_user[1] ;
    for (int i=0; i<DataSet.size(); i++){
        int tmpid = DataSet.at(i).User_Id;
        if (DataSet.at(i).t.day <= t_end_cot->day && DataSet.at(i).t.day >= t_start_cot->day &&
                ((cot_user_flag == 3) || (cot_user_flag == 2 && (tmpid == cot_user[0] || tmpid == cot_user[1])) ||
                 (cot_user_flag == 0 && tmpid == cot_user[0])) && ans < 10000){
            ans++;
            if (tmpid == id && ans != 1){
                lct_cnt tmp(DataSet.at(i).Location_Id,DataSet.at(i).latitude,DataSet.at(i).longitude);
                u[cnt].append(tmp);
            }
            else {
                if (ans != 1) {
                    cnt++;
                    id = tmpid;
                }
                //u[cnt] = new QVector<lct_cnt>;
                //u1[cnt] = new QVector<lct_cnt>;
                u[cnt].clear();
                u1[cnt].clear();
                if (ans == 1){
                    lct_cnt tmp(DataSet.at(i).Location_Id,DataSet.at(i).latitude,DataSet.at(i).longitude);
                    u[cnt].append(tmp);
                }

                for (int j=0; j<10; j++){
                    cotchart->addSeries(scatterSeries[cnt*10 +j]);
                    //connect(scatterSeries[cnt*10 + j],&QScatterSeries::hovered, this,&Function::PointHoverd);
                }

                QString name = "User " + QString::number(id,10);
                scatterSeries[cnt*10]->setName(name);
            }
        }
    }

    if (ans == 0) {
        QMessageBox::warning(this, "Warning!", "No user contains!");
        return;
    }

    else {
        for (int i=0; i<=cnt; i++){
            for (int j=0; j<u[i].size(); j++){
                int t = std::count(u1[i].begin(),u1[i].end(),u[i].at(j));
                if (t > 0)
                    continue;

                else {
                    int nt = std::count(u[i].begin(),u[i].end(),u[i].at(j));
                    lct_cnt tmp(u[i].at(j).l_id,nt,u[i].at(j).la,u[i].at(j).lo);
                    u1[i].append(tmp);
                }
            }
            std::sort(u1[i].begin(),u1[i].end());

            //set color
            switch (i){
            case 0: c[i].setRgb(211, 47, 47); break;
            case 1: c[i].setRgb(123, 31, 162); break;
            case 2: c[i].setRgb(25, 118, 210); break;
            case 3: c[i].setRgb(0, 131, 143); break;
            case 4: c[i].setRgb(85, 139, 47); break;
            case 5: c[i].setRgb(158, 157, 36); break;
            case 6: c[i].setRgb(249, 168, 37); break;
            case 7: c[i].setRgb(255, 111, 0); break;
            case 8: c[i].setRgb(255, 112, 67); break;
            default: c[i].setRgb((c[i%9].red()+10)%255,(c[i%9].blue()+20)%255,(c[i%9].green()+30)%255); break;
            }

            if (u1[i].size() == 0){
                scatterSeries[i*10]->clear();
                continue;
            }

            for (int k=0; k<(std::min(10,u1[i].size())); k++){
                int s = u1[i].at(k).l_cnt;
                scatterSeries[i*10 + k]->append(u1[i].at(k).la,u1[i].at(k).lo);
                scatterSeries[i*10 + k]->setMarkerSize(s*5); // 设置节点大小
                c[i].setAlphaF(0.5);
                scatterSeries[i*10 + k]->setColor(c[i]);
                max_la = std::max(max_la,u1[i].at(k).la);
                min_la = std::min(min_la,u1[i].at(k).la);
                max_lo = std::max(max_lo,u1[i].at(k).lo);
                min_lo = std::min(min_lo,u1[i].at(k).lo);
            }
        }

        //view
        cotchart->setAnimationOptions(QChart::GridAxisAnimations);
        cotchart->createDefaultAxes();
        cotchart->axes(Qt::Horizontal).first()->setRange(min_la-10, max_la+10);// x轴范围
        cotchart->axes(Qt::Vertical).first()->setRange(min_lo-20, max_lo+20);// y轴范围
        cotchart->axes(Qt::Horizontal).first()->setTitleText("Latitude");
        cotchart->axes(Qt::Vertical).first()->setTitleText("Longitude");

        // some settings
        QValueAxis *axisY = qobject_cast<QValueAxis*>(cotchart->axes(Qt::Vertical).first());
        Q_ASSERT(axisY);
        axisY->setLabelFormat("%.1f  ");

        const auto markers = cotchart->legend()->markers();
        for (QLegendMarker *marker : markers) {
            QObject::disconnect(marker, &QLegendMarker::clicked, this ,&Function::handleMarkerClicked);
            QObject::connect(marker, &QLegendMarker::clicked, this, &Function::handleMarkerClicked);
        }

        cotchart->legend()->setVisible(true);
        if (cot_user_flag == 3){
            cotchart->legend()->setVisible(false);
        }

        //show
        QChartView *view = new QChartView(cotchart);
        view->setRenderHint(QPainter::Antialiasing);
        view->QChartView::setRubberBand(QChartView::RectangleRubberBand);
        QGridLayout *baseLayout = new QGridLayout();
        baseLayout->addWidget(view, 1, 0);
        ui->charts->setLayout(baseLayout);
    }
}

void Function::NumberOfCheckin(){
    if (ui->ckinchart->layout()){
        delete ui->ckinchart->layout();
    }
    t_start_ckin = new Time(ui->dateTimeEdit->date(),ui->dateTimeEdit->time());
    t_end_ckin = new Time(ui->dateTimeEdit_2->date(),ui->dateTimeEdit_2->time());
    QDate t_min, t_max;
    t_min.setDate(2009,2,4);
    t_max.setDate(2010,10,23);
    int cnt[700];

    for (int i=0; i<700; i++) {
        cnt[i] = 0;
    }

    for (int i=0; i<DataSet.size(); i++){
        if (((ckin_id_flag && DataSet.at(i).Location_Id == ckin_loid) ||
             (!ckin_id_flag && DataSet.at(i).latitude >= ckin_la_low && DataSet.at(i).latitude <= ckin_la_high &&
                               DataSet.at(i).longitude >= ckin_lo_low && DataSet.at(i).longitude <= ckin_lo_high)) &&
             DataSet.at(i).t.day <= t_end_ckin->day && DataSet.at(i).t.day >= t_start_ckin->day){
            int tmp = t_start_ckin->day.daysTo(DataSet.at(i).t.day);
            cnt[tmp]++;
        }
    }

    QChart *chart = new QChart();
    QLineSeries* line1 = new QLineSeries;
    QDateTimeAxis* x = new QDateTimeAxis;
    QValueAxis* y = new QValueAxis;


    //add point
    for(int i = 0 ;i < t_start_ckin->day.daysTo(t_end_ckin->day);i++){
        QDateTime t;
        t.setDate(t_start_ckin->day.addDays(i));
        t.setTime(QTime(0,0,0));
        line1->append(t.toMSecsSinceEpoch(), cnt[i]);
    }
    chart->addSeries(line1);

    //set xOy
    chart->setAnimationOptions(QChart::AllAnimations);
    x->setFormat("yyyy.MM.dd");
    x->setTitleText("Date");
    y->setTitleText("Times");
    chart->addAxis(x, Qt::AlignBottom);
    chart->addAxis(y,Qt::AlignLeft);
    line1->attachAxis(x);
    line1->attachAxis(y);
    chart->setBackgroundVisible(true);
    chart->setContentsMargins(0, 0, 0, 0);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundRoundness(0);
    line1->setPointsVisible(true);

    //show
    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->QChartView::setRubberBand(QChartView::RectangleRubberBand);
    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(view, 1, 0);
    ui->ckinchart->setLayout(baseLayout);
}


void Function::DAU(){
    if(ui->DAU_1->layout()){
        delete ui->DAU_1->layout();
    }

    if (ui->DAU_2->layout()){
        delete ui->DAU_2->layout();
    }

    if (ui->DAU_3->layout()){
        delete ui->DAU_3->layout();
    }


    QVector<int> lo_u[55000];
    t_start_dau = new Time(ui->dateTimeEdit_3->date(),ui->dateTimeEdit_3->time());
    t_end_dau = new Time(ui->dateTimeEdit_4->date(),ui->dateTimeEdit_4->time());

    int cnt1[55000];
    int cnt2[55000];
    int ans1 = 0;
    int ans2 = 0;
    int user1[25000];
    int user2[25000];
    int cntline1[700];
    int cntline2[700];
    bool visited1[55000];
    bool visited2[55000];
    bool number_contain = false;

    for (int i=0; i<700; i++) {
        cntline1[i] = 0;
        cntline2[i] = 0;
    }

    for (int i=0; i<55000; i++){
        if (i<25000){
            user1[i] = 0;
            user2[i] = 0;
        }
        cnt1[i] = 0;
        cnt2[i] = 0;
        visited1[i] = false;
        visited2[i] = false;
        lo_u[i].clear();
    }

    for (int i=0; i<DataSet.size(); i++){
        Datas tmp = DataSet.at(i);
        if (tmp.t.day <= t_start_dau->day.addDays(timestep*step) && tmp.t.day >= t_start_dau->day.addDays(timestep*(step-1))){
            number_contain = true;
            lo_u[tmp.Location_Id].append(tmp.User_Id);
            int tmp_d = t_start_dau->day.daysTo(DataSet.at(i).t.day);
            if (tmp.Location_Id == DAU_loid1) cntline1[tmp_d]++;
            if (tmp.Location_Id == DAU_loid2) cntline2[tmp_d]++;
        }
    }

    if (number_contain){
        ui->warning_text->hide();
        QChart *chart1 = new QChart();
        QChart *chart2 = new QChart();
        QChart *chart3 = new QChart();

        QPieSeries *series1 = new QPieSeries(chart1);
        QPieSeries *series2 = new QPieSeries(chart2);
        QLineSeries* line1 = new QLineSeries(chart3);
        QLineSeries* line2 = new QLineSeries(chart3);

        //qDebug() << DAU_loid1 << ' ' << DAU_loid2;

        //set location1
        for (int i=0; i<lo_u[DAU_loid1].size(); i++){
            if (!visited1[lo_u[DAU_loid1].at(i)]){
                visited1[lo_u[DAU_loid1].at(i)] = true;
                user1[ans1++] = lo_u[DAU_loid1].at(i);
            }
            cnt1[lo_u[DAU_loid1].at(i)]++;
        }

        for (int i=0; i<ans1; i++){
            if (cnt1[user1[i]] > (ans1/30)){
                QString name = QString::number(user1[i]);
                series1->append(name,cnt1[user1[i]]);
            }

        }

        //set location2
        for (int i=0; i<lo_u[DAU_loid2].size(); i++){
            if (!visited2[lo_u[DAU_loid2].at(i)]){
                visited2[lo_u[DAU_loid2].at(i)] = true;
                user2[ans2++] = lo_u[DAU_loid2].at(i);
            }
            cnt2[lo_u[DAU_loid2].at(i)]++;
        }

        for (int i=0; i<ans2; i++){
            if (cnt2[user2[i]] > (ans2/30)){
                QString name = QString::number(user2[i]);
                series2->append(name,cnt2[user2[i]]);
            }
        }

        //set line
        QDateTimeAxis* x = new QDateTimeAxis;
        QValueAxis* y = new QValueAxis;

        //add point
        for(int i = 0 ;i < t_start_dau->day.daysTo(t_end_dau->day);i++){
            QDateTime t;
            t.setDate(t_start_dau->day.addDays(i));
            t.setTime(QTime(0,0,0));
            line1->append(t.toMSecsSinceEpoch(), cntline1[i]);
            line2->append(t.toMSecsSinceEpoch(), cntline2[i]);
        }
        chart3->addSeries(line1);
        chart3->addSeries(line2);

        //set xOy
        chart3->setAnimationOptions(QChart::AllAnimations);
        x->setFormat("yyyy.MM.dd");
        x->setTitleText("Date");
        y->setTitleText("Times");
        chart3->addAxis(x, Qt::AlignBottom);
        chart3->addAxis(y, Qt::AlignLeft);
        line1->attachAxis(x);
        line2->attachAxis(x);
        line1->attachAxis(y);
        line2->attachAxis(y);


        chart1->addSeries(series1);
        chart2->addSeries(series2);
        chart1->legend()->setVisible(false);
        chart2->legend()->setVisible(false);
        chart3->legend()->setVisible(true);

        //饼图中间空心的比例
        //series1->setHoleSize(0.25);
        //series2->setHoleSize(0.25);
        series1->setName("Location " + QString::number(DAU_loid1));
        series2->setName("Location " + QString::number(DAU_loid2));
        series1->setLabelsVisible(true);
        series2->setLabelsVisible(true);

        //show
        QChartView *view1 = new QChartView(chart1);
        QChartView *view2 = new QChartView(chart2);
        QChartView *view3 = new QChartView(chart3);
        view3->setRenderHint(QPainter::Antialiasing);
        view3->QChartView::setRubberBand(QChartView::RectangleRubberBand);
        QGridLayout *baseLayout1 = new QGridLayout();
        QGridLayout *baseLayout2 = new QGridLayout();
        QGridLayout *baseLayout3 = new QGridLayout();
        baseLayout1->addWidget(view1, 1, 0);
        baseLayout2->addWidget(view2, 1, 0);
        baseLayout3->addWidget(view3, 1, 0);
        ui->DAU_1->setLayout(baseLayout1);
        ui->DAU_2->setLayout(baseLayout2);
        ui->DAU_3->setLayout(baseLayout3);
    }
    else {
        ui->warning_text->show();
    }

}

void Function::Trajectory(){
    if (ui->mapWidget->layout()){
        delete ui->mapWidget->layout();
    }
    t_start_tra = new Time(ui->tra_str_edit->date(),ui->tra_str_edit->time());
    t_end_tra = new Time(ui->tra_end_edit->date(),ui->tra_end_edit->time());
    emit SendMapStart();
    int cnt = 0;
    for (int i=0; i<DataSet.size(); i++){
        float la = DataSet.at(i).latitude;
        float lo = DataSet.at(i).longitude;
        double la1 = la;
        double lo1 = lo;
        if (DataSet.at(i).User_Id == tra_user &&
            DataSet.at(i).t.day >= t_start_tra->day && DataSet.at(i).t.day <= t_end_tra->day &&
            cnt < 200){
            cnt++;
            Delay_MSec(500);
            emit SendMapLocation(la1, lo1);
        }
    }
}

void Function::HeatOfMap(){
    if (ui->heatmapWidget->layout()){
        delete ui->heatmapWidget->layout();
    }
    for(int i=0; i<3000; i++){
        piece[i] = 0;
    }
    t_start_heat = new Time(ui->heat_str_edit->date(),ui->heat_str_edit->time());
    t_end_heat = new Time(ui->heat_end_edit->date(),ui->heat_end_edit->time());
    emit SendHeatMapStart();
    for (int i=0; i<DataSet.size(); i++){
        Datas tmp = DataSet.at(i);
        float la = -tmp.latitude/5 + 18;
        float lo = -tmp.longitude/5 + 36;

        int row = la;
        int col = lo;
        int index = row * 72 + col;

        //qDebug () << tmp.latitude << tmp.longitude << index << row << col;
        if (tmp.t.day <= t_end_heat->day && tmp.t.day >= t_start_heat->day){
            piece[index]++;
        }
    }
    for (int i=0; i<2592; i++){
        //qDebug() << i << piece[i] << endl;
        emit SendHeatMapLocation(i, piece[i]);
    }
}

void Function::SimilarityOfUsers(){
    for (int i=0; i<3000; i++){
        u1_piece[i] = 0;
        u2_piece[i] = 0;
    }

    for (int i=0; i<2592; i++){
        smlpieces1[i]->clear();
        smlpieces2[i]->clear();
    }
    int tmp_size = smlchart->series().size();
    for (int i=0; i<tmp_size; i++){
        smlchart->removeSeries(smlchart->series().at(0));
    }

    if (ui->sml_chart1->layout()){
        delete ui->sml_chart1->layout();
    }

    t_start_sml = new Time(ui->sml_str_edit->date(),ui->sml_str_edit->time());
    t_end_sml = new Time(ui->sml_end_edit->date(),ui->sml_end_edit->time());

    double sml1 = 0, sml2 = 0, sml  = 0;
    double inner=0, length1=0, length2=0;
    double ans1=0, ans2=0;
    int u1_max = 50, u2_max = 50;
    QColor c1[3000];
    QColor c2[3000];

    /////////for debug
    //sml_user1 = 3;
    //sml_user2 = 5;

    for (int i=0; i<DataSet.size(); i++){
        Datas tmp = DataSet.at(i);
        if (tmp.t.day <= t_end_sml->day && tmp.t.day >= t_start_sml->day){
            if (tmp.User_Id == sml_user1){
                int row = -tmp.latitude/5 + 18;
                int col = -tmp.longitude/5 + 36;
                int index = row * 72 + col;
                u1_piece[index]++;
                u1_max = std::max(u1_max, u1_piece[index]);
            }
            if (tmp.User_Id == sml_user2){
                int row = -tmp.latitude/5 + 18;
                int col = -tmp.longitude/5 + 36;
                int index = row * 72 + col;
                u2_piece[index]++;
                u2_max = std::max(u2_max, u2_piece[index]);
            }
        }
    }

    for (int i=0; i<2592; i++){
        inner += u1_piece[i] * u2_piece[i];
        ans1 += u1_piece[i] * u1_piece[i];
        ans2 += u2_piece[i] * u2_piece[i];

        if (u1_piece[i] >0)
            smlpieces1[i]->append((-i/72+17)*5,(-(i-(i/72)*72)+35)*5);
        if (u2_piece[i] >0)
            smlpieces2[i]->append((-i/72+17)*5,(-(i-(i/72)*72)+35)*5);

        c1[i].setRgb(255, 111, 0);
        c2[i].setRgb(104, 159, 56);
        double alpha1 = std::min(1.0,(5*u1_piece[i]/u1_max + 0.1));
        double alpha2 = std::min(1.0,(5*u2_piece[i]/u1_max + 0.1));
        //if (u1_piece[i] == 0) alpha1 = 0.02;
        //if (u2_piece[i] == 0) alpha2 = 0.05;
        c1[i].setAlphaF(alpha1);
        c2[i].setAlphaF(alpha2);
        //qDebug() << alpha1 << alpha2;

        smlpieces1[i]->setColor(c1[i]);
        smlpieces2[i]->setColor(c2[i]);
        smlpieces1[i]->setMarkerSize(30);
        smlpieces2[i]->setMarkerSize(30);
        smlpieces1[i]->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
        smlpieces2[i]->setMarkerShape(QScatterSeries::MarkerShapeRectangle);

        smlchart->addSeries(smlpieces1[i]);
        smlchart->addSeries(smlpieces2[i]);
    }
    length1 = std::sqrt(ans1);
    length2 = std::sqrt(ans2);

    sml1 = inner/(length1*length2);
    sml2 = std::min(length1,length2)/std::max(length1,length2);
    sml = 0.7*sml1 + 0.3*sml2;
    ui->lsml_label->setText(QString::number(sml1));
    ui->vsml_label->setText(QString::number(sml2));
    ui->gsml_label->setText(QString::number(sml));

    cotchart->setAnimationOptions(QChart::GridAxisAnimations);
    smlchart->createDefaultAxes();
    smlchart->axes(Qt::Horizontal).first()->setRange(-60, 90);// x轴范围
    smlchart->axes(Qt::Vertical).first()->setRange(-180, 40);// y轴范围
    smlchart->axes(Qt::Horizontal).first()->setTitleText("Latitude");
    smlchart->axes(Qt::Vertical).first()->setTitleText("Longitude");
    smlchart->legend()->setVisible(false);

    //show
    QChartView *view = new QChartView(smlchart);
    view->setRenderHint(QPainter::Antialiasing);
    view->QChartView::setRubberBand(QChartView::RectangleRubberBand);
    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(view, 1, 0);
    ui->sml_chart1->setLayout(baseLayout);
}

void Function::on_cot_user_Box1_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "All"){
        cot_user_flag = 3;
    }
    else {
        cot_user[0] = arg1.toInt();
        if (cot_user[1] == -1) cot_user_flag = 0;
        else if (cot_user[1] >= 0) cot_user_flag = 2;
    }
    ComprasionOfTop();
}

void Function::on_cot_user_Box2_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "None"){
        if(cot_user_flag != 3 && cot_user[0] != -1){
            cot_user_flag = 0;
        }
    }
    else {
        cot_user[1] = arg1.toInt();
        if (cot_user_flag != 3) cot_user_flag = 2;
    }
    ComprasionOfTop();
}

void Function::on_dau_lo1_textEdited(const QString &arg1)
{
    DAU_loid1 = arg1.toInt();
    DAU();
}

void Function::on_dau_lo2_textEdited(const QString &arg1)
{
    DAU_loid2 = arg1.toInt();
    DAU();
}

void Function::on_top_user_Box_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "All(no more than 10)"){
        top_single = false;
    }
    else {
        top_single = true;
        top10_user = arg1.toInt();
    }
    Top10POIs();
}


void Function::on_locationBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Select by Latitude & Longitude"){
        ckin_id_flag = false;
    }

    else {
        ckin_id_flag = true;
        ckin_loid = arg1.toInt();
        NumberOfCheckin();
    }
}

void Function::on_step_plus_clicked()
{
    if (step*timestep > 626) {
        QMessageBox::warning(this, "Warning!", "No more than 626 days!");
        return;
    }
    step++;
    ui->stepEdit->setText(QString::number(step));
    DAU();
}
void Function::on_step_minus_clicked()
{
    if (step*timestep > 626) {
        QMessageBox::warning(this, "Warning!", "No more than 626 days!");
        return;
    }
    if (step <= 0) {
        QMessageBox::warning(this, "Warning!", "step can't be negative");
        return;
    }
    step--;
    ui->stepEdit->setText(QString::number(step));
    DAU();
}

void Function::on_Timestep_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "Daily") timestep = 1;
    if (arg1 == "Ten Days") timestep = 10;
    if (arg1 == "One Month") timestep = 30;
    step = 1;
    ui->stepEdit->setText("1");

    if (arg1 == "One Year"){
        timestep = 365;
        step = 0;
        ui->stepEdit->setText("0");
    }
}

void Function::on_stepEdit_textChanged(const QString &arg1)
{
    step = arg1.toInt();
}

void Function::on_tra_user_box_currentIndexChanged(const QString &arg1)
{
    tra_user = arg1.toInt();
}

void Function::on_cot_ok_clicked()
{
    ComprasionOfTop();
}

void Function::on_top_ok_clicked()
{
    Top10POIs();
}

void Function::on_ckin_ok_clicked()
{
    NumberOfCheckin();
}

void Function::on_dau_ok_clicked()
{
    DAU();
}

void Function::on_pushButton_clicked()
{
    Trajectory();
}

void Function::on_heat_ok_clicked()
{
    HeatOfMap();
}

void Function::on_sml_ok_clicked()
{
    SimilarityOfUsers();
}

void Function::Delay_MSec(unsigned int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
}

void Function::on_sml_user_box_currentIndexChanged(const QString &arg1)
{
    sml_user1 = arg1.toInt();
}

void Function::on_sml_user_box_2_currentIndexChanged(const QString &arg1)
{
    sml_user2 = arg1.toInt();
}


