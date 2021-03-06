#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <stdio.h>
#include<string.h>
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
    if(start){              //pierwszy komunikat
        char myId[3];
        tcpSocket->readLine(myId, 3);
        id = myId[0] - '0';

        tcpSocket->readLine(tableCard, 4);
        update_table_card(tableCard);

        /*
        char msg[27];
        string message = "";
        char myCards[7][3];
        do
        {
            tcpSocket->readLine(msg, 27);
            message += msg;
        } while(tcpSocket->bytesAvailable());

        sscanf(message.c_str(), "%[^;];%[^;];%[^;];%[^;];%[^;];%[^;];%[^;]",  myCards[0],
                myCards[1], myCards[2], myCards[3], myCards[4], myCards[5], myCards[6]);

        for(int i = 0; i < 7; i ++){
            myDeck.push_back(myCards[i]);
        }*/

        char myCards[4];
        for(int i = 0; i < 7; i++){
            tcpSocket->readLine(myCards, 4);
            myDeck.push_back(myCards);
        }

        left = 0;
        middle = 1;
        right = 2;
        update_cards_in_hand(left, middle, right);
        /*char allNicks[83];
        do
        {
            tcpSocket->readLine(allNicks, 83);
            nick += allNicks;
        } while(tcpSocket->bytesAvailable());

        sscanf(nick.c_str(), "%[^;];%[^;];%[^;];%[^;]",  nick1, nick2, nick3, nick4);*/



        if(id == 0)
        {
            ui->player1->setText(QString::fromStdString(nick2));
            ui->player2->setText(QString::fromStdString(nick3));
            ui->player3->setText(QString::fromStdString(nick4));
        }
        else if(id == 1)
        {
            ui->player3->setText(QString::fromStdString(nick1));
            ui->player1->setText(QString::fromStdString(nick3));
            ui->player2->setText(QString::fromStdString(nick4));
        }
        else if(id == 2)
        {
            ui->player2->setText(QString::fromStdString(nick1));
            ui->player3->setText(QString::fromStdString(nick2));
            ui->player1->setText(QString::fromStdString(nick4));
        }
        else
        {
            ui->player1->setText(QString::fromStdString(nick1));
            ui->player2->setText(QString::fromStdString(nick2));
            ui->player3->setText(QString::fromStdString(nick3));
        }

        /*
        if(strcmp(nick1, ui->yourNick->text().toStdString().c_str()) == 0)
        {
            id = 0;
            ui->player1->setText(nick2);
            ui->player2->setText(nick3);
            ui->player3->setText(nick4);
        }
        else if(strcmp(nick2, ui->yourNick->text().toStdString().c_str()) == 0)
        {
            id = 1;
            ui->player3->setText(nick1);
            ui->player1->setText(nick3);
            ui->player2->setText(nick4);
        }
        else if(strcmp(nick3, ui->yourNick->text().toStdString().c_str()) == 0)
        {
            id = 2;
            ui->player2->setText(nick1);
            ui->player3->setText(nick2);
            ui->player1->setText(nick4);
        }
        else
        {
            id = 3;
            ui->player1->setText(nick1);
            ui->player2->setText(nick2);
            ui->player3->setText(nick3);
        }*/
        ui->yourTurn->setText(QString::fromStdString(nick1));

        ui->stackedWidget->setCurrentWidget(ui->gamePage);
        start = false;
    }
    else{
        char msg[20];
        string message = "";
        int turn;

        do
        {
            tcpSocket->readLine(msg, 20);
            message += msg;
        } while(tcpSocket->bytesAvailable());


        if(message[0] >= '0' && message[0] <= '9')      //komunikaty cykliczne - odbieram: czyja kolej, karta-stół,  ilość kart każdego z graczy
        {
            sscanf(message.c_str(), "%d;%[^;];%d;%d;%d;%d",  &turn, tableCard,
                   &numberOfCards[0], &numberOfCards[1], &numberOfCards[2], &numberOfCards[3]);

            update_table_card(tableCard);

            if(tableCard[0] == 'b'){
                if(demand)
                {
                    int playerChoosingColor;
                    if(turn == 0)
                        playerChoosingColor = 3;
                    else
                        playerChoosingColor = turn - 1;

                    if(id == playerChoosingColor)
                    {
                        ui->stackedWidget->setCurrentWidget(ui->chooseColorPage);
                        demand = false;
                    }
                }
            }
            else
                ui->demandingColor->setText("");


            if(turn == 0)
                ui->yourTurn->setText(QString::fromStdString(nick1));
            else if(turn == 1)
                ui->yourTurn->setText(QString::fromStdString(nick2));
            else if(turn == 2)
                ui->yourTurn->setText(QString::fromStdString(nick3));
            else
                ui->yourTurn->setText(QString::fromStdString(nick4));

            if(id == 0)
            {
                ui->numberOfMyCards->setText(QString::number(numberOfCards[0]));
                ui->numberOfCardsPlayer1->setText(QString::number(numberOfCards[1]));
                ui->numberOfCardsPlayer2->setText(QString::number(numberOfCards[2]));
                ui->numberOfCardsPlayer3->setText(QString::number(numberOfCards[3]));
            }
            else if(id == 1)
            {
                ui->numberOfCardsPlayer3->setText(QString::number(numberOfCards[0]));
                ui->numberOfMyCards->setText(QString::number(numberOfCards[1]));
                ui->numberOfCardsPlayer1->setText(QString::number(numberOfCards[2]));
                ui->numberOfCardsPlayer2->setText(QString::number(numberOfCards[3]));
            }
            else if(id == 2)
            {
                ui->numberOfCardsPlayer2->setText(QString::number(numberOfCards[0]));
                ui->numberOfCardsPlayer3->setText(QString::number(numberOfCards[1]));
                ui->numberOfMyCards->setText(QString::number(numberOfCards[2]));
                ui->numberOfCardsPlayer1->setText(QString::number(numberOfCards[3]));
            }
            else
            {
                ui->numberOfCardsPlayer1->setText(QString::number(numberOfCards[0]));
                ui->numberOfCardsPlayer2->setText(QString::number(numberOfCards[1]));
                ui->numberOfCardsPlayer3->setText(QString::number(numberOfCards[2]));
                ui->numberOfMyCards->setText(QString::number(numberOfCards[3]));
            }
        }
        else if(message[0] == '!')      //komunikat o wygranej
        {
            string winnerIs = "Zwyciężył ";

            if(message[1] - '0' == 0)
                winnerIs += nick1;
            else if(message[1] - '0' == 1)
                winnerIs += nick2;
            else if(message[1] - '0' == 2)
                winnerIs += nick3;
            else
                winnerIs += nick4;

            ui->winner->setText(QString::fromStdString(winnerIs));  //string na Qstring
            ui->stackedWidget->setCurrentWidget(ui->winnerPage);
        }
        else if(message[0] == '?'){
            //char card[3];
            //card[0] = myDeck[middle][0];
            //card[1] = myDeck[middle][1];
            //card[2] = myDeck[middle][2];

            tcpSocket->write(confirm, 2);
            throwCard();
        }
        else if(message[0] == 'n' && message[1] == 'n'){
            ui->stackedWidget->setCurrentWidget(ui->gamePage);
            ui->demandingColor->setText("Żądany kolor:\nNIEBIESKI");
        }
        else if(message[0] == 'y' && message[1] == 'y'){
            ui->stackedWidget->setCurrentWidget(ui->gamePage);
            ui->demandingColor->setText("Żądany kolor:\nŻÓŁTY");
        }
        else if(message[0] == 'z' && message[1] == 'z'){
            ui->stackedWidget->setCurrentWidget(ui->gamePage);
            ui->demandingColor->setText("Żądany kolor:\nZIELONY");
        }
        else if(message[0] == 'c' && message[1] == 'c'){
            ui->stackedWidget->setCurrentWidget(ui->gamePage);
            ui->demandingColor->setText("Żądany kolor:\nCZERWONY");
        }
        else{                           //dobieranie karty
            myDeck.push_back(msg);
            if(myDeck.size() == 3)
            {
                left = 2;
                ui->leftCard->show();
                ui->leftCardValue->show();
                update_cards_in_hand(left, middle, right);
            }
            else if(myDeck.size() == 2)
            {
                right = 1;
                left = -1;
                ui->rightCard->show();
                ui->rightCardValue->show();
                update_cards_in_hand(left, middle, right);
            }
            else if(left == int(myDeck.size()) -2){
                left = myDeck.size() - 1;
                update_cards_in_hand(left, middle, right);
            }
            else if(middle == int(myDeck.size()) -2){
                right = myDeck.size() - 1;
                update_cards_in_hand(left, middle, right);
            }
        }
    }

}

