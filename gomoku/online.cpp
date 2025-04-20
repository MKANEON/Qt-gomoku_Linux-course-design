#include "online.h"
#include "ui_online.h"
#include "board.h"
#include "connect.h"
#include "p2psocket.h"
#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTimer>
Online::Online(P2PSocket *sokt, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Online)
{

    socket = sokt;
    ui->setupUi(this);
    srand(time(0));
    board = new Board(this);
    board->setFixedSize(460, 600);
    ui->verticalLayout->addWidget(board);
    ui->pushButton_surrender->setEnabled(false);
    connect(board, &Board::cellClicked, this, &Online::handleCellClicked);
    connect(board, &Board::changeRival, this, &Online::alterRival);
    connect(this, &Online::readyAlterPlayer, board,
            &Board::changePlayer);
    connect(socket->socket, &QTcpSocket::readyRead, this,
            &Online::rivalReady);
    connect(board, &Board::hasTerminated, this, &Online::handleBoardTerminated);
    connect(this, &Online::rivalHasSurrender, board, &Board::rivalSurrender);
    timer = new QTimer(this);
    timer->start(5000); // 每秒触发一次
    connect(timer, &QTimer::timeout, this, &Online::checkConnected);


}

Online::~Online()
{
    delete ui;
}

void Online::handleCellClicked(Board *board, int row, int col){
    if(board->getPointStatus(row, col) == 0){
        int player = board->getIsPlayerBlack() ? 1 : 2;
        board->setPoint(row, col, player);
        board->update();
    }
}

void Online::rivalReady(){
    QByteArray buf;
    isRivalReady = true;
    buf = socket->socket->readAll();
    if(isPlayerReady){
        this->ui->label_color->setText("black");
        this->ui->label_turn->setText("you");
        this->ui->pushButton_surrender->setEnabled(true);
        board->firstPoint();
    }
    disconnect(socket->socket, &QTcpSocket::readyRead, this,
            &Online::rivalReady);
    connect(socket->socket, &QTcpSocket::readyRead, this,
            &Online::receiveRivalMsg);
}
void Online::on_pushButton_start_clicked()
{
    board->reset();
    if(isRivalReady){
        this->ui->label_color->setText("white");
        this->ui->label_turn->setText("rival");
        this->ui->pushButton_surrender->setEnabled(true);
    }
    this->ui->pushButton_start->setEnabled(false);
    isPlayerReady = true;
    QString str = "R";
    QHostAddress addr = socket->socket->peerAddress();
    this->ui->label_rival->setText(addr.toString());
    //QString pName = socket->socket->peerName();
    //this->ui->label_rival->setText(pName);

    socket->sendPoint(str);
}

void Online::alterRival(Board *board, bool non)
{
    int row = board->point_row;
    int col = board->point_col;
    QByteArray buffer;
    QDataStream out(&buffer, QIODevice::WriteOnly);
    out << row << col; // 将两个整数写入字节数组
    socket->sendPoint(QString(buffer));
    this->ui->label_turn->setText("rival");
}

void Online::receiveRivalMsg()
{
    QByteArray buf;
    buf = socket->socket->readAll();
    QDataStream in(buf);
    int row, col;
    in >> row >> col; // 从字节数组中读取两个整数
    if(row == 20 && col == 20){
        emit rivalHasSurrender();
    }else{
        emit readyAlterPlayer(row, col);
        this->ui->label_turn->setText("you");
    }
}

void Online::setSets(int s[]){
    for(int i = 0; i < 3; i++){
        sets[i] = s[i];
        board->sets[i] = s[i];
    }
}

void Online::checkConnected()
{
    if(socket->socket->state() == QAbstractSocket::UnconnectedState){
        handleDisConnected();
    }
}

void Online::on_pushButton_surrender_clicked()
{
    int row = 20;
    int col = 20;
    QByteArray buffer;
    QDataStream out(&buffer, QIODevice::WriteOnly);
    out << row << col;
    board->playerSurrender();
    socket->sendPoint(QString(buffer));
}

void Online::on_pushButton_exit_clicked()
{
    emit gameTerminate();
    socket->socket->disconnect();
    timer->stop();
    timer->deleteLater();
    this->close();
}

void Online::handleBoardTerminated(bool isPlayerWin)
{
    ui->pushButton_start->setEnabled(true);
    socket->socket->readAll();
    isPlayerReady = false;
    isRivalReady = false;
    ui->pushButton_surrender->setEnabled(false);
    if(isPlayerWin){
        int num = ui->lcdNumber_player->value();
        ui->lcdNumber_player->display(num+1);
    }else{
        int num = ui->lcdNumber_rival->value();
        ui->lcdNumber_rival->display(num+1);
    }
    disconnect(socket->socket, &QTcpSocket::readyRead, this,
            &Online::receiveRivalMsg);
    connect(socket->socket, &QTcpSocket::readyRead, this,
            &Online::rivalReady);
}

void Online::handleDisConnected()
{
    QMessageBox::information(this, "Game over", "disconnected");
    timer->stop();
    timer->deleteLater();
    emit gameTerminate();
    this->close();
}

