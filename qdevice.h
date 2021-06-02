#ifndef QDEVICE_H
#define QDEVICE_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QLineEdit>
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
    void SetPort(QSerialPort* serialPort);
    void SetRemoveButtonCallback(void (*callback)());
    void RemoveDevice();
    void Connect();
    void Disconnect();

    int Channel();
    QString PortName();
    int Current();
    int Frequency();
    int Duration();
    int WaveForm();
    bool IsStimulate();
    bool IsAvailable();
    bool IsPortExist();

    //ウィンドウサイズ
    static const int BOX_WIDTH = 180;
    static const int BOX_HEIGHT = 270;
    static const int BOX_MARGIN = 10;

    //波形を増やしたいときはこいつを編集
    const QMap<QString, int> WaveFormMap{
        {"Bipole", 0},
        {"PosiMono", 1},
        {"NegaMono", 2},
        {"PosiConst", 3},
        {"NegaConst", 4}
    };
signals:
    void removed(QDevice* dev);
    void stimEnd();
    void portNameChanged(QDevice* dev);

private:
    void SetStimulateState(bool isStimulate);
    void UpdatePortLabel();
    void SetIsAvailable(bool b);
    void AskDeviceState();
    void ReceiveDeviceState();
    void SendGVSParam(int current, int frequency, int waveForm);
    void SendGVSParam();

    QPushButton* removeButton;

    QSpinBox* channelSpinBox;
    QLineEdit* portNameLineEdit;
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
    QLabel* portAvailableLabel;

    QSerialPort* port;

    QLabel* stimulateStateLabel;

    bool isStimulate;
    bool isAvailable;

    QTimer* stimTimer;
};

#endif // QDEVICE_H