void MainWindow::connection_error_occured()
{
    ui->stackedWidget->setCurrentWidget(ui->waitingPage);
    ui->waitingLabel->setText("Nie można nawiązać połączenia!");

}

void MainWindow::state_changed()
{
    if (tcpSocket->state() == QTcpSocket::ConnectedState)
    {
        //tcpSocket->write(ui->yourNick->text().toStdString().c_str(), 20);   //zamiana qstring na char
        ui->stackedWidget->setCurrentWidget(ui->waitingPage);
        ui->waitingLabel->setText("Oczekiwanie na graczy...");
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

void MainWindow::on_previousCard_clicked()
{
    int l, m, r;

    if(myDeck.size() >= 3)
    {
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
    }
    else if(myDeck.size() == 2)
    {
        l = -1;
        if(middle == 1){
            m = 0;
            r = 1;
        }
        else{
            m = 1;
            r = 0;
        }
    }
    else{
        l = -1;
        m = 0;
        r = -1;
    }

    update_cards_in_hand(l, m, r);
    left = l;
    middle = m;
    right = r;
}

void MainWindow::on_nextCard_clicked()
{
    int l, m, r;

    if(myDeck.size() >= 3)
    {
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
    }
    else if(myDeck.size() == 2)
    {
        l = -1;
        if(middle == 1){
            m = 0;
            r = 1;
        }
        else{
            m = 1;
            r = 0;
        }
    }
    else{
        l = -1;
        m = 0;
        r = -1;
    }

    update_cards_in_hand(l, m, r);
    left = l;
    middle = m;
    right = r;
}

void MainWindow::throwCard()
{
    /*char card[4];
    card[0] = id + '0';
    card[1] = myDeck[middle][0];
    card[2] = myDeck[middle][1];
    card[3] = myDeck[middle][2];

    tcpSocket->write(card, 4);*/

    if(myDeck[middle][0] == 'b')    //wcześniej card[1]
        demand = true;
    else
        demand = false;

    myDeck.erase(myDeck.begin() + middle);

    if(myDeck.size() >= 3){
        if(middle > int(myDeck.size()) - 1){
            middle = 0;
            right = 1;
        }
        else if(right > int(myDeck.size()) - 1)
            right = 0;
        else if(left > int(myDeck.size()) - 1)
            left = int(myDeck.size()) - 1;
    }
    else if(myDeck.size() == 2)
    {
        if(right > 0)
        {
            left = -1;
            middle = 0;
            right = 1;
        }
        else{
            left = -1;
            middle = 1;
            right = 0;
        }
    }
    else{
        left = -1;
        middle = 0;
        right  = -1;
    }

    update_cards_in_hand(left, middle, right);
}

void MainWindow::on_throwCard_clicked()
{
    char card[4];
    card[0] = id + '0';
    card[1] = myDeck[middle][0];
    card[2] = myDeck[middle][1];
    card[3] = myDeck[middle][2];

    tcpSocket->write(card, 4);
}

void MainWindow::on_unoButton_clicked()
{
    char uno[4];
    uno[0] = id + '0';
    uno[1] = 'u';
    uno[2] = 'n';
    uno[3] = 'o';

    tcpSocket->write(uno, 4);
}

void MainWindow::on_takeCard_clicked()
{
    char take[4];
    take[0] = id + '0';
    take[1] = 'p';
    take[2] = 'l';
    take[3] = 's';

    tcpSocket->write(take, 4);
}

void MainWindow::on_blueButton_clicked()
{
    char color[2];
    color[0] = 'n';
    color[1] = 'n';
    tcpSocket->write(color, 2);
}

void MainWindow::on_yellowButton_clicked()
{
    char color[2];
    color[0] = 'y';
    color[1] = 'y';
    tcpSocket->write(color, 2);
}

void MainWindow::on_greenButton_clicked()
{
    char color[2];
    color[0] = 'z';
    color[1] = 'z';
    tcpSocket->write(color, 2);
}

void MainWindow::on_redButton_clicked()
{
    char color[2];
    color[0] = 'c';
    color[1] = 'c';
    tcpSocket->write(color, 2);
}

void MainWindow::update_table_card(char card[4])
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
            break;
        case '4':
            ui->tableCardValue->setText("+4");
            break;
        }
    }
}

//podaje już ustawione indeksy
void MainWindow::update_cards_in_hand(int l, int m, int r)
{
    char color, specialCard, value;

    if(l == -1){
        ui->leftCardValue->hide();
        ui->leftCard->hide();
    }
    if(m == -1){
        ui->middleCardValue->hide();
        ui->middleCard->hide();
    }
    if(r == -1){
        ui->rightCardValue->hide();
        ui->rightCard->hide();
    }

    for(int i = 0; i < 3; i++)
    {
        if(i==0 && l!=-1)  //lewa karta
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
                    break;
                case '4':
                    ui->leftCardValue->setText("+4");
                    break;
                }
            }
        }
        else if(i==1 && m!=-1) //środkowa karta
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
                    break;
                case '4':
                    ui->middleCardValue->setText("+4");
                    break;
                }
            }
        }
        else if(r != -1){   //prawa karta
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
                    break;
                case '4':
                    ui->rightCardValue->setText("+4");
                    break;
                }
            }
        }
    }
}
