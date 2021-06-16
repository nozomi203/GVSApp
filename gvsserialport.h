#ifndef GVSSERIALPORT_H
#define GVSSERIALPORT_H

//#include "qdevice.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class QDevice;

class GVSSerialPort : public QSerialPort
{
    Q_OBJECT
public:
    GVSSerialPort(const QSerialPortInfo &info);
    void AddDevice(QDevice* device);
    void RemoveDevice(QDevice* device);

signals:
    void removed(GVSSerialPort* port);

private:
    void DistributeReceivedData();
    QList<QDevice*> deviceList;
};

#endif // GVSSERIALPORT_H
