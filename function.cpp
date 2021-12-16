#include "function.h"
#include "ui_function.h"
#include "QtCharts"
#include "iostream"
#include "cmath"

struct lct_cnt;
Function::Function(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Function)
{
    qRegisterMetaType<QVector<Datas>>("QVector<Datas>");
    ui->setupUi(this);
    connect(ui->confirm,SIGNAL(clicked()),this,SLOT(start()));
}

Function::~Function()
{
    delete ui;
}

void Function::rec_dataset(Datas da){
    DataSet.append(da);
}

void Function::start(){
    //NumberOfCheckin();
    DAU();
}

void Function::test(){
    // scatter chart
        QChart *chart = new QChart();
        chart->setTitle("Scatter chart");

        QScatterSeries *scatterSeries1 = new QScatterSeries(chart);
        QScatterSeries *scatterSeries2 = new QScatterSeries(chart);

        chart->addSeries(scatterSeries1);
        chart->addSeries(scatterSeries2);

        // add series
        scatterSeries1->setName("A店铺接单数");
        scatterSeries1->setPointLabelsFormat("@yPoint");
        scatterSeries1->setPointLabelsVisible();
        scatterSeries1->setMarkerSize(16); // 设置节点大小

        scatterSeries2->setName("B店铺接单数");
        //scatterSeries2->setPointLabelsFormat("@yPoint");
        scatterSeries2->setPointLabelsVisible();
        scatterSeries2->setMarkerSize(16);

        scatterSeries1->append(0,6);
        scatterSeries1->append(1,10);
        scatterSeries1->append(4,12);
        scatterSeries1->append(6,5);
        scatterSeries2->append(0,18);
        scatterSeries2->append(3,13);
        scatterSeries2->append(5,7);
        scatterSeries2->append(6,2);

        const auto markers = chart->legend()->markers();
        for (QLegendMarker *marker : markers) {
            QObject::disconnect(marker, &QLegendMarker::clicked, this ,&Function::handleMarkerClicked);
            QObject::connect(marker, &QLegendMarker::clicked, this, &Function::handleMarkerClicked);
        }

        //view
        chart->setAnimationOptions(QChart::AllAnimations);
        chart->createDefaultAxes();//设置网格线
        chart->axes(Qt::Horizontal).first()->setRange(-5, 7);// x轴范围
        chart->axes(Qt::Vertical).first()->setRange(-5, 20);// y轴范围

        // some settings
        QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
        Q_ASSERT(axisY);
        axisY->setLabelFormat("%.1f  ");

        //show
        QChartView *view = new QChartView(chart);
        //view->setRenderHint(QPainter::Antialiasing);
        QGridLayout *baseLayout = new QGridLayout();
        baseLayout->addWidget(view, 1, 0);
        ui->charts->setLayout(baseLayout);
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
        qDebug() << " zhixing " << endl;
        ui->valueLabel->setText("hello");

        QPoint curPos = mapFromGlobal(QCursor::pos());
        //ui->valueLabel->move(curPos.x(), curPos.y());
        //qDebug() << ui->valueLabel->x() << " "<<endl;

        ui->valueLabel->show();
    }
    else
        ui->valueLabel->hide();
}

