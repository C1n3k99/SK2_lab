#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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

}

void MainWindow::on_exitGame_clicked()
{

}
