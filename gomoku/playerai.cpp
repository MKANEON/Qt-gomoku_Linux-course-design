#include "playerai.h"
#include "stdlib.h"
#include <QFuture>
#include <QtConcurrent>
#include "board.h"
PlayerAI::PlayerAI(QObject *parent)
    : QObject{parent}
{}

int PlayerAI::evaluatePoint(int row, int col, int color)
{
    int score = 0;
    const int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {1, -1} };
    for (const auto& direction : directions){
        int count = 0;
        int openEnds = 0;

        for (int i = 1; i <= 4; ++i){
            int newRow = row + direction[0] * i;
            int newCol = col + direction[1] * i;
            if (newRow >= 0 && newRow < Board::board_size && newCol >= 0 && newCol < Board::board_size){
                if (board[newRow][newCol] == color){
                    count++;
                }
                else if (board[newRow][newCol] == 0){
                    openEnds++;
                    break;
                }
                else{
                    break;
                }
            }
            else{
                break;
            }
        }

        for (int i = 1; i <= 4; ++i){
            int newRow = row - direction[0] * i;
            int newCol = col - direction[1] * i;
            if (newRow >= 0 && newRow < Board::board_size && newCol >= 0 && newCol < Board::board_size){
                if (board[newRow][newCol] == color){
                    count++;
                }
                else if (board[newRow][newCol] == 0){
                    openEnds++;
                    break;
                }
                else{
                    break;
                }
            }
            else{
                break;
            }
        }

        if (count == 4 && openEnds > 0){
            score += 10000;
        } else if (count == 3 && openEnds > 0){
            score += 1000;
        } else if (count == 2 && openEnds > 0){
            score += 100;
        } else if (count == 1 && openEnds > 0){
            score += 10;
        }
    }

    return score;
}

int PlayerAI::evaluateBoard()
{
    int score = 0;
    for (int i = 0; i < Board::board_size; ++i) {
        for (int j = 0; j < Board::board_size; ++j) {
            if (board[i][j] != 0) {
                score += evaluateDirection(i, j, 1, 0); // 水平
                score += evaluateDirection(i, j, 0, 1); // 垂直
                score += evaluateDirection(i, j, 1, 1); // 主对角线
                score += evaluateDirection(i, j, 1, -1); // 副对角线
            }
        }
    }

    return score;
}

int PlayerAI::evaluateDirection(int row, int col, int deltaRow, int deltaCol)
{
    int blackCount = 0, whiteCount = 0;

    for (int step = 0; step < 5; ++step) {
        int r = row + step * deltaRow;
        int c = col + step * deltaCol;
        if (r >= 0 && r < Board::board_size && c >= 0 && c < Board::board_size) {
            if (board[r][c] == 1) {
                blackCount++;
            } else if (board[r][c] == 2) {
                whiteCount++;
            }
        }
    }

    if (blackCount > 0) {
        if (blackCount == 5) return 10000;
        else if (blackCount == 4) return 1000;
        else if (blackCount == 3) return 100;
        else if (blackCount == 2) return 10;
    }

    if (whiteCount > 0) {
        if (whiteCount == 5) return -10000;
        else if (whiteCount == 4) return -1000;
        else if (whiteCount == 3) return -100;
        else if (whiteCount == 2) return -10;
    }
    return 0;
}

std::pair<int, int> PlayerAI::getBestMove(int color)
{
    QFuture<std::pair<int, int>> future = QtConcurrent::run([=]() { return minimax(color, 1); });
    return future.result();

}

std::pair<int, int> PlayerAI::minimax(int color, int depth)
{
    if (depth == 0) {
        return {evaluateBoard(), -1};
    }

    std::pair<int, int> bestMove = {-1, -1};
    int bestScore = (color == 1) ? -10000 : 10000;
    //int bestScore = 0;
    for (int i = 0; i < Board::board_size; ++i) {
        for (int j = 0; j < Board::board_size; ++j) {
            if (board[i][j] == 0) {
                board[i][j] = color;
                int score = minimax(color == 1 ? 2 : 1, depth - 1).first;
                board[i][j] = 0;
                if ((color == 1 && score > bestScore) || (color == 2 && score < bestScore)) {
                    bestScore = score;
                    bestMove = {i, j};
                }
            }
        }
    }

    return bestMove;
}

std::pair<int, int> PlayerAI::getBestPoint(int color)
{
    int bestRow = 7, bestCol = 7;
    int bestScore = 0;

    for (int row = 0; row < Board::board_size; ++row){
        for (int col = 0; col < Board::board_size; ++col){
            if (board[row][col] == 0){
                int score = evaluatePoint(row, col, color) + evaluatePoint(row, col, 3-color);
                if (score > bestScore){
                    bestScore = score;
                    bestRow = row;
                    bestCol = col;
                }
            }
        }
    }

    return {bestRow, bestCol};
}

void PlayerAI::doPoint(Board *board, bool isPlayerBlack){
    //int board[board->getBoardSize()][board->getBoardSize()];
    for(int i = 0; i < board->getBoardSize(); i++){
        for(int j = 0; j< board->getBoardSize(); j++){
            this->board[i][j] = board->getPointStatus(i, j);
        }
    }
    //std::pair<int, int> bestPoint = getBestMove(isPlayerBlack ? 2 : 1);
    //point_row = rand() % 15;
    //point_col = rand() % 15;
    std::pair<int, int> bestPoint = getBestPoint(isPlayerBlack ? 2 : 1);
    emit hasPoint(bestPoint.first, bestPoint.second);
}
