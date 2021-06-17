#include "qdevice.h"
#include "gvsserialport.h"
#include "mainwindow.h"
#include <QtCore/QDebug>


QDevice::QDevice(int deviceID, QWidget *parent) : QGroupBox(parent)
{
    qDebug() << "Hello Device.";

    //Widget生成
    qDebug() << "Instantiate widget.";
    removeButton = new QPushButton(this);

    channelSpinBox = new QSpinBox(this);
    portNameLineEdit = new QLineEdit(this);
    currentSpinBox = new QSpinBox(this);
    frequencySpinBox = new QSpinBox(this);
    stimDurationSpinBox = new QSpinBox(this);
    waveFormComboBox = new QComboBox(this);
    transitionDurationSpinBox = new QSpinBox(this);
    transitionFormComboBox = new QComboBox(this);

    channelLabel = new QLabel(this);
    portNameLabel = new QLabel(this);
    currentLabel = new QLabel(this);
    frequencyLabel = new QLabel(this);
    stimDurationLabel = new QLabel(this);
    waveFormLabel = new QLabel(this);
    transitionDurationLabel = new QLabel(this);
    transitionFormLabel = new QLabel(this);

    portErrorLabel = new QLabel(this);
    //portAvailableLabel = new QLabel(this);
    stimulateStateLabel = new QLabel(this);

    //移動
    qDebug() << "Set position.";
    this->setFixedSize(BOX_WIDTH, BOX_HEIGHT);
    this->SetID(deviceID);

    removeButton->setGeometry(180,10,70,28);

    channelSpinBox->setGeometry(120, 67, 130, 22);
    channelSpinBox->setAlignment(Qt::AlignRight);
    portNameLineEdit->setGeometry(120, 97, 130, 22);
    portNameLineEdit->setAlignment(Qt::AlignRight);
    currentSpinBox->setGeometry(120, 127, 130, 22);
    currentSpinBox->setAlignment(Qt::AlignRight);
    frequencySpinBox->setGeometry(120, 157, 130, 22);
    frequencySpinBox->setAlignment(Qt::AlignRight);
    stimDurationSpinBox->setGeometry(120, 187, 130, 22);
    stimDurationSpinBox->setAlignment(Qt::AlignRight);
    waveFormComboBox->setGeometry(120, 217, 130, 22);
    transitionDurationSpinBox->setGeometry(120, 247, 130, 22);
    transitionDurationSpinBox->setAlignment(Qt::AlignRight);
    transitionFormComboBox->setGeometry(120, 277, 130, 22);
    stimulateStateLabel->setGeometry(120, 310, 130, 15);

    portErrorLabel->setGeometry(10, 40, 160,15);
    channelLabel->setGeometry(10, 70, 110, 15);
    portNameLabel->setGeometry(10, 100, 110, 15);
    currentLabel->setGeometry(10, 130, 110, 15);
    frequencyLabel->setGeometry(10, 160, 110, 15);
    stimDurationLabel->setGeometry(10, 190, 110, 15);
    waveFormLabel->setGeometry(10, 220, 110, 15);
    transitionDurationLabel->setGeometry(10, 250, 110, 15);
    transitionFormLabel->setGeometry(10, 280, 110,15);
    //portAvailableLabel->setGeometry(10, 310, 110, 15);
    //ComboBoxの初期化
    foreach(auto waveForm, WaveFormMap.keys()){
        waveFormComboBox->addItem(waveForm);
    }
    foreach(auto gradForm, TransitionFormMap.keys()){
        transitionFormComboBox->addItem(gradForm);
    }

    //最初は無刺激
    SetStimulateState(false);
    //SetIsAvailable(false);

    //コールバック設定
    qDebug() << "Set callback";
    connect(removeButton, &QPushButton::clicked, this, [=](){
            RemoveDevice();
    });
    connect(channelSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value){
        if(IsStimulate()) Disconnect();
        channel = value;
        //SetIsAvailable(false);
        //AskDeviceState();
    });
    connect(portNameLineEdit, &QLineEdit::textChanged, this, [=]{
        if(IsStimulate()) Disconnect();
        emit portNameChanged(this);
    });
    connect(currentSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value){
        current = value;
        if(IsStimulate()){
            SendGVSParam();
        }
    });
    connect(frequencySpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value){
        frequency = value;
        if(IsStimulate()){
            SendGVSParam();
        }
    });
    connect(stimDurationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value){
        stimDuration = value;
    });
    connect(waveFormComboBox, &QComboBox::currentTextChanged, this, [=](QString value){
        waveForm = WaveFormMap[value];
    });
    connect(transitionDurationSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int value){
        transitionDuration = value;
    });
    connect(transitionFormComboBox, &QComboBox::currentTextChanged, this, [=](QString value){
        transitionForm = TransitionFormMap[value];
    });

    //値の設定
    qDebug() << "Set value.";
    removeButton->setText("Remove");

    channelSpinBox->setMaximum(MAX_CHANNEL);
    currentSpinBox->setMaximum(MAX_CURRENT);
    frequencySpinBox->setMaximum(MAX_FREQUENCY);
    stimDurationSpinBox->setMaximum(MAX_STIM_DURATION);
    transitionDurationSpinBox->setMaximum(MAX_TRANSITION_DURATION);
    channelSpinBox->setValue(INIT_CHANNEL);
    portNameLineEdit->setText("COM0");
    currentSpinBox->setValue(INIT_CURRENT);
    frequencySpinBox->setValue(INIT_FREQUENCY);
    waveFormComboBox->setCurrentIndex(INIT_WAVEFORM_IDX);
    stimDurationSpinBox->setValue(INIT_STIM_DURATION);
    transitionDurationSpinBox->setValue(INIT_TRANSITION_DURATION);
    transitionFormComboBox->setCurrentIndex(INIT_TRANSITIONFORM_IDX);

    channelLabel->setText("Channel:");
    portNameLabel->setText("PortName:");
    currentLabel->setText("Current:");
    frequencyLabel->setText("Frequency:");
    stimDurationLabel->setText("Duration:");
    waveFormLabel->setText("WaveForm:");
    transitionDurationLabel->setText("Transition:");
    transitionFormLabel->setText("TransitionForm:");

    //portAvailableLabel->setText("No device.");
    portErrorLabel->setText("Port doesn't exist.");
    QPalette palette = portErrorLabel->palette();
    palette.setColor(QPalette::Foreground, QColor("#FF0000"));
    //portAvailableLabel->setPalette(palette);
    portErrorLabel->setPalette(palette);

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
    int row = id / MainWindow::DEVICE_PER_ROW;
    int column = id % MainWindow::DEVICE_PER_ROW;
    int max_column = MainWindow::DEVICE_PER_ROW;
    int margin_left = (MainWindow::WINDOW_WIDTH
                       - BOX_WIDTH * max_column
                       - BOX_MARGIN * (max_column - 1)) / 2;
    this->move(margin_left + (BOX_WIDTH + BOX_MARGIN) * column, MainWindow::MARGIN_TOP + (BOX_HEIGHT + BOX_MARGIN) * row);
}
void QDevice::SetPort(GVSSerialPort* serialPort){
    if(port != nullptr){
        port->RemoveDevice(this);
    }
    port = serialPort;
    if(port != nullptr){
        port->AddDevice(this);
    }
    UpdatePortLabel();
    //SetIsAvailable(false);
    //if(IsPortExist()){
        //デバイスの状態を確認
        //AskDeviceState();
    //}
}

