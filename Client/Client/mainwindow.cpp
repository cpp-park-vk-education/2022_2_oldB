#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QMessageBox>
#include <QLabel>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <string>
#include <QDebug>
#include <string>
#include <hunspell/hunspell.hxx>
#include <QList>
#include <QDebug>


#define FREE_PORT_NUM_START 1024
#define FREE_PORT_NUM_END 65535



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), client(ui) {
    ui->setupUi(this);
    ui->inputTextEdit->setPlaceholderText("Type here");
    ui->chatTextWidget->setReadOnly(true);

    setStyleSheet("color: #f0f0f0; background-color: #262626 ");
    QString const& image_path = ":/img/send-icon.png";
    QPixmap pixmap(image_path);
    QIcon ButtonIcon(pixmap);
    ui->sendButton->setIcon(ButtonIcon);
    ui->sendButton->setIconSize(ui->sendButton->rect().size() / 1.5);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onEnterPressed()
{
    qDebug() << "ENTER PRESSED\n";
}

void MainWindow::on_startButton_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(1);
}


void MainWindow::on_loginButton_clicked()
{
    auto const login(ui->loginLine->text());
    auto const password(ui->passwordLine->text());

    qDebug() << "login =" << login << "  pass =" <<  password << '\n';

    std::string strLogin = login.toStdString();
    std::string strPassword = password.toStdString();

    if (client.Authorization(strLogin, strPassword)) {
        std::vector<int> userPorts;
        if (client.GetUsersPorts(userPorts)) {
            qDebug() << "Порты считаны";
        }

        QString strPort;
        ui->roomsList->clear();
        for (auto port : userPorts) {
            strPort = QString::number(port);
            ui->roomsList->addItem(strPort);
        }

        ui->stackedWidget_2->setCurrentIndex(3);
    }
    else {
        qDebug() << "ошибка авторизации";
    }

}


void MainWindow::on_joinRoomButton_clicked()
{
    QString roomPort = ui->roomsList->currentItem()->text();
    bool correctConverted;
    int portNum = roomPort.toInt(&correctConverted);
    if (correctConverted && portNum > FREE_PORT_NUM_START && portNum < FREE_PORT_NUM_END) {
        qDebug() << "SUCCESS! port = " << portNum << '\n';
        client.ConnectToChat(portNum); // CONNECTING FIXME

        ui->stackedWidget->setCurrentIndex(1);
    }
}


void MainWindow::on_exitButton_clicked()
{
    QApplication::quit();
}


void MainWindow::on_exitButton_4_clicked()
{
    QApplication::quit();
}


void MainWindow::on_backButton_4_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}


void MainWindow::on_backButton_8_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}


void MainWindow::on_exitButton_9_clicked()
{
    QApplication::quit();
}


void MainWindow::on_exitLobbyButton_clicked()
{
    QApplication::quit();
}


void MainWindow::on_leaveLobbyButton_clicked()
{
    client.DisconnectToChat();
    ui->chatTextWidget->clear();
    ui->stackedWidget->setCurrentIndex(0);
    ui->stackedWidget_2->setCurrentIndex(3); // rooms list
}


void MainWindow::on_createNewAccBtn_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);

}


void MainWindow::on_createAccBtn_clicked()
{
    auto const login(ui->newLoginLine->text());
    auto const password(ui->newPassLine->text());

    qDebug() << "new_login =" << login << "   new_pass =" <<  password << '\n';

    std::string strLogin = login.toStdString();
    std::string strPassword = password.toStdString();

    if (client.Registration(strLogin, strPassword)) {
        ui->stackedWidget_2->setCurrentIndex(1);
    }
    else {
        qDebug() << "ошибка регистрации";
    }
}


void MainWindow::on_returnFromRoomsBtn_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(0);
}

void MainWindow::on_goToCreateRoomButton_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(4);
}


void MainWindow::on_createRoomButton_clicked()
{
    auto const newRoomPort(ui->newRoomPort->text());

    qDebug() << "new_room_posrt =" << newRoomPort;

    std::string strPort = newRoomPort.toStdString();

//    if (client.СreateNewRoom(strPort)) {
//        std::vector<int> userPorts;
//        if (client.GetUsersPorts(userPorts)) {
//            qDebug() << "Порты считаны";
//        }

//        QString strPort;
//        for (auto port : userPorts) {
//            strPort = QString::number(port);
//            ui->roomsList->addItem(strPort);
//        }

//        ui->stackedWidget_2->setCurrentIndex(3);
//    }
//    else {
//        qDebug() << "ошибка создания комнаты";
//    }
}

bool MakeDecision(QString message) {
    Hunspell spell ("../spell/en_US.aff", "../spell/en_US.dic");
    bool result = true;
    QStringList words = message.split(' ');

    for (QString word : words) {
        if (spell.spell(word.toStdString().c_str()) == 0) {
                result = false;
                break;
        }
    }
    return result;
}


void MainWindow::on_sendButton_clicked()
{
    sendMessage();

}

void MainWindow::sendMessage() {
    if (!ui->inputTextEdit->toPlainText().isEmpty()) {

        QString message(ui->inputTextEdit->toPlainText());

        if (MakeDecision(message)) {
    //        std::string message(ui->inputTextEdit->toPlainText().toStdString());
            std::string strMsg = message.toStdString();
            client.WriteMessage(strMsg); //FIXME
            qDebug() << "Сообщение отправлено!";
            ui->inputTextEdit->clear();

        }
        else {
            QMessageBox::warning(this, "Внимание","Ваше сообщение содержит ошибку");

        }

    }
}

void ChatClient::do_read_body() {
    boost::asio::async_read(socket_, boost::asio::buffer(read_message_.inf(), read_message_.inf_length()),
        [this](boost::system::error_code ec, std::size_t /*length*/)
        {
            if (!ec && read_message_.decode_text()) {                
                qDebug() << "заходит улитка в бар";

                // вызов м-да, который выводит сообщения на экран
                ui->chatTextWidget->appendPlainText(QString::fromStdString(read_message_.get_username() + ": " + read_message_.get_body()));
                do_read_header();                                                    // сразу же начинаем читать следующее, если оно пришло
            }
            else
                socket_.close();
        });
}

void MainWindow::on_backButton_10_clicked()
{
    ui->stackedWidget_2->setCurrentIndex(2);
}


void MainWindow::on_exitButton_11_clicked()
{
    QApplication::quit();
}

