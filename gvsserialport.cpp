#include "gvsserialport.h"
#include "qdevice.h"
#include <QtCore/QDebug>

GVSSerialPort::GVSSerialPort(const QSerialPortInfo &info)
    :QSerialPort(info)
{
    connect(this, &QSerialPort::readyRead, this, [=]{
        DistributeReceivedData();
    });
}

void GVSSerialPort::AddDevice(QDevice* device)
{
    if(!deviceList.contains(device)){
        deviceList.append(device);
    }
}

void GVSSerialPort::RemoveDevice(QDevice* device)
{
    if(deviceList.contains(device)){
        deviceList.removeOne(device);
        if(deviceList.count() == 0){
            emit removed(this);
            if(this->isOpen()){
                this->close();
            }
            delete this;
        }
    }
}

void GVSSerialPort::DistributeReceivedData(){
    if(sender() == this){
        QString data = this->readAll();
        qDebug() << "Received: " << data;
        for(auto dev : deviceList){
            dev->ReceiveDeviceState(data);
        }
    }
}
