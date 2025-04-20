#ifndef ONLINE_H
#define ONLINE_H

#include <QWidget>
#include <QMainWindow>
#include "board.h"
#include "connect.h"
#include "p2psocket.h"
namespace Ui {
class Online;
}

class Online : public QMainWindow
{
    Q_OBJECT

public:
    explicit Online(P2PSocket *sokt, QWidget *parent = nullptr);
    void setSets(int s[]);
    void checkConnected();
    void handleDisConnected();
    ~Online();
    P2PSocket *socket;

private slots:
    void handleCellClicked(Board *board, int row, int col);
    void on_pushButton_start_clicked();
    void alterRival(Board *board, bool non);
    void receiveRivalMsg();
    void rivalReady();
    void on_pushButton_surrender_clicked();
    void on_pushButton_exit_clicked();
    void handleBoardTerminated(bool isPlayerWin);
signals:
    void readyAlterPlayer(int row, int col);
    void gameTerminate();
    void rivalHasSurrender();

private:
    Ui::Online *ui;
    Board *board;
    int sets[3];
    bool isPlayerReady = false;
    bool isRivalReady = false;
    QTimer *timer;
};

#endif // ONLINE_H
