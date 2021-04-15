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

    //移動
    this->setFixedSize(180, 240);
    this->SetID(deviceID);

    removeButton->setGeometry(100,10,70,28);

    idSpinBox->setGeometry(100, 67, 70, 22);
    portNamePlainTextEdit->setGeometry(100, 97, 70, 22);
    currentSpinBox->setGeometry(100, 127, 70, 22);
    frequencySpinBox->setGeometry(100, 157, 70, 22);
    durationSpinBox->setGeometry(100, 187, 70, 22);

    idLabel->setGeometry(10, 70, 80, 15);
    portNameLabel->setGeometry(10, 100, 80, 15);
    currentLabel->setGeometry(10, 130, 80, 15);
    frequencyLabel->setGeometry(10, 160, 80, 15);
    durationLabel->setGeometry(10, 190, 80, 15);

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
}

void QDevice::SetID(int id){
    this->setTitle("Device " + QString::number(id));
    this->move(10 + 190 * id, 60);
}

void QDevice::RemoveDevice(){
    qDebug() << "Remove Device.";
    emit removed(this);
    delete this;
}