void QDevice::RemoveDevice(){
    //接続の停止
    if(IsStimulate()){
        Disconnect();
    }
    emit removed(this);
    delete this;
}

void QDevice::Connect(){
    if(IsPortExist()){
        qDebug() << PortName() << "Connect.";
        SetStimulateState(true);
        //刺激開始
        SendGVSParam();
        //タイマー開始
        stimTimer->start(StimDuration() * 1000);
    }
}

void QDevice::Disconnect(){
    qDebug() << "Stop GVS";
    stimTimer->stop();
    SetStimulateState(false);
    SendGVSParam(0,0,0,0,0);
    emit stimEnd();
}

int QDevice::Channel(){
    return channel;
}
QString QDevice::PortName(){
    return portNameLineEdit->text();
}
int QDevice::Current(){
    return current;
}
int QDevice::Frequency(){
    return frequency;
}
int QDevice::StimDuration(){
    return stimDuration;
}
int QDevice::WaveForm(){
    return waveForm;
}
int QDevice::TransitionDuration(){
    return transitionDuration;
}
int QDevice::TransitionForm(){
    return transitionForm;
}
bool QDevice::IsStimulate(){
    return isStimulate;
}
/*
bool QDevice::IsAvailable(){
    return isAvailable;
}
*/
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

/*
void QDevice::SetIsAvailable(bool b){
    isAvailable = b;
    if(isAvailable){
        portAvailableLabel->hide();
    }else{
        portAvailableLabel->show();
    }
}
*/

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

