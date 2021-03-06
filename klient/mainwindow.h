#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <vector>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void read_data();
    void connection_error_occured();
    void state_changed();
    void update_table_card(char card[3]);
    void update_cards_in_hand(int l, int m, int r);
    void throwCard();

private slots:

    void on_connectButton_clicked();

    void on_backButton_clicked();

    void on_blueButton_clicked();

    void on_yellowButton_clicked();

    void on_greenButton_clicked();

    void on_redButton_clicked();

    void on_nextCard_clicked();

    void on_previousCard_clicked();

    void on_throwCard_clicked();

    void on_unoButton_clicked();

    void on_takeCard_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpSocket;

    vector<string> myDeck;
    char tableCard[4];
    int left, middle, right;
    bool start = true;
    bool demand = false;

    //char nick1[20], nick2[20], nick3[20], nick4[20];
    //string nick;
    string nick1 = "gracz_1";
    string nick2 = "gracz_2";
    string nick3 = "gracz_3";
    string nick4 = "gracz_4";
    int numberOfCards[4] = {7, 7, 7, 7};
    int id;
    char confirm[2] = {'?', '?'};

};
#endif // MAINWINDOW_H
