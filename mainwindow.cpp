#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddDevice(){
    QDevice* dev = new QDevice(deviceList.count(), ui->centralwidget);
    connect(dev, SIGNAL(removed(QDevice*)), this, SLOT(RemoveDevice(QDevice*)));
    dev->show();
    deviceList.append(dev);
    if(deviceList.count() == MAX_DEVICE_COUNT){
        ui->AddDeviceButton->setDisabled(true);
    }
}

void MainWindow::RemoveDevice(QDevice* device){
    deviceList.removeOne(device);
    ui->AddDeviceButton->setDisabled(false);
    for(int i = 0; i < deviceList.count(); i++){
        deviceList[i]->SetID(i);
    }
}

void MainWindow::on_AddDeviceButton_clicked()
{
    AddDevice();
}


void MainWindow::on_StartButton_clicked()
{

}

void MainWindow::on_StopButton_clicked()
{

}