/*
void QDevice::AskDeviceState(){
    if(!IsPortExist()) return;
    //安否確認
    qDebug() << "Send message.";
    //Cはマジックナンバー
    port->write("C", 1);
}
*/
/*
void QDevice::ReceiveDeviceState(QString data){
    if(data == QString::number(Channel())){
        qDebug() << "Device is alive.";
        //SetIsAvailable(true);
    }
}
*/

void QDevice::SendGVSParam(){
    SendGVSParam(Current(), Frequency(), WaveForm(), TransitionDuration(), TransitionForm());
}
void QDevice::SendGVSParam(int current, int frequency, int waveForm, int transitionDuration, int transitionForm){
    char dat1 = 0;
    char dat2 = 0;
    char dat3 = 0;
    char dat4 = 0;
    char dat5 = 0;
    int channel = Channel();
    //dat1
    if((channel & 0x04) != 0) dat1 += 128;
    if((channel & 0x02) != 0) dat1 += 64;
    if((channel & 0x01) != 0) dat1 += 32;
    if((current & 0x800) != 0) dat1 += 16;
    if((current & 0x400) != 0) dat1 += 8;
    if((current & 0x200) != 0) dat1 += 4;
    if((current & 0x100) != 0) dat1 += 2;
    if((current & 0x80) != 0) dat1 += 1;

    //dat2
    if ((current & 0x40) != 0) dat2 += 128;
    if ((current & 0x20) != 0) dat2 += 64;
    if ((current & 0x10) != 0) dat2 += 32;
    if ((current & 0x08) != 0) dat2 += 16;
    if ((current & 0x04) != 0) dat2 += 8;
    if ((current & 0x02) != 0) dat2 += 4;
    if ((current & 0x01) != 0) dat2 += 2;
    if ((frequency & 0x200) != 0) dat2 += 1;

    //dat3
    if ((frequency & 0x100) != 0) dat3 += 128;
    if ((frequency & 0x80) != 0) dat3 += 64;
    if ((frequency & 0x40) != 0) dat3 += 32;
    if ((frequency & 0x20) != 0) dat3 += 16;
    if ((frequency & 0x10) != 0) dat3 += 8;
    if ((frequency & 0x08) != 0) dat3 += 4;
    if ((frequency & 0x04) != 0) dat3 += 2;
    if ((frequency & 0x02) != 0) dat3 += 1;

    //dat4
    if ((frequency & 0x01) != 0) dat4 += 128;
    if ((waveForm & 0x04) != 0) dat4 += 64;
    if ((waveForm & 0x02) != 0) dat4 += 32;
    if ((waveForm & 0x01) != 0) dat4 += 16;
    if ((transitionDuration & 0x08) != 0) dat4 += 8;
    if ((transitionDuration & 0x04) != 0) dat4 += 4;
    if ((transitionDuration & 0x02) != 0) dat4 += 2;
    if ((transitionDuration & 0x01) != 0) dat4 += 1;

    //dat5
    if ((transitionForm & 0x04) != 0) dat5 += 128;
    if ((transitionForm & 0x02) != 0) dat5 += 64;
    if ((transitionForm & 0x01) != 0) dat5 += 32;

    char buffer[6] = {71, dat1, dat2, dat3, dat4, dat5};
    port->write(buffer, sizeof(buffer));
}
