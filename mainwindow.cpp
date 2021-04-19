#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    isConnect = false;
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

void MainWindow::Connect(){
    if(isConnect) return;
    qDebug() << "Connect.";

    foreach(auto &dev, deviceList){
        QString portName = dev->GetPortName();
        //まだ開いていないポートか確認
        bool portExist = true;
        if(!nameToPort.contains(portName)){
            portExist = false;
            //名前の一致するポートを探す
            foreach(auto &serialPortInfo, QSerialPortInfo::availablePorts()){
                if(serialPortInfo.portName() == portName){
                    portExist = true;
                    QSerialPort* serialPort = new QSerialPort(serialPortInfo);
                    //ポートオープン
                    serialPort->open(QIODevice::ReadWrite);
                    qDebug() << portName << " added.";
                    nameToPort[portName] = serialPort;
                    break;
                }
            }
        }
        //ポートが無かったらエラー表示
        dev->SetPortExist(portExist);
        //デバイス接続
        if(portExist){
            isConnect = true;
            dev->Connect(nameToPort[portName]);
        }
        dev->Connect(nullptr);
    }
}
void MainWindow::Disconnect(){
    if(!isConnect) return;
    isConnect = false;
    qDebug() << "Disconnect.";

    //デバイスの停止
    foreach(auto &dev, deviceList){
        dev->Disconnect();
    }
    //ポートのクローズ
    foreach(auto serialPort, nameToPort.values()){
        serialPort->close();
    }
    nameToPort.clear();
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
