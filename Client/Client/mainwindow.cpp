#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <string>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::on_pushButton_clicked() {
    std::string tmp = ui->lineEdit->text().toStdString();
    client.WriteMessage(tmp);
}

void MainWindow::write(std::string &message) {
    ui->lineEdit_2->insert(QString::fromStdString(message));
}

