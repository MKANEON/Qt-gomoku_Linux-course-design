#ifndef OFFLINE_H
#define OFFLINE_H

#include <QWidget>
#include "board.h"
#include "playerai.h"
#include <QMainWindow>
namespace Ui {
class Offline;
}

class Offline : public QMainWindow
{
    Q_OBJECT

public:
    explicit Offline(QWidget *parent = nullptr);
    void setSets(int s[]);
    ~Offline();
private slots:
    void handleCellClicked(Board *board, int row, int col);
    void on_pushButton_start_clicked();
    void on_pushButton_exit_clicked();
    void handleBoardTerminated(bool isPlayerWin);
    void on_pushButton_surrender_clicked();

private:
    Ui::Offline *ui;
    Board *board;
    PlayerAI *AI;
    int sets[3];

signals:
    void gameTerminate();

};

#endif // OFFLINE_H
