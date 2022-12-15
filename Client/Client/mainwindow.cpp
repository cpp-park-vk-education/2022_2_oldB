#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QMessageBox>
#include <QLabel>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <string>
#include <QDebug>

#define FREE_PORT_NUM_START 1024
#define FREE_PORT_NUM_END 65535

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->inputTextEdit->setPlaceholderText("Type here");
    ui->chatTextWidget->setReadOnly(true);

    setStyleSheet("color: #f0f0f0; background-color: #262626 ");
    ;

    ui->roomsList->addItem("bebra");
    ui->roomsList->addItem("abobus");


//    Utility::setButtonIcon(ui->emojiButton, ":/img/img/emoji.png", 1.5);
//    Utility::setButtonIcon(ui->attachmentsButton, ":/img/img/clip.png", 1.5);
}

MainWindow::~MainWindow() {
    delete ui;
}



//void MainWindow::on_pushButton_clicked() {
////    std::string tmp = ui->lineEdit->text().toStdString();
////    client.WriteMessage(tmp);
//}

//void MainWindow::write(std::string &message) {
////    ui->lineEdit_2->insert(QString::fromStdString(message));
//}




void MainWindow::on_startButton_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}


void MainWindow::on_loginButton_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(3);
}


void MainWindow::on_joinRoomButton_clicked()
{
    QString roomPort = ui->roomsList->currentItem()->text();
    bool correctConverted;
    int portNum = roomPort.toInt(&correctConverted);
    if (correctConverted && portNum > FREE_PORT_NUM_START && portNum < FREE_PORT_NUM_END) {
        qDebug() << "SUCCESS! port = " << portNum << '\n';
//        client.ConnectToChat(portNum);

    }


//    ui->roomsList->currentItem()->text()
}

