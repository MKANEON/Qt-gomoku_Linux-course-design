#ifndef CONNECT_H
#define CONNECT_H

#include <QWidget>
#include "connect.h"
#include "p2psocket.h"
namespace Ui {
class Connect;
}

class Connect : public QWidget
{
    Q_OBJECT

public:
    explicit Connect(QWidget *parent = nullptr);
    ~Connect();
    P2PSocket *socket;
    void startThread();

private slots:
    void on_pushButton_connect_clicked();
    void on_pushButton_back_clicked();
    void doNewConnect();

signals:
    void connectSuccess(P2PSocket *socket);
    void connectFail();
private:
    Ui::Connect *ui;
    QString targetAddress;
};

#endif // CONNECT_H
