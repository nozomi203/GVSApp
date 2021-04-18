#ifndef QDEVICE_H
#define QDEVICE_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QPlainTextEdit>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class QDevice : public QGroupBox
{
    Q_OBJECT
public:
    explicit QDevice(int deviceID, QWidget *parent = nullptr);
    void SetID(int id);
    void SetRemoveButtonCallback(void (*callback)());
    void RemoveDevice();
    void Connect(QSerialPort *serialPort);
    void Disconnect();
    QString GetPortName();
    void SetPortExist(bool portExist);
signals:
    void removed(QDevice* dev);

private:
    void SetStimulateState(bool isStimulate);
    void SendGVSParam(int current, int frequency);
    void SendGVSParam();

    QPushButton* removeButton;

    QSpinBox* idSpinBox;
    QPlainTextEdit* portNamePlainTextEdit;
    QSpinBox* currentSpinBox;
    QSpinBox* frequencySpinBox;
    QSpinBox* durationSpinBox;

    QLabel* idLabel;
    QLabel* portNameLabel;
    QLabel* currentLabel;
    QLabel* frequencyLabel;
    QLabel* durationLabel;

    QLabel* portErrorLabel;

    QSerialPort* port;

    QLabel* stimulateStateLabel;

    bool isStimulate;
};

#endif // QDEVICE_H
