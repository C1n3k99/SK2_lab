#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent):
      QMainWindow(parent),
      ui(new Ui::MainWindow),
      tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);
    //ui->leftCardValue->setStyleSheet("QLabel {  color : white; }");
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connect_clicked()
{
    tcpSocket->connectToHost("127.0.0.1", 1234);
}

void MainWindow::on_exitGame_clicked()
{

}
