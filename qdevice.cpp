#include "qdevice.h"
#include <QtCore/QDebug>

QDevice::QDevice(int deviceID, QWidget *parent) : QGroupBox(parent)
{
    qDebug() << "Hello Device.";
    //Widget生成
    removeButton = new QPushButton(this);

    idSpinBox = new QSpinBox(this);
    portNamePlainTextEdit = new QPlainTextEdit(this);
    currentSpinBox = new QSpinBox(this);
    frequencySpinBox = new QSpinBox(this);
    durationSpinBox = new QSpinBox(this);

    idLabel = new QLabel(this);
    portNameLabel = new QLabel(this);
    currentLabel = new QLabel(this);
    frequencyLabel = new QLabel(this);
    durationLabel = new QLabel(this);

    portErrorLabel = new QLabel(this);

    stimulateStateLabel = new QLabel(this);

    //移動
    this->setFixedSize(180, 240);
    this->SetID(deviceID);

    removeButton->setGeometry(100,10,70,28);

    idSpinBox->setGeometry(90, 67, 80, 22);
    portNamePlainTextEdit->setGeometry(90, 97, 80, 22);
    currentSpinBox->setGeometry(90, 127, 80, 22);
    frequencySpinBox->setGeometry(90, 157, 80, 22);
    durationSpinBox->setGeometry(90, 187, 80, 22);

    idLabel->setGeometry(10, 70, 80, 15);
    portNameLabel->setGeometry(10, 100, 80, 15);
    currentLabel->setGeometry(10, 130, 80, 15);
    frequencyLabel->setGeometry(10, 160, 80, 15);
    durationLabel->setGeometry(10, 190, 80, 15);

    portErrorLabel->setGeometry(10, 40, 160,15);

    stimulateStateLabel->setGeometry(90, 217, 80, 15);

    //コールバック設定
    connect(removeButton, &QPushButton::clicked, this, [=](){
            RemoveDevice();
    });
    //値の設定
    removeButton->setText("Remove");

    idSpinBox->setValue(0);
    portNamePlainTextEdit->setPlainText("COM0");
    currentSpinBox->setValue(500);
    frequencySpinBox->setValue(80);
    durationSpinBox->setValue(10);

    idLabel->setText("ID:");
    portNameLabel->setText("PortName:");
    currentLabel->setText("Current:");
    frequencyLabel->setText("Frequency:");
    durationLabel->setText("Duration:");

    portErrorLabel->setText("Port doesn't exist.");
    QPalette palette = portErrorLabel->palette();
    palette.setColor(QPalette::Foreground, QColor("#FF0000"));
    portErrorLabel->setPalette(palette);
    portErrorLabel->hide();
    //最初は無刺激
    SetStimulateState(false);
}

void QDevice::SetID(int id){
    this->setTitle("Device " + QString::number(id));
    this->move(10 + 190 * id, 60);
}

void QDevice::RemoveDevice(){
    //接続の停止
    Disconnect();
    qDebug() << "Remove Device.";
    emit removed(this);
    delete this;
}

void QDevice::Connect(QSerialPort* serialPort){
    qDebug() << portNamePlainTextEdit->toPlainText();
    port = serialPort;
}
void QDevice::Disconnect(){

}

QString QDevice::GetPortName(){
    return portNamePlainTextEdit->toPlainText();
}

void QDevice::SetPortExist(bool portExist){
    if(portExist){
        portErrorLabel->hide();
    }else{
        portErrorLabel->show();
    }
}

void QDevice::SetStimulateState(bool b){
    isStimulate = b;
    if(isStimulate){
        stimulateStateLabel->setText("Stim");
        QPalette palette = stimulateStateLabel->palette();
        palette.setColor(QPalette::Foreground, QColor("#00FF00"));
        stimulateStateLabel->setPalette(palette);
        stimulateStateLabel->hide();
    }else{
        stimulateStateLabel->setText("Stay");
        QPalette palette = stimulateStateLabel->palette();
        palette.setColor(QPalette::Foreground, QColor("#FF0000"));
        stimulateStateLabel->setPalette(palette);
        stimulateStateLabel->hide();
    }
}

void QDevice::SendGVSParam(){
    SendGVSParam(currentSpinBox->value(), frequencySpinBox->value());
}
void QDevice::SendGVSParam(int current, int frequency, int mode){
    char dat1, dat2, dat3, dat4;
    int channel = idSpinBox->value();
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
    if ((mode & 0x04) != 0) dat4 += 128;
    if ((mode & 0x02) != 0) dat4 += 64;
    if ((mode & 0x01) != 0) dat4 += 32;

    char buffer[5] = {71, dat1, dat2, dat3, dat4};
    port->write(buffer);
}
