#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <math.h>
#include <algorithm>

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = nullptr);
    int sets[3];
    int point_col = -1, point_row = -1;

public:
    int getBoardSize();
    int getCellSize();
    int getPointStatus(int row, int col);
    void setPoint(int row, int col, int player);
    bool getIsPlayerBlack();
    void setIsPlayerTurn(bool newIsPlayerTurn);
    void setIsPlayerBlack(bool newIsPlayerBlack);
    void firstPoint();
    void reset();
    void playerSurrender();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    double getDist(int x1, int y1, int x2, int y2);
    bool checkTerminate(int lrow, int lcol, int color);

public:
    static const int board_size = 15;
    static const int cell_size = 30;
    static const int fringe_size = 20;
    bool getIsPlayerTurn() const;

private:
    double valid_r = 10;
    bool is_move = false;
    int map_board[board_size][board_size] = {0};    //borad status
    bool isPlayerTurn = false;
    bool isPlayerBlack = false;
    bool isTerminate = false;

public slots:
    void changePlayer(int row, int col);
    void rivalSurrender();

signals:
    void cellClicked(Board *board, int row, int col);
    void changeRival(Board *board, bool isPlayerBlack);
    void hasTerminated(bool isPlayerWin);
};

#endif // BOARD_H
