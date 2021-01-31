#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent):
      QMainWindow(parent),
      ui(new Ui::MainWindow),
      tcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);

    ui->leftLabelBlue->setPixmap(QPixmap("../klient/obrazki/blue.jpg"));
    ui->leftLabelYellow->setPixmap(QPixmap("../klient/obrazki/yellow.png"));
    ui->leftLabelGreen->setPixmap(QPixmap("../klient/obrazki/green.jpg"));
    ui->leftLabelRed->setPixmap(QPixmap("../klient/obrazki/red.jpg"));
    ui->leftLabelBlack->setPixmap(QPixmap("../klient/obrazki/black.jpg"));

    ui->centerLabelBlue->setPixmap(QPixmap("../klient/obrazki/blue.jpg"));
    ui->centerLabelYellow->setPixmap(QPixmap("../klient/obrazki/yellow.png"));
    ui->centerLabelGreen->setPixmap(QPixmap("../klient/obrazki/green.jpg"));
    ui->centerLabelRed->setPixmap(QPixmap("../klient/obrazki/red.jpg"));
    ui->centerLabelBlack->setPixmap(QPixmap("../klient/obrazki/black_bigger.jpg"));

    ui->rightLabelBlue->setPixmap(QPixmap("../klient/obrazki/blue.jpg"));
    ui->rightLabelYellow->setPixmap(QPixmap("../klient/obrazki/yellow.png"));
    ui->rightLabelGreen->setPixmap(QPixmap("../klient/obrazki/green.jpg"));
    ui->rightLabelRed->setPixmap(QPixmap("../klient/obrazki/red.jpg"));
    ui->rightLabelBlack->setPixmap(QPixmap("../klient/obrazki/black.jpg"));

    ui->tableLabelBlue->setPixmap(QPixmap("../klient/obrazki/blue.jpg"));
    ui->tableLabelYellow->setPixmap(QPixmap("../klient/obrazki/yellow.png"));
    ui->tableLabelGreen->setPixmap(QPixmap("../klient/obrazki/green.jpg"));
    ui->tableLabelRed->setPixmap(QPixmap("../klient/obrazki/red.jpg"));
    ui->tableLabelBlack->setPixmap(QPixmap("../klient/obrazki/black.jpg"));

    ui->leftCardValue->setStyleSheet("QLabel {  color : black; }");
    ui->centerCardValue->setStyleSheet("QLabel { color : black; }");
    ui->rightCardValue->setStyleSheet("QLabel { color : black; }");
    ui->tableCardValue->setStyleSheet("QLabel { color : black; }");



/*
    ui->leftLabelBlue->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->leftLabelYellow->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->leftLabelGreen->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->leftLabelRed->setStyleSheet("QLabel { border : 3px solid black; }");

    ui->centerLabelBlue->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->centerLabelYellow->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->centerLabelGreen->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->centerLabelRed->setStyleSheet("QLabel { border : 3px solid black; }");

    ui->rightLabelBlue->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->rightLabelYellow->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->rightLabelGreen->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->rightLabelRed->setStyleSheet("QLabel { border : 3px solid black; }");

    ui->tableLabelBlue->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->tableLabelYellow->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->tableLabelGreen->setStyleSheet("QLabel { border : 3px solid black; }");
    ui->tableLabelRed->setStyleSheet("QLabel { border : 3px solid black; }");*/
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
