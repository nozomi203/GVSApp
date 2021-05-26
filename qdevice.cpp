 #include "qdevice.h"
#include <QtCore/QDebug>

QDevice::QDevice(int deviceID, QWidget *parent) : QGroupBox(parent)
{
    qDebug() << "Hello Device.";
    //Widget生成
    removeButton = new QPushButton(this);

    channelSpinBox = new QSpinBox(this);
    portNameLineEdit = new QLineEdit(this);
    currentSpinBox = new QSpinBox(this);
    frequencySpinBox = new QSpinBox(this);
    durationSpinBox = new QSpinBox(this);
    waveFormComboBox = new QComboBox(this);

    channelLabel = new QLabel(this);
    portNameLabel = new QLabel(this);
    currentLabel = new QLabel(this);
    frequencyLabel = new QLabel(this);
    durationLabel = new QLabel(this);
    waveFormLabel = new QLabel(this);

    portErrorLabel = new QLabel(this);
    portAvailableLabel = new QLabel(this);

    stimulateStateLabel = new QLabel(this);

    //移動
    this->setFixedSize(180, 270);
    this->SetID(deviceID);

    removeButton->setGeometry(100,10,70,28);

    channelSpinBox->setGeometry(90, 67, 80, 22);
    portNameLineEdit->setGeometry(90, 97, 80, 22);
    currentSpinBox->setGeometry(90, 127, 80, 22);
    frequencySpinBox->setGeometry(90, 157, 80, 22);
    durationSpinBox->setGeometry(90, 187, 80, 22);
    waveFormComboBox->setGeometry(90, 217, 80, 22);
    stimulateStateLabel->setGeometry(90, 250, 80, 15);

    portErrorLabel->setGeometry(10, 40, 160,15);
    channelLabel->setGeometry(10, 70, 80, 15);
    portNameLabel->setGeometry(10, 100, 80, 15);
    currentLabel->setGeometry(10, 130, 80, 15);
    frequencyLabel->setGeometry(10, 160, 80, 15);
    durationLabel->setGeometry(10, 190, 80, 15);
    waveFormLabel->setGeometry(10, 220, 80, 15);
    portAvailableLabel->setGeometry(10, 250, 80, 15);

    //ComboBoxの初期化
    foreach(auto waveForm, WaveFormMap.keys()){
        waveFormComboBox->addItem(waveForm);
    }

    //コールバック設定
    connect(removeButton, &QPushButton::clicked, this, [=](){
            RemoveDevice();
    });

    connect(portNameLineEdit, &QLineEdit::textChanged, this, [=]{
        emit portNameChanged(this);
    });

    connect(channelSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=]{
        AskDeviceState();
    });

    //値の設定
    removeButton->setText("Remove");

    channelSpinBox->setMaximum(3);
    currentSpinBox->setMaximum(4095);
    frequencySpinBox->setMaximum(200);
    durationSpinBox->setMaximum(180);
    channelSpinBox->setValue(0);
    portNameLineEdit->setText("COM0");
    currentSpinBox->setValue(500);
    frequencySpinBox->setValue(80);
    durationSpinBox->setValue(10);

    channelLabel->setText("Channel:");
    portNameLabel->setText("PortName:");
    currentLabel->setText("Current:");
    frequencyLabel->setText("Frequency:");
    durationLabel->setText("Duration:");
    waveFormLabel->setText("WaveForm:");

    portAvailableLabel->setText("No device.");
    portErrorLabel->setText("Port doesn't exist.");
    QPalette palette = portErrorLabel->palette();
    palette.setColor(QPalette::Foreground, QColor("#FF0000"));
    portAvailableLabel->setPalette(palette);
    portErrorLabel->setPalette(palette);

    //最初は無刺激
    SetStimulateState(false);
    SetIsAvailable(false);

    //タイマー作成
    stimTimer = new QTimer(this);
    //コールバックは一度キリ
    stimTimer->setSingleShot(true);
    connect(stimTimer, &QTimer::timeout, this, [=](){
        Disconnect();
    });
}

void QDevice::SetID(int id){
    this->setTitle("Device " + QString::number(id));
    this->move(10 + 190 * id, 60);
}
void QDevice::SetPort(QSerialPort* serialPort){
    port = serialPort;
    UpdatePortLabel();
    SetIsAvailable(false);
    if(IsPortExist()){
        //デバイスの状態を確認
        AskDeviceState();
        connect(port, &QSerialPort::readyRead, this, [=]{
            ReceiveDeviceState();
        });
    }
}

void QDevice::RemoveDevice(){
    //接続の停止
    if(isStimulate){
        Disconnect();
    }
    emit removed(this);
    delete this;
}

