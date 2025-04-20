#include "connect.h"
#include "online.h"
#include "ui_connect.h"
#include "p2psocket.h"
#include <QThread>
Connect::Connect(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Connect)
{
    ui->setupUi(this);
    socket = new P2PSocket();
    socket->start();
    QHostAddress localHost = QHostAddress::LocalHost;
    ui->label->setText(localHost.toString());
    connect(socket, &P2PSocket::newConnectionAccept, this,
            &Connect::doNewConnect);

}

Connect::~Connect()
{
    delete ui;
}

void Connect::on_pushButton_connect_clicked()
{
    bool res = false;
    QHostAddress targetAddress;
    res = targetAddress.setAddress(ui->lineEdit->text());
    res = socket->connectTarget(targetAddress);
    if(!res){
        this->ui->label->setText("fail");
        socket->connecting = false;
        return;
    }
    this->ui->label->setText("success");
    emit connectSuccess(socket);
    this->close();

}

void Connect::on_pushButton_back_clicked()
{
    emit connectFail();
    this->close();
}

void Connect::doNewConnect()
{
    emit connectSuccess(socket);
    this->close();
}

