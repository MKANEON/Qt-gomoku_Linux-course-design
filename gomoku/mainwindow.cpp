#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "board.h"
#include "settings.h"
#include "offline.h"
#include "online.h"
#include "connect.h"
#include <QVBoxLayout>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleSettings(Settings *settings){

    sound = settings->sound;
    chess = settings->chess;
    prompt = settings->prompt;
    sets[0] = settings->sound;
    sets[1] = settings->chess;
    sets[2] = settings->prompt;
    settings->close();
    //this->show();

}

void MainWindow::handleConnect(P2PSocket *socket)
{
    //this->show();
    Online *online = new Online(socket);
    online->setSets(sets);
    online->show();
    //this->hide();
    connect(online, &Online::gameTerminate, this, &MainWindow::showMainwindow);


}

void MainWindow::showMainwindow()
{
    this->show();
}

void MainWindow::on_pushButton_offline_clicked()
{

    Offline *offline = new Offline(this);
    connect(offline, &Offline::gameTerminate, this, &MainWindow::showMainwindow);
    offline->setSets(sets);
    offline->show();
    this->hide();

}

void MainWindow::on_pushButton_online_clicked()
{

    Connect *conn = new Connect();
    conn->show();
    this->hide();
    connect(conn, &Connect::connectSuccess, this, &MainWindow::handleConnect);
    connect(conn, &Connect::connectFail, this, &MainWindow::showMainwindow);

}


void MainWindow::on_pushButton_settings_clicked()
{

    Settings *settings = new Settings(nullptr, sound=sound, chess=chess, prompt=prompt);

    settings->show();
    //this->hide();
    connect(settings, &Settings::backSettings, this, &MainWindow::handleSettings);


}


void MainWindow::on_pushButton_exit_clicked()
{
    this->close();
}