void Function::linetest(){
    chart = new QChart();
    mAxY = new QValueAxis();
    mAxX = new QValueAxis();
    mLineSeries = new QLineSeries();

    //set x&y range
    mAxY->setRange(0, 10);
    mAxY->setTickCount(11);
    mAxX->setRange(0,10);
    mAxX->setTickCount(11);

    // add series
    chart->addSeries(mLineSeries);
    chart->setTheme(QtCharts::QChart::ChartThemeBrownSand);

    //title
    mAxX->setTitleText(QString(tr("ImageNumber")));
    mAxY->setTitleText(QString(tr("ReadRate(%)")));
    chart->addAxis(mAxY, Qt::AlignLeft);
    chart->addAxis(mAxX, Qt::AlignBottom);
    mLineSeries->attachAxis(mAxY);
    mLineSeries->attachAxis(mAxX);

    //some settings
    chart->setBackgroundVisible(false);
    chart->setContentsMargins(0, 0, 0, 0);
    chart->setMargins(QMargins(0, 0, 0, 0));
    chart->setBackgroundRoundness(0);
    mLineSeries->setPointsVisible(true);

    //legend
    QLegend *mlegend = chart->legend();
    mLineSeries->setName("testname");
    mLineSeries->setColor(QColor(255,0,0));
    mlegend->setAlignment(Qt::AlignBottom);
    mlegend->show();

    //add point
    for(int i = 0 ;i < 100;i++){
        mLineSeries->append(i+1, i+i%4);
    }

    //hide
    const auto markers = chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        QObject::disconnect(marker, &QLegendMarker::clicked, this ,&Function::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &Function::handleMarkerClicked);
    }

    //show
    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->resize(2000,2000);

    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(view, 1, 0);
    ui->charts->setLayout(baseLayout);
}

void Function::Top10POIs(){
    QVector<lct_cnt> * u[100];
    QVector<lct_cnt> * u1[100];
    QChart *chart = new QChart();
    QScatterSeries *scatterSeries[100];

    float max_la = -90;
    float min_la = 90;
    float max_lo = -180;
    float min_lo = 180;
    int cnt = 0;
    int id = DataSet.at(0).User_Id;

    u[0] = new QVector<lct_cnt>;
    u1[0] = new QVector<lct_cnt>;
    scatterSeries[0] = new QScatterSeries(chart);
    scatterSeries[0]->setMarkerSize(40);
    chart->addSeries(scatterSeries[0]);
    connect(scatterSeries[cnt],&QScatterSeries::hovered, this,&Function::PointHoverd);

    QString name = "User" + QString::number(id,10);
    //int lo = DataSet.at(i).Location_Id;
    scatterSeries[0]->setName(name);


    for (int i=0; i<DataSet.size(); i++){
        int tmpid = DataSet.at(i).User_Id;
        if (tmpid == id){
            lct_cnt tmp(DataSet.at(i).Location_Id,DataSet.at(i).latitude,DataSet.at(i).longitude);
            u[cnt]->append(tmp);
        }
        else {
            cnt++;
            id = tmpid;
            u[cnt] = new QVector<lct_cnt>;
            u1[cnt] = new QVector<lct_cnt>;
            scatterSeries[cnt] = new QScatterSeries(chart);
            chart->addSeries(scatterSeries[cnt]);

            QString name = "User " + QString::number(id,10);
            //QString lo = QString::number(DataSet.at(i).Location_Id,10);

            scatterSeries[cnt]->setName(name);
            //scatterSeries[i]->setPointLabelsFormat(lo);
            //scatterSeries[i]->setPointLabelsVisible();
            scatterSeries[cnt]->setMarkerSize(40); // 设置节点大小
            connect(scatterSeries[cnt],&QScatterSeries::hovered, this,&Function::PointHoverd);
        }
    }

    for (int i=0; i<=cnt; i++){
        for (int j=0; j<u[i]->size(); j++){
            int t = std::count(u1[i]->begin(),u1[i]->end(),u[i]->at(j));
            if (t > 0)
                continue;

            else {
                int nt = std::count(u[i]->begin(),u[i]->end(),u[i]->at(j));
                lct_cnt tmp(u[i]->at(j).l_id,nt,u[i]->at(j).la,u[i]->at(j).lo);
                u1[i]->append(tmp);
            }
        }
        std::sort(u1[i]->begin(),u1[i]->end());
        for (int k=0; k<(std::min(10,u1[i]->size())); k++){
            scatterSeries[i]->append(u1[i]->at(k).la,u1[i]->at(k).lo);
            max_la = std::max(max_la,u1[i]->at(k).la);
            min_la = std::min(min_la,u1[i]->at(k).la);
            max_lo = std::max(max_lo,u1[i]->at(k).lo);
            min_lo = std::min(min_lo,u1[i]->at(k).lo);
        }
    }

    //view
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->createDefaultAxes();//设置网格线
    chart->axes(Qt::Horizontal).first()->setRange(min_la-5, max_la+5);// x轴范围
    chart->axes(Qt::Vertical).first()->setRange(min_lo-5, max_lo+5);// y轴范围

    // some settings
    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    Q_ASSERT(axisY);
    axisY->setLabelFormat("%.1f  ");

    const auto markers = chart->legend()->markers();
    for (QLegendMarker *marker : markers) {
        QObject::disconnect(marker, &QLegendMarker::clicked, this ,&Function::handleMarkerClicked);
        QObject::connect(marker, &QLegendMarker::clicked, this, &Function::handleMarkerClicked);
    }

    //show
    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->QChartView::setRubberBand(QChartView::RectangleRubberBand);
    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(view, 1, 0);
    ui->charts->setLayout(baseLayout);
}

