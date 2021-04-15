#ifndef QDEVICE_H
#define QDEVICE_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QPlainTextEdit>

class QDevice : public QGroupBox
{
    Q_OBJECT
public:
    explicit QDevice(QWidget *parent = nullptr);

private:
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

signals:

};

#endif // QDEVICE_H
