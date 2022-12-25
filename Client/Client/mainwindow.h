#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void onEnterPressed();


    void on_startButton_clicked();

    void on_loginButton_clicked();

    void on_joinRoomButton_clicked();

//    void on_exitButton_clicked();

//    void on_exitButton_4_clicked();

    void on_backButton_4_clicked();

    void on_backButton_8_clicked();

//    void on_exitButton_9_clicked();

//    void on_exitLobbyButton_clicked();

    void on_leaveLobbyButton_clicked();

    void on_createNewAccBtn_clicked();

    void on_createAccBtn_clicked();

    void on_returnFromRoomsBtn_clicked();

    void on_sendButton_clicked();


    void on_createRoomButton_clicked();

    void on_goToCreateRoomButton_clicked();

    void on_getStatButton_clicked();

    void on_backButton_10_clicked();

//    void on_exitButton_11_clicked();

private:
    void sendMessage();

    Ui::MainWindow *ui;
    Client client;

    void write(std::string &message);
};

#endif // MAINWINDOW_H
