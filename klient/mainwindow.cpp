#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
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
    ui->middleCardValue->setStyleSheet("QLabel { color : black; }");
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
        left = 0;
        middle = 1;
        right = 2;


        //odbieranie mojego numerka
        //odbieranie nicków
        start = false;
    }
    else{
        //odbieram: karta-stół, czyja kolej, ilość kart każdego z graczy
        char msg[10];
        tcpSocket->readLine(msg, 10);

        tableCard[0] = msg[0];
        tableCard[1] = msg[1];
        tableCard[2] = msg[2];

        update_table_card(tableCard);
        //to nie koniec
        //ogarnąć ilość kart każdego z graczy
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

void MainWindow::on_previousCard_clicked()
{
    int l, m, r;
    if(left == int(myDeck.size()) - 1)
        l = 0;
    else
        l = left + 1;

    if(middle == int(myDeck.size()) - 1)
        m = 0;
    else
        m = middle + 1;

    if(right == int(myDeck.size()) - 1)
        r = 0;
    else
        r = right + 1;

    update_cards_in_hand(l, m, r);
    left = l;
    middle = m;
    right = r;
}

void MainWindow::on_nextCard_clicked()
{
    int l, m, r;
    if(left == 0)
        l = myDeck.size() - 1;
    else
        l = left -1;

    if(middle == 0)
        m = myDeck.size() - 1;
    else
        m = middle - 1;

    if(right == 0)
        r = myDeck.size() - 1;
    else
        r = right - 1;

    update_cards_in_hand(l, m, r);
    left = l;
    middle = m;
    right = r;
}

void MainWindow::on_throwCard_clicked()
{
    char card[3];
    card[0] = myDeck[middle][0];
    card[1] = myDeck[middle][1];
    card[2] = myDeck[middle][2];

    tcpSocket->write(card, 3);

    myDeck.erase(myDeck.begin() + middle - 1);

    if(myDeck.size() >= 3){
        if(middle > int(myDeck.size()) - 1){
            middle = 0;
            right = 1;
        }
        else if(right > int(myDeck.size()) - 1)
            right = 0;
    }
    else if(myDeck.size() < 3)
    {
        if(left >= int(myDeck.size()))
            left = -1;
        if(middle >= int(myDeck.size()))
            middle = -1;
        if(right >= int(myDeck.size()))
            right = -1;
    }

    update_cards_in_hand(left, middle, right);
}

void MainWindow::on_unoButton_clicked()
{

}

void MainWindow::on_takeCard_clicked()
{

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

//podaje już ustawione indeksy
void MainWindow::update_cards_in_hand(int l, int m, int r)
{
    char color, specialCard, value;

    for(int i = 0; i < 3; i++)
    {
        if(l == -1){
            ui->leftCardValue->hide();
            ui->leftCard->hide();
            continue;
        }
        else if(m == -1){
            ui->middleCardValue->hide();
            ui->middleCard->hide();
            continue;
        }
        else if(r == -1){
            ui->rightCardValue->hide();
            ui->rightCard->hide();
            continue;
        }

        if(i == 0)  //lewa karta
        {
            color = myDeck[l][0];
            specialCard = myDeck[l][1];
            value = myDeck[l][2];

            switch (color){
            case 'n':
                ui->leftCard->setCurrentWidget(ui->leftPageBlue);
                break;
            case 'y':
                ui->leftCard->setCurrentWidget(ui->leftPageYellow);
                break;
            case 'z':
                ui->leftCard->setCurrentWidget(ui->leftPageGreen);
                break;
            case 'c':
                ui->leftCard->setCurrentWidget(ui->leftPageRed);
                break;
            case 'b':
                ui->leftCard->setCurrentWidget(ui->leftPageBlack);
                break;
            }

            if(specialCard == '0')
            {
                switch (value){
                case '0':
                    ui->leftCardValue->setText("0");
                    break;
                case '1':
                    ui->leftCardValue->setText("1");
                    break;
                case '2':
                    ui->leftCardValue->setText("2");
                    break;
                case '3':
                    ui->leftCardValue->setText("3");
                    break;
                case '4':
                    ui->leftCardValue->setText("4");
                    break;
                case '5':
                    ui->leftCardValue->setText("5");
                    break;
                case '6':
                    ui->leftCardValue->setText("6");
                    break;
                case '7':
                    ui->leftCardValue->setText("7");
                    break;
                case '8':
                    ui->leftCardValue->setText("8");
                    break;
                case '9':
                    ui->leftCardValue->setText("9");
                    break;
                }
            }
            else{
                switch (value){
                case '0':
                    ui->leftCardValue->setText("<>");
                    break;
                case '1':
                    ui->leftCardValue->setText("Ø");
                    break;
                case '2':
                    ui->leftCardValue->setText("+2");
                    break;
                case '3':
                    ui->leftCardValue->setText("");
                    ui->stackedWidget->setCurrentWidget(ui->chooseCardPage);
                    break;
                case '4':
                    ui->leftCardValue->setText("+4");
                }
            }
        }
        else if(i == 1) //środkowa karta
        {
            color = myDeck[m][0];
            specialCard = myDeck[m][1];
            value = myDeck[m][2];

            switch (color){
            case 'n':
                ui->middleCard->setCurrentWidget(ui->middlePageBlue);
                break;
            case 'y':
                ui->middleCard->setCurrentWidget(ui->middlePageYellow);
                break;
            case 'z':
                ui->middleCard->setCurrentWidget(ui->middlePageGreen);
                break;
            case 'c':
                ui->middleCard->setCurrentWidget(ui->middlePageRed);
                break;
            case 'b':
                ui->middleCard->setCurrentWidget(ui->middlePageBlack);
                break;
            }

            if(specialCard == '0')
            {
                switch (value){
                case '0':
                    ui->middleCardValue->setText("0");
                    break;
                case '1':
                    ui->middleCardValue->setText("1");
                    break;
                case '2':
                    ui->middleCardValue->setText("2");
                    break;
                case '3':
                    ui->middleCardValue->setText("3");
                    break;
                case '4':
                    ui->middleCardValue->setText("4");
                    break;
                case '5':
                    ui->middleCardValue->setText("5");
                    break;
                case '6':
                    ui->middleCardValue->setText("6");
                    break;
                case '7':
                    ui->middleCardValue->setText("7");
                    break;
                case '8':
                    ui->middleCardValue->setText("8");
                    break;
                case '9':
                    ui->middleCardValue->setText("9");
                    break;
                }
            }
            else{
                switch (value){
                case '0':
                    ui->middleCardValue->setText("<>");
                    break;
                case '1':
                    ui->middleCardValue->setText("Ø");
                    break;
                case '2':
                    ui->middleCardValue->setText("+2");
                    break;
                case '3':
                    ui->middleCardValue->setText("");
                    ui->stackedWidget->setCurrentWidget(ui->chooseCardPage);
                    break;
                case '4':
                    ui->middleCardValue->setText("+4");
                }
            }
        }
        else{   //prawa karta
            color = myDeck[r][0];
            specialCard = myDeck[r][1];
            value = myDeck[r][2];

            switch (color){
            case 'n':
                ui->rightCard->setCurrentWidget(ui->rightPageBlue);
                break;
            case 'y':
                ui->rightCard->setCurrentWidget(ui->rightPageYellow);
                break;
            case 'z':
                ui->rightCard->setCurrentWidget(ui->rightPageGreen);
                break;
            case 'c':
                ui->rightCard->setCurrentWidget(ui->rightPageRed);
                break;
            case 'b':
                ui->rightCard->setCurrentWidget(ui->rightPageBlack);
                break;
            }

            if(specialCard == '0')
            {
                switch (value){
                case '0':
                    ui->rightCardValue->setText("0");
                    break;
                case '1':
                    ui->rightCardValue->setText("1");
                    break;
                case '2':
                    ui->rightCardValue->setText("2");
                    break;
                case '3':
                    ui->rightCardValue->setText("3");
                    break;
                case '4':
                    ui->rightCardValue->setText("4");
                    break;
                case '5':
                    ui->rightCardValue->setText("5");
                    break;
                case '6':
                    ui->rightCardValue->setText("6");
                    break;
                case '7':
                    ui->rightCardValue->setText("7");
                    break;
                case '8':
                    ui->rightCardValue->setText("8");
                    break;
                case '9':
                    ui->rightCardValue->setText("9");
                    break;
                }
            }
            else{
                switch (value){
                case '0':
                    ui->rightCardValue->setText("<>");
                    break;
                case '1':
                    ui->rightCardValue->setText("Ø");
                    break;
                case '2':
                    ui->rightCardValue->setText("+2");
                    break;
                case '3':
                    ui->rightCardValue->setText("");
                    ui->stackedWidget->setCurrentWidget(ui->chooseCardPage);
                    break;
                case '4':
                    ui->rightCardValue->setText("+4");
                }
            }
        }
    }
}
