#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qdevice.h"

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void AddDevice();
    void Connect();
    void Disconnect();

private slots:
    void on_AddDeviceButton_clicked();
    void on_StartButton_clicked();
    void on_StopButton_clicked();

    void RemoveDevice(QDevice* device);
    void DetectStimEnd();

private:
    Ui::MainWindow *ui;
    QList<QDevice*> deviceList;
    QMap<QString, QSerialPort*> nameToPort;
    const int MAX_DEVICE_COUNT = 4;
    bool isConnect;
};
#endif // MAINWINDOW_H
