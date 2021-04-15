#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qdevice.h"

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

private slots:
    void on_AddDeviceButton_clicked();
    void RemoveDevice(QDevice* device);


private:
    Ui::MainWindow *ui;
    QList<QDevice*> deviceList;
    const int MAX_DEVICE_COUNT = 4;
};
#endif // MAINWINDOW_H
