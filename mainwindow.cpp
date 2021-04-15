#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdevice.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->label->setText("HOGE");
    delete ui->pushButton;
    /*
    QLabel* l = new QLabel(window());
    l->setText("GUFAA");
    l->move(100,100);
    l->show();
    */

    QDevice* qd = new QDevice(window());
    qd->move(10,10);
    qd->show();
}
