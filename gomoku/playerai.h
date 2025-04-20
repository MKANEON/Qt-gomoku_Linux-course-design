#ifndef PLAYERAI_H
#define PLAYERAI_H

#include <QObject>
#include "board.h"

class PlayerAI : public QObject
{
    Q_OBJECT
public:
    explicit PlayerAI(QObject *parent = nullptr);
    int evaluatePoint(int row, int col, int color);
    int evaluateBoard();
    int evaluateDirection(int row, int col, int deltaRow, int deltaCol);
    std::pair<int, int> getBestMove(int color);
    std::pair<int, int> minimax(int color, int depth);
    std::pair<int, int> getBestPoint(int color);

public slots:
    void doPoint(Board *board, bool isPlayerBlack);
signals:
    void hasPoint(int row, int col);
private:
    int point_row;
    int point_col;
    int board[Board::board_size][Board::board_size];

};

#endif // PLAYERAI_H
