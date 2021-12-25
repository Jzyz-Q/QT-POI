#include "mainwindow.h"
#include "OpenFile.h"
#include "dialog.h"
#include "Setting.h"
#include "function.h"
#include "ui_mainwindow.h"
#include "ui_dialog.h"
#include "ui_function.h"

#include "QFileDialog"
#include "QMessageBox"
#include "QGridLayout"
#include "QPushButton"
#include "QTextStream"
#include "QDebug"
#include "iostream"
#include <QMetaType>
#include <QEventLoop>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qRegisterMetaType<Datas>("Datas");
    Dataset = new QVector<Datas>;

    ui->setupUi(this);

    oui = new Dialog;
    oui->setWindowTitle("Open File");
    oui->show();

    //read in
    th1 = new Openf(this,Dataset);
    oui->ui->lineEdit->setPlaceholderText(" There are currently no files listed, please click \"browsing\".");
    connect(oui->ui->Open,SIGNAL(clicked(bool)),this,SLOT(FileOpen()));
    connect(oui->ui->begin,SIGNAL(clicked()),this,SLOT(begin()));
    connect(this,&MainWindow::SendFile,th1,&Openf::rec,Qt::AutoConnection);
    connect(th1,&Openf::SendToMain,this,&MainWindow::RecFromOp);

    //set range
    userid_flag = false;
    locationid_flag = false;
    lo_flag = false;
    la_flag = false;

    ui->userid_edit->setPlaceholderText("eg: 1;3;4");
    ui->locationid_edit->setPlaceholderText("eg: 1;3;4");
    ui->lo_edit->setPlaceholderText("eg: 30.24;31.35");
    ui->la_edit->setPlaceholderText("eg: 30.24;31.35");
    ui->fctBox->addItem("Top10 POIs");
    ui->fctBox->addItem("Number of Checking-ins");
    ui->fctBox->addItem("Comprasion Of Top POIs");
    ui->fctBox->addItem("Daily Active Users");
    ui->fctBox->addItem("Trajectory on Map");
    ui->fctBox->addItem("Heat Map");
    ui->fctBox->addItem("User Similarity");
    ui->introduction->setStyleSheet("QTextBrowser{border-width:0;border-style:outset}");

    connect(ui->User_ID,SIGNAL(clicked()),this,SLOT(SlcUserid()));
    connect(ui->Location_ID,SIGNAL(clicked()),this,SLOT(SlcLocationid()));
    connect(ui->Longitude,SIGNAL(clicked()),this,SLOT(SlcLo()));
    connect(ui->Latitude,SIGNAL(clicked()),this,SLOT(SlcLa()));

    Set_th2 = new Setting(this,Dataset);
    connect(ui->next,SIGNAL(clicked()),this,SLOT(setRange()));
    connect(this,&MainWindow::SendUsers,Set_th2,&Setting::rec_users);
    connect(this,&MainWindow::SendLocations,Set_th2,&Setting::rec_locations);
    connect(this,&MainWindow::SendLatitude,Set_th2,&Setting::rec_latitude);
    connect(this,&MainWindow::SendLongitude,Set_th2,&Setting::rec_longitude);

    //function
    fui = new Function(this);
    fui->setWindowTitle("Functions");
    connect(Set_th2,&Setting::SendDataset,fui,&Function::rec_dataset);
    connect(Set_th2,&Setting::SendData,fui,&Function::rec_data);
    connect(Set_th2,&Setting::SendFinishedSignal,fui,&Function::rec_finished);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::begin(){
    //qDebug()<<"Main ID:"<<QThread::currentThreadId()<<endl;
    oui->on_toolButton_clicked();
    this->show();
}

void MainWindow::RecFromOp(int i){
    oui->ui->Openprogress->setValue(i);
    if (i == 1502535){
        th1->wait();
        oui->ui->Loading->setText("Upload Successful!");
        th1->quit();
    }
}

void MainWindow::FileOpen(){
    oui->ui->Openprogress->setRange(0,1502535);
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("open a file."),
                "D:/",
                tr("excel(*.csv);;All files(*.*)")
        );
    QFile file(fileName);

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this, "Warning!", "Failed to open the video!");
            return;
        }

        if (fileName.isEmpty()) {
            QMessageBox::warning(this, "Warning!", "Failed to open the video!");
            return;
        }
        else {
            th1->start();
            emit SendFile(fileName);
            oui->ui->Loading->setText("Loading...");
            oui->ui->lineEdit->setText(fileName);
        }
}

void MainWindow::SlcUserid(){
    if (ui->User_ID->isChecked()){
        userid_flag = true;
    }
    else {
        userid_flag = false;
    }
}

void MainWindow::SlcLocationid(){
    if (ui->Location_ID->isChecked()){
        locationid_flag = true;
    }
    else {
        locationid_flag = false;
    }
}

void MainWindow::SlcLo(){
    if (ui->Longitude->isChecked()){
        lo_flag = true;
    }
    else {
        lo_flag = false;
    }
}

void MainWindow::SlcLa(){
    if (ui->Latitude->isChecked()){
        la_flag = true;
    }
    else {
        la_flag = false;
    }
}

void MainWindow::setRange(){
    QStringList list;
    list.clear();

    if (userid_flag){
        QString tmp_uid = ui->userid_edit->text();
        emit SendUsers(tmp_uid);
    }

    if (locationid_flag){
        QString tmp_lctid = ui->locationid_edit->text();
        emit SendLocations(tmp_lctid);
    }

    if (la_flag){
        QString tmp_la = ui->la_edit->text();
        emit SendLatitude(tmp_la);
    }

    if (lo_flag){
        QString tmp_lo = ui->lo_edit->text();
        emit SendLongitude(tmp_lo);
    }

    //qDebug() << Dataset->size() << "Dataset";

    Set_th2->start();
    this->hide();
    startfunction();
}

void MainWindow::startfunction(){
    Set_th2->quit();
    fui->show();
}

void MainWindow::on_fctBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        ui->introduction->setText("  Here we have counted the ten most visited locations "
                                  "by a specific user or all users(but no more than 10).");
        break;
    case 1:
        ui->introduction->setText("  This feature compares the top10 of different users"
                                  " on the basis of the statistics of the top10. The "
                                  "coordinates of a point are its latitude and longitude."
                                  " A user is represented by a dot of one color.");
        break;
    case 2:
        ui->introduction->setText("  This section implements statistics on the "
                                  "number of times a certain location ID or a certain"
                                  " latitude and longitude range has been accessed "
                                  "by all users.");
        break;
    case 3:
        ui->introduction->setText("  In this part, we compared the change in "
                                  "active users over time for two locations. When"
                                  " you changes current steps of time, the dynamic"
                                  " change of active users can be observed.");
        break;
    case 4:
        ui->introduction->setText("  Select a user and you will be able to see the"
                                  " track of his punch card on the map.");
        break;
    case 5:
        ui->introduction->setText("  Here you will see on the map how hot each "
                                  "location has been visited, the darker the color"
                                  " the more times it has been visited");
        break;
    case 6:
        ui->introduction->setText("  This part evaluates the similarity of two users "
                                  "based on the locations they visit and the number of"
                                  " times they clock in over a period of time");
        break;
    default:
        break;
    }
}