void QDevice::Connect(){
    if(isAvailable){
        qDebug() << PortName() << "Connect.";
        SetStimulateState(true);
        //刺激開始
        SendGVSParam();
        //タイマー開始
        stimTimer->start(Duration() * 1000);
    }
}

void QDevice::Disconnect(){
    qDebug() << "Stop GVS";
    stimTimer->stop();
    SetStimulateState(false);
    SendGVSParam(0,0,0);
    emit stimEnd();
}

int QDevice::Channel(){
    return channelSpinBox->value();
}
QString QDevice::PortName(){
    return portNameLineEdit->text();
}
int QDevice::Current(){
    return currentSpinBox->value();
}
int QDevice::Frequency(){
    return frequencySpinBox->value();
}
int QDevice::Duration(){
    return durationSpinBox->value();
}
int QDevice::WaveForm(){
    return WaveFormMap[waveFormComboBox->currentText()];
}
bool QDevice::IsStimulate(){
    return isStimulate;
}
bool QDevice::IsAvailable(){
    return isAvailable;
}
bool QDevice::IsPortExist(){
    return port != nullptr && port->isOpen();
}
void QDevice::UpdatePortLabel(){
    if(IsPortExist()){
        portErrorLabel->hide();
    }else{
        portErrorLabel->show();
    }
}

void QDevice::SetIsAvailable(bool b){
    isAvailable = b;
    if(isAvailable){
        portAvailableLabel->hide();
    }else{
        portAvailableLabel->show();
    }
}

void QDevice::SetStimulateState(bool b){
    isStimulate = b;
    QPalette palette = stimulateStateLabel->palette();

    if(isStimulate){
        stimulateStateLabel->setText("Stim");
        palette.setColor(QPalette::Foreground, QColor("#00FF00"));
    }else{
        stimulateStateLabel->setText("Stay");
        palette.setColor(QPalette::Foreground, QColor("#FF0000"));
    }

    stimulateStateLabel->setPalette(palette);
}

void QDevice::AskDeviceState(){
    if(!IsPortExist()) return;
    //安否確認
    qDebug() << "Send message.";
    //Cはマジックナンバー
    port->write("C", 1);
}
void QDevice::ReceiveDeviceState(){
    if(sender() == port){
        QString data = port->readAll();
        qDebug() << "Received: " << data;
        if(data == QString::number(Channel())){
            qDebug() << "Device is alive.";
            SetIsAvailable(true);
        }
    }
}

void QDevice::SendGVSParam(){
    SendGVSParam(Current(), Frequency(), WaveForm());
}
void QDevice::SendGVSParam(int current, int frequency, int waveForm){
    char dat1 = 0;
    char dat2 = 0;
    char dat3 = 0;
    char dat4 = 0;
    int channel = Channel();
    //dat1
    if((channel & 0x02) != 0)dat1 += 128;
    if((channel & 0x01) != 0)dat1 += 64;
    if((current & 0x800) != 0)dat1 += 32;
    if((current & 0x400) != 0)dat1 += 16;
    if((current & 0x200) != 0)dat1 += 8;
    if((current & 0x100) != 0)dat1 += 4;
    if((current & 0x80) != 0)dat1 += 2;
    if((current & 0x40) != 0)dat1 += 1;
    //dat2
    if ((current & 0x20) != 0) dat2 += 128;
    if ((current & 0x10) != 0) dat2 += 64;
    if ((current & 0x08) != 0) dat2 += 32;
    if ((current & 0x04) != 0) dat2 += 16;
    if ((current & 0x02) != 0) dat2 += 8;
    if ((current & 0x01) != 0) dat2 += 4;
    if ((frequency & 0x200) != 0) dat2 += 2;
    if ((frequency & 0x100) != 0) dat2 += 1;
    //dat3
    if ((frequency & 0x80) != 0) dat3 += 128;
    if ((frequency & 0x40) != 0) dat3 += 64;
    if ((frequency & 0x20) != 0) dat3 += 32;
    if ((frequency & 0x10) != 0) dat3 += 16;
    if ((frequency & 0x08) != 0) dat3 += 8;
    if ((frequency & 0x04) != 0) dat3 += 4;
    if ((frequency & 0x02) != 0) dat3 += 2;
    if ((frequency & 0x01) != 0) dat3 += 1;
    //dat4
    if ((waveForm & 0x04) != 0) dat4 += 128;
    if ((waveForm & 0x02) != 0) dat4 += 64;
    if ((waveForm & 0x01) != 0) dat4 += 32;

    char buffer[5] = {71, dat1, dat2, dat3, dat4};
    port->write(buffer, sizeof(buffer));
}
