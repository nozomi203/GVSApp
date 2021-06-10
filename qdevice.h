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
    int TransitionDuration();
    int TransitionForm();

    bool IsStimulate();
    bool IsAvailable();
    bool IsPortExist();

    //ウィンドウサイズ
    static const int BOX_WIDTH = 260;
    static const int BOX_HEIGHT = 330;
    static const int BOX_MARGIN = 10;
    //最大値
    static const int MAX_CHANNEL = 7;
    static const int MAX_CURRENT = 4095;
    static const int MAX_FREQUENCY = 1023;
    static const int MAX_STIM_DURATION = 180;
    static const int MAX_TRANSITION_DURATION = 15;
    //初期値
    static const int INIT_CHANNEL = 0;
    static const int INIT_CURRENT = 512;
    static const int INIT_FREQUENCY = 80;
    static const int INIT_STIM_DURATION = 10;
    static const int INIT_TRANSITION_DURATION = 1;
    static const int INIT_WAVEFORM_IDX = 3;
    static const int INIT_TRANSITIONFORM_IDX = 0;

    //波形を増やしたいときはこいつを編集
    const QMap<QString, int> WaveFormMap{
        {"Square_Bipole", 0},
        {"Square_Positive", 1},
        {"Square_Negative", 2},
        {"Direct_Positive", 3},
        {"Direct_Negative", 4},
        {"Sin", 5}
    };
    //刺激開始時の値の上げ方を増やしたいときはこいつを編集
    const QMap<QString, int> TransitionFormMap{
        {"Constant", 0},
        {"Linear", 1},
        {"Smooth", 2}
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
    void SendGVSParam(int current, int frequency, int waveForm, int transitionDuration, int transitionForm);
    void SendGVSParam();

    QPushButton* removeButton;

    QSpinBox* channelSpinBox;
    QLineEdit* portNameLineEdit;
    QSpinBox* currentSpinBox;
    QSpinBox* frequencySpinBox;
    QSpinBox* stimDurationSpinBox;
    QComboBox* waveFormComboBox;
    QSpinBox* transitionDurationSpinBox;
    QComboBox* transitionFormComboBox;

    QLabel* channelLabel;
    QLabel* portNameLabel;
    QLabel* currentLabel;
    QLabel* frequencyLabel;
    QLabel* stimDurationLabel;
    QLabel* waveFormLabel;
    QLabel* transitionDurationLabel;
    QLabel* transitionFormLabel;

    QLabel* portErrorLabel;
    QLabel* portAvailableLabel;

    QLabel* stimulateStateLabel;

    int channel;
    QSerialPort* port;
    int current;
    int frequency;
    int stimDuration;
    int waveForm;
    int transitionDuration;
    int transitionForm;

    bool isStimulate;
    bool isAvailable;

    QTimer* stimTimer;
};

#endif // QDEVICE_H
