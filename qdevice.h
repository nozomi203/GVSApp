#ifndef QDEVICE_H
#define QDEVICE_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QPlainTextEdit>
#include <QComboBox>
#include <QTimer>

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

    int Channel();
    QString PortName();
    int Current();
    int Frequency();
    int Duration();
    int WaveForm();
    bool IsStimulate();

    //波形を増やしたいときはこいつを編集
    const QMap<QString, int> WaveFormMap{
        {"Direct", 3},
        {"Square", 0}
    };
signals:
    void removed(QDevice* dev);
    void stimEnd();

private:
    void SetStimulateState(bool isStimulate);
    void SetPortExist(bool portExist);
    void SendGVSParam(int current, int frequency, int waveForm);
    void SendGVSParam();

    QPushButton* removeButton;

    QSpinBox* channelSpinBox;
    QPlainTextEdit* portNamePlainTextEdit;
    QSpinBox* currentSpinBox;
    QSpinBox* frequencySpinBox;
    QSpinBox* durationSpinBox;
    QComboBox* waveFormComboBox;

    QLabel* channelLabel;
    QLabel* portNameLabel;
    QLabel* currentLabel;
    QLabel* frequencyLabel;
    QLabel* durationLabel;
    QLabel* waveFormLabel;

    QLabel* portErrorLabel;

    QSerialPort* port;

    QLabel* stimulateStateLabel;

    bool isStimulate;

    QTimer* stimTimer;
};

#endif // QDEVICE_H
