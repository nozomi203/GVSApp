#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "qdevice.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static const int MAX_DEVICE_COUNT = 18;
    static const int DEVICE_PER_ROW = 6;
    static const int WINDOW_WIDTH = 1200;
    static const int WINDOW_HEIGHT = 900;
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
    void FindTargetPort(QDevice* device);

private:
    Ui::MainWindow *ui;
    QList<QDevice*> deviceList;
    QMap<QString, QSerialPort*> nameToPort;
    bool isConnect;
};
#endif // MAINWINDOW_H
