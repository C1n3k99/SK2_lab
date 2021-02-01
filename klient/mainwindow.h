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

private slots:

    void on_connectButton_clicked();

    void on_backButton_clicked();

    void on_newGameButton_clicked();

    void on_blueButton_clicked();

    void on_yellowButton_clicked();

    void on_greenButton_clicked();

    void on_redButton_clicked();

    void on_nextCard_clicked();

    void on_previousCard_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *tcpSocket;
    vector<string> myDeck;
    char tableCard[3];
    int left, middle, right;
    bool start = true;
};
#endif // MAINWINDOW_H
