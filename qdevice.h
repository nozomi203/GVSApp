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
    int StimDuration();
    int WaveForm();
    int GradDuration();
    int GradForm();

    bool IsStimulate();
    bool IsAvailable();
    bool IsPortExist();

    //ウィンドウサイズ
    static const int BOX_WIDTH = 180;
    static const int BOX_HEIGHT = 330;
    static const int BOX_MARGIN = 10;
    //最大値
    static const int MAX_CHANNEL = 3;
    static const int MAX_CURRENT = 4095;
    static const int MAX_FREQUENCY = 1023;
    static const int MAX_STIM_DURATION = 180;
    static const int MAX_GRAD_DURATION = 7;
    //初期値
    static const int INIT_CHANNEL = 0;
    static const int INIT_CURRENT = 512;
    static const int INIT_FREQUENCY = 80;
    static const int INIT_STIM_DURATION = 10;
    static const int INIT_GRAD_DURATION = 0;

    //波形を増やしたいときはこいつを編集
    const QMap<QString, int> WaveFormMap{
        {"Bipole", 0},
        {"PosiMono", 1},
        {"NegaMono", 2},
        {"PosiConst", 3},
        {"NegaConst", 4}
    };
    //刺激開始時の値の上げ方を増やしたいときはこいつを編集
    const QMap<QString, int> GradFormMap{
        {"Const", 0},
        {"Linear", 1},
        {"Sigmoid", 2}
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
    void SendGVSParam(int current, int frequency, int waveForm, int gradDuration, int GradForm);
    void SendGVSParam();

    QPushButton* removeButton;

    QSpinBox* channelSpinBox;
    QLineEdit* portNameLineEdit;
    QSpinBox* currentSpinBox;
    QSpinBox* frequencySpinBox;
    QSpinBox* stimDurationSpinBox;
    QComboBox* waveFormComboBox;
    QSpinBox* gradDurationSpinBox;
    QComboBox* gradFormComboBox;

    QLabel* channelLabel;
    QLabel* portNameLabel;
    QLabel* currentLabel;
    QLabel* frequencyLabel;
    QLabel* stimDurationLabel;
    QLabel* gradDurationLabel;
    QLabel* waveFormLabel;
    QLabel* gradFormLabel;

    QLabel* portErrorLabel;
    QLabel* portAvailableLabel;

    QSerialPort* port;

    QLabel* stimulateStateLabel;

    bool isStimulate;
    bool isAvailable;

    QTimer* stimTimer;
};

#endif // QDEVICE_H