void Function::NumberOfCheckin(){
    Time t_start(ui->dateTimeEdit->date(),ui->dateTimeEdit->time());
    Time t_end(ui->dateTimeEdit_2->date(),ui->dateTimeEdit_2->time());
    QDate t_min, t_max;
    t_min.setDate(2010,1,1);
    t_max.setDate(2010,12,31);
    int cnt[1000];

    for (int i=0; i<1000; i++) {
        cnt[i] = 0;
    }

    ckin_loid = ui->lineEdit->text().toInt();
    for (int i=0; i<DataSet.size(); i++){
        if (DataSet.at(i).t.day <= t_end.day && DataSet.at(i).t.day >= t_start.day && DataSet.at(i).Location_Id == ckin_loid){
            //qDebug() << DataSet.at(i).t.day << DataSet.at(i).User_Id << " " << DataSet.at(i).Location_Id;
            int tmp = t_start.day.daysTo(DataSet.at(i).t.day);
            cnt[tmp]++;
            //qDebug() << tmp << " " << cnt[tmp];
        }
    }

    QChart *chart = new QChart();
    QLineSeries* line1 = new QLineSeries;
    QDateTimeAxis* x = new QDateTimeAxis;
    QValueAxis* y = new QValueAxis;

    //
    QLegend *mlegend = chart->legend();
    mlegend->setAlignment(Qt::AlignBottom);
    mlegend->show();

    //add point
    for(int i = 0 ;i < t_start.day.daysTo(t_end.day);i++){
        QDateTime t;
        t.setDate(t_start.day.addDays(i));
        t.setTime(QTime(0,0,0));
        line1->append(t.toMSecsSinceEpoch(), cnt[i]);
        //qDebug() << line1->at(i).x() << " " << line1->at(i).y();
    }
    chart->addSeries(line1);

    //set xOy
    chart->setAnimationOptions(QChart::AllAnimations);
    x->setFormat("yyyy.MM.dd");
    chart->addAxis(x, Qt::AlignBottom);
    chart->addAxis(y,Qt::AlignLeft);
    line1->attachAxis(x);
    line1->attachAxis(y);

    //show
    QChartView *view = new QChartView(chart);
    view->setRenderHint(QPainter::Antialiasing);
    view->QChartView::setRubberBand(QChartView::RectangleRubberBand);
    QGridLayout *baseLayout = new QGridLayout();
    baseLayout->addWidget(view, 1, 0);
    ui->ckinchart->setLayout(baseLayout);
}

void Function::checkNumberOfCheckin(){
    QChart *chart = new QChart();
    QLineSeries* mLineSeries = new QLineSeries();

    chart->addSeries(mLineSeries);

}


