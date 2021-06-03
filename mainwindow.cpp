#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isConnect = false;
    this->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddDevice(){
    if(isConnect) return;
    QDevice* dev = new QDevice(deviceList.count(), ui->centralwidget);
    connect(dev, SIGNAL(removed(QDevice*)), this, SLOT(RemoveDevice(QDevice*)));
    connect(dev, SIGNAL(stimEnd()), this, SLOT(DetectStimEnd()));
    connect(dev, SIGNAL(portNameChanged(QDevice*)), this, SLOT(FindTargetPort(QDevice*)));
    dev->show();
    deviceList.append(dev);
    if(deviceList.count() == MAX_DEVICE_COUNT){
        ui->AddDeviceButton->setDisabled(true);
    }
}

void MainWindow::RemoveDevice(QDevice* device){
    qDebug() << "Remove device.";
    deviceList.removeOne(device);
    ui->AddDeviceButton->setDisabled(false);
    for(int i = 0; i < deviceList.count(); i++){
        deviceList[i]->SetID(i);
    }
}

void MainWindow::DetectStimEnd(){
    foreach(auto &dev, deviceList){
        if(dev->IsStimulate()){
            return;
        }
    }
    Disconnect();
}

void MainWindow::FindTargetPort(QDevice* device){
    QSerialPort* sp = nullptr;
    QString portName = device->PortName();
    if(nameToPort.contains(portName)){
        sp = nameToPort[portName];
    }else{
        //名前の一致するポートを探す
        foreach(auto &serialPortInfo, QSerialPortInfo::availablePorts()){
            if(serialPortInfo.portName() == portName){
                QSerialPort* newPort = new QSerialPort(serialPortInfo);
                //ポートオープン
                if(newPort->open(QIODevice::ReadWrite)){
                    qDebug() << portName << " added.";
                    nameToPort[portName] = newPort;
                    sp = nameToPort[portName];
                    break;
                }else{
                    qDebug() << "Couldn't open the port.";
                }
            }
        }
    }

    device->SetPort(sp);
}

void MainWindow::Connect(){
    if(isConnect) return;
    qDebug() << "Connect.";

    foreach(auto &dev, deviceList){
        if(dev->IsAvailable()){
            isConnect = true;
            qDebug() << "Connect.";
            dev->Connect();
        }
    }
}
void MainWindow::Disconnect(){
    if(!isConnect) return;
    isConnect = false;
    qDebug() << "Disconnect.";

    //デバイスの停止
    foreach(auto &dev, deviceList){
        if(dev->IsAvailable()){
            dev->Disconnect();
        }
    }
}

void MainWindow::on_AddDeviceButton_clicked()
{
    AddDevice();
}


void MainWindow::on_StartButton_clicked()
{
    Connect();
}

void MainWindow::on_StopButton_clicked()
{
    Disconnect();
}
