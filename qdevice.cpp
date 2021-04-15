#include "qdevice.h"

QDevice::QDevice(QWidget *parent) : QGroupBox(parent)
{
    this->setTitle("Device 0");
    this->setFixedSize(180, 240);
    //Widget生成
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
    idSpinBox->setGeometry(100, 37, 70, 22);
    portNamePlainTextEdit->setGeometry(100, 67, 70, 22);
    currentSpinBox->setGeometry(100, 97, 70, 22);
    frequencySpinBox->setGeometry(100, 127, 70, 22);
    durationSpinBox->setGeometry(100, 157, 70, 22);

    idLabel->setGeometry(10, 40, 80, 15);
    portNameLabel->setGeometry(10, 70, 80, 15);
    currentLabel->setGeometry(10, 100, 80, 15);
    frequencyLabel->setGeometry(10, 120, 80, 15);
    durationLabel->setGeometry(10, 150, 80, 15);

    //コールバック設定

    //値の設定
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
