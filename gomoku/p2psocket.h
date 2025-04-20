#ifndef P2PSOCKET_H
#define P2PSOCKET_H
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>

class P2PSocket : public QObject
{
    Q_OBJECT
public:
    explicit P2PSocket(QObject *parent = nullptr);
    ~P2PSocket();
    void start();
    bool connectTarget(const QHostAddress &addr);
    bool sendPoint(const QString &str);
    void receiveMsg();
    void onNewConnection();
    QHostAddress addr;
    QTcpServer *server;
    QTcpSocket *socket;
    bool connecting = false;
private:
    QObject *parent;
    bool isConnected=false;

signals:
    void newConnectionAccept();
};

#endif // P2PSOCKET_H
