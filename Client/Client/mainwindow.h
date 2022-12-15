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


    void on_startButton_clicked();

    void on_loginButton_clicked();

    void on_joinRoomButton_clicked();

private:
    Ui::MainWindow *ui;
    Client client;

    void write(std::string &message);
};
#endif // MAINWINDOW_H