void Function::DAU(){
    QVector<int>* lo_u[1000];
    Time t_start(ui->dateTimeEdit_3->date(),ui->dateTimeEdit_3->time());
    Time t_end(ui->dateTimeEdit_4->date(),ui->dateTimeEdit_4->time());
    int index_lo[10000];
    int lo_index[10000];
    int cnt[10000];
    int crt_index = -1;
    int ans1 = 0;
    int ans2 = 0;
    int user1[10000];
    int user2[10000];
    bool flag[10000];
    bool visited[10000];

    for (int i=0; i<10000; i++){
        flag[i] = false;
        cnt[i] = 0;
        visited[i] = false;
        user1[i] = 0;
        user2[i] = 0;
    }

    for (int i=0; i<DataSet.size(); i++){
        Datas tmp = DataSet.at(i);

        if (tmp.t <= t_end && tmp.t >= t_start){
            if (flag[tmp.Location_Id] == false){
                flag[tmp.Location_Id] = true;
                ui->locationBox->addItem(QString::number(tmp.Location_Id,10));

                index_lo[++crt_index] = tmp.Location_Id;
                lo_index[tmp.Location_Id] = crt_index;

                lo_u[lo_index[tmp.Location_Id]] = new QVector<int>;
                lo_u[lo_index[tmp.Location_Id]]->append(tmp.User_Id);

                //qDebug() << index_lo[crt_index] << " " << crt_index;
            }
            else {
                lo_u[lo_index[tmp.Location_Id]]->append(tmp.User_Id);
            }
        }
    }

    QChart *chart1 = new QChart();
    QChart *chart2 = new QChart();

    QPieSeries *series1 = new QPieSeries(chart1);
    QPieSeries *series2 = new QPieSeries(chart2);

    DAU_loid1 = 32;
    DAU_loid2 = 520;

    //set location1
    for (int i=0; i<lo_u[lo_index[DAU_loid1]]->size(); i++){
        if (!visited[lo_u[lo_index[DAU_loid1]]->at(i)]){
            visited[lo_u[lo_index[DAU_loid1]]->at(i)] = true;
            user1[ans1++] = lo_u[lo_index[DAU_loid1]]->at(i);
        }
        cnt[lo_u[lo_index[DAU_loid1]]->at(i)]++;
        //qDebug()<<cnt[lo_u[lo_index[DAU_loid1]]->at(i)] ;
    }

    for (int i=0; i<ans1; i++){
        QString name = "User " + QString::number(user1[i]);
        series1->append(name,cnt[user1[i]]);
        //qDebug()<< i << " " << cnt[user1[i]] ;
    }

    //set location2
    for (int i=0; i<lo_u[lo_index[DAU_loid2]]->size(); i++){
        if (!visited[lo_u[lo_index[DAU_loid2]]->at(i)]){
            visited[lo_u[lo_index[DAU_loid2]]->at(i)] = true;
            user2[ans2++] = lo_u[lo_index[DAU_loid2]]->at(i);
        }
        cnt[lo_u[lo_index[DAU_loid2]]->at(i)]++;
    }

    for (int i=0; i<ans2; i++){
        QString name = "User " + QString::number(user2[i]);
        series2->append(name,cnt[user2[i]]);
        //qDebug()<< i << " " << cnt[user2[i]] ;
    }

    chart1->addSeries(series1);
    chart2->addSeries(series2);

    //series1->setHoleSize(0.25);//饼图中间空心的比例
    //series2->setHoleSize(0.25);

    //show
    QChartView *view1 = new QChartView(chart1);
    QChartView *view2 = new QChartView(chart2);
    //view1->setRenderHint(QPainter::Antialiasing);
    //view2->setRenderHint(QPainter::Antialiasing);
    //view1->QChartView::setRubberBand(QChartView::RectangleRubberBand);
    //view2->QChartView::setRubberBand(QChartView::RectangleRubberBand);
    QGridLayout *baseLayout1 = new QGridLayout();
    QGridLayout *baseLayout2 = new QGridLayout();
    baseLayout1->addWidget(view1, 1, 0);
    baseLayout2->addWidget(view2, 1, 0);
    ui->DAU_1->setLayout(baseLayout1);
    ui->DAU_2->setLayout(baseLayout2);
}

void Function::paintDAU(){

}

void Function::on_DAUBox1_currentIndexChanged(const QString &arg1)
{
    DAU_loid1 = arg1.toInt();
}

void Function::on_DAUBox_2_currentIndexChanged(const QString &arg1)
{
    DAU_loid2 = arg1.toInt();
}

void Function::on_locationBox_currentIndexChanged(const QString &arg1)
{

}
