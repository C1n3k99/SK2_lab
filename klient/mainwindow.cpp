#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVector>
#include <QFile>

MainWindow::MainWindow(QWidget *parent):
      QMainWindow(parent),
      ui(new Ui::MainWindow),
      tcpSocket(new QTcpSocket(this))
{
    connect(tcpSocket, &QIODevice::readyRead, this, &MainWindow::read_data);
    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    connect(tcpSocket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::errorOccurred), this, &MainWindow::connection_error_occured);
    connect(tcpSocket, &QAbstractSocket::stateChanged, this, &MainWindow::state_changed);

    ui->setupUi(this);

    ui->leftLabelBlue->setPixmap(QPixmap("../klient/obrazki/blue.jpg"));
    ui->leftLabelYellow->setPixmap(QPixmap("../klient/obrazki/yellow.png"));
    ui->leftLabelGreen->setPixmap(QPixmap("../klient/obrazki/green.jpg"));
    ui->leftLabelRed->setPixmap(QPixmap("../klient/obrazki/red.jpg"));
    ui->leftLabelBlack->setPixmap(QPixmap("../klient/obrazki/black.jpg"));

    ui->middleLabelBlue->setPixmap(QPixmap("../klient/obrazki/blue.jpg"));
    ui->middleLabelYellow->setPixmap(QPixmap("../klient/obrazki/yellow.png"));
    ui->middleLabelGreen->setPixmap(QPixmap("../klient/obrazki/green.jpg"));
    ui->middleLabelRed->setPixmap(QPixmap("../klient/obrazki/red.jpg"));
    ui->middleLabelBlack->setPixmap(QPixmap("../klient/obrazki/black_bigger.jpg"));

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::read_data()
{
    if(start){
        char myCards[3];

        tcpSocket->readLine(tableCard, 3);
        for(int i = 0; i < 7; i++){
            tcpSocket->readLine(myCards, 3);
            myDeck.push_back(myCards);
        }
        update_table_card(tableCard);
        middle = 1;

        //odbieranie mojego numerka
        //odbieranie nicków
        start = false;
    }
    else{
        //odbieram: karta-stół, czyja kolej, ilość kart każdego z graczy

    }

}

void MainWindow::connection_error_occured()
{

}

void MainWindow::state_changed()
{
    if (tcpSocket->state() == QTcpSocket::ConnectedState)
    {
        ui->stackedWidget->setCurrentWidget(ui->gamePage);
    }
    else if (tcpSocket->state() == QTcpSocket::UnconnectedState)
    {
        ui->waitingLabel->setText("Nie można nawiązać połączenia!");
    }
}

void MainWindow::on_connectButton_clicked()
{
    tcpSocket->connectToHost(ui->yourServerAddress->text(), 1234);
    ui->stackedWidget->setCurrentWidget(ui->waitingPage);
}


void MainWindow::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->startingPage);
}

void MainWindow::on_newGameButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->startingPage);
}

void MainWindow::on_blueButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->gamePage);
    ui->demandingColor->setText("Żądany kolor:\nNIEBIESKI");
}

void MainWindow::on_yellowButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->gamePage);
    ui->demandingColor->setText("Żądany kolor:\nŻÓŁTY");
}

void MainWindow::on_greenButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->gamePage);
    ui->demandingColor->setText("Żądany kolor:\nZIELONY");
}

void MainWindow::on_redButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->gamePage);
    ui->demandingColor->setText("Żądany kolor:\nCZERWONY");
}

void MainWindow::update_table_card(char card[3])
{    //0 - kolor(n,y,z,c,b); 1 - specjalne 0=nie 1=tak; 2 - wartosc: cyferki, 0=zmiana kolejki 1=Ø 2=+2 3=zmiana koloru 4=+4
    char color = card[0];
    char specialCard = card[1];
    char value = card[2];

    switch (color){
    case 'n':
        ui->tableCard->setCurrentWidget(ui->tablePageBlue);
        break;
    case 'y':
        ui->tableCard->setCurrentWidget(ui->tablePageYellow);
        break;
    case 'z':
        ui->tableCard->setCurrentWidget(ui->tablePageGreen);
        break;
    case 'c':
        ui->tableCard->setCurrentWidget(ui->tablePageRed);
        break;
    case 'b':
        ui->tableCard->setCurrentWidget(ui->tablePageBlack);
        break;
    }

    if(specialCard == '0')
    {
        switch (value){
        case '0':
            ui->tableCardValue->setText("0");
            break;
        case '1':
            ui->tableCardValue->setText("1");
            break;
        case '2':
            ui->tableCardValue->setText("2");
            break;
        case '3':
            ui->tableCardValue->setText("3");
            break;
        case '4':
            ui->tableCardValue->setText("4");
            break;
        case '5':
            ui->tableCardValue->setText("5");
            break;
        case '6':
            ui->tableCardValue->setText("6");
            break;
        case '7':
            ui->tableCardValue->setText("7");
            break;
        case '8':
            ui->tableCardValue->setText("8");
            break;
        case '9':
            ui->tableCardValue->setText("9");
            break;
        }
    }
    else{
        switch (value){
        case '0':
            ui->tableCardValue->setText("<>");
            break;
        case '1':
            ui->tableCardValue->setText("Ø");
            break;
        case '2':
            ui->tableCardValue->setText("+2");
            break;
        case '3':
            ui->tableCardValue->setText("");
            ui->stackedWidget->setCurrentWidget(ui->chooseCardPage);
            break;
        case '4':
            ui->tableCardValue->setText("+4");
        }
    }
}

/*void update_cards_in_hand(int left, int middle, int right)
{
    char card[3];
    for(int i = 0; i < 3; i++)
    {
        if(i == 0)
            card = myDeck(left);
        if(i == 1)
            card = myDeck(middle);
        if(i == 2)
            card = myDeck(right);
    }
}*/
