#include "mainwindow.h"
#include "OpenFile.h"
#include "dialog.h"
#include "Setting.h"
#include "ui_mainwindow.h"
#include "ui_dialog.h"

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
    ui->setupUi(this);
    oui = new Dialog;
    oui->setWindowFlags(Qt::WindowStaysOnTopHint);
    Dataset = new QVector<Datas>;

    //read in
    th1 = new Openf(this,Dataset);
    connect(ui->start,SIGNAL(clicked()),this,SLOT(begin()));
    connect(oui->ui->Open,SIGNAL(clicked()),this,SLOT(FileOpen()));
    connect(this,&MainWindow::SendFile,th1,&Openf::rec,Qt::AutoConnection);
    connect(th1,&Openf::SendToMain,this,&MainWindow::RecFromOp);

    //set range
    //ui.lineEdit->setPlaceholderText(QString::fromLocal8Bit("提示输入"));
    ui->userid_edit->setPlaceholderText("eg: 1;3;4-5");
    userid_flag = false;
    locationid_flag = false;
    time_flag = false;
    lo_flag = false;
    la_flag = false;
    connect(ui->User_ID,SIGNAL(clicked()),this,SLOT(SlcUserid()));
    connect(ui->Location_ID,SIGNAL(clicked()),this,SLOT(SlcLocationid()));
    connect(ui->Time,SIGNAL(clicked()),this,SLOT(SlcTime()));
    connect(ui->Longitude,SIGNAL(clicked()),this,SLOT(SlcLo()));
    connect(ui->Latitude,SIGNAL(clicked()),this,SLOT(SlcLa()));

    Set_th2 = new Setting(this,Dataset);
    connect(ui->next,SIGNAL(clicked()),this,SLOT(setRange()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::begin(){
    qDebug()<<"Main ID:"<<QThread::currentThreadId()<<endl;
    oui->show();
}

void MainWindow::RecFromOp(int i){
    oui->ui->Openprogress->setValue(i);
    if (i == 1502535){
        oui->ui->Loading->setText("Upload Successful!");
        oui->on_toolButton_clicked();
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

        if (fileName.isEmpty()) {
            QMessageBox::warning(this, "Warning!", "Failed to open the video!");
        }
        else {
            th1->start();
            emit SendFile(fileName);
            oui->ui->Loading->setText("Loading...");
        }
        ui->lineEdit->setText(fileName);
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
    if (ui->User_ID->isChecked()){
        locationid_flag = true;
    }
    else {
        locationid_flag = false;
    }
}

void MainWindow::SlcTime(){
    if (ui->User_ID->isChecked()){
        time_flag = true;
    }
    else {
        time_flag = false;
    }
}

void MainWindow::SlcLo(){
    if (ui->User_ID->isChecked()){
        lo_flag = true;
    }
    else {
        lo_flag = false;
    }
}

void MainWindow::SlcLa(){
    if (ui->User_ID->isChecked()){
        la_flag = true;
    }
    else {
        la_flag = false;
    }
}

void MainWindow::setRange(){
    QStringList list;
    list.clear();
    QString tmp_uid = ui->userid_edit->text();
    list = tmp_uid.split(";", QString::SkipEmptyParts);
}















