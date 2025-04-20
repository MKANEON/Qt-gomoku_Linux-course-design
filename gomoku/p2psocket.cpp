#include "p2psocket.h"
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
P2PSocket::P2PSocket(QObject *parent)
{

}

P2PSocket::~P2PSocket()
{
    server->close();
    delete server;
}

void P2PSocket::start()
{
    server = new QTcpServer();
    server->listen(QHostAddress::Any, 19862);
    connect(server, &QTcpServer::newConnection, this,
            &P2PSocket::onNewConnection);
}

void P2PSocket::onNewConnection(){

    if (!isConnected){
        socket = server->nextPendingConnection();
        isConnected = true;
        emit newConnectionAccept();
    }
}

bool P2PSocket::connectTarget(const QHostAddress &addr)
{

    server->close();
    socket = new QTcpSocket();
    socket->connectToHost(addr, 19862);
    isConnected = socket->waitForConnected(1000);
    if(!isConnected) server->listen(QHostAddress::Any, 19862);

    return isConnected;
}

bool P2PSocket::sendPoint(const QString &str)
{
    bool res = socket->write(str.toLocal8Bit());
    return res;
}

