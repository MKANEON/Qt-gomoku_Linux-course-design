#include "board.h"
#include <QMessageBox>
#include <QMediaPlayer>
#include <QUrl>
#include <QAudioOutput>
int min(int a, int b){
    return a > b ? b : a;
}

Board::Board(QWidget *parent): QWidget{parent}{
    setMouseTracking(true);
}

double Board::getDist(int x1, int y1, int x2, int y2){
    return sqrt((double)(x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
}

void Board::firstPoint(){
    isPlayerBlack = true;
    isPlayerTurn = true;
}

void Board::reset()
{
    for(int i = 0; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            map_board[i][j] = 0;
        }
    }
    isPlayerTurn = false;
    isPlayerBlack = false;
    isTerminate = false;
    point_col = -1;
    point_row = -1;
    update();
}

void Board::playerSurrender()
{
    isTerminate = true;
    emit hasTerminated(false);
    QMessageBox::information(this, "Title", "You loss");

}

void Board::rivalSurrender()
{
    isTerminate = true;
    emit hasTerminated(true);
    QMessageBox::information(this, "Title", "You win");
}

bool Board::checkTerminate(int lrow, int lcol, int color){
    if(isTerminate) return true;
    int ir, ic;
    int cnt[4] = {1, 1, 1, 1};

    ir = lrow;
    while(ir > 0 && map_board[--ir][lcol] == color)cnt[0]++;
    ir = lrow;
    while(ir < board_size - 1 && map_board[++ir][lcol] == color)cnt[0]++;

    ic = lcol;
    while(ic > 0 && map_board[lrow][--ic] == color)cnt[1]++;
    ic = lcol;
    while(ic < board_size - 1 && map_board[lrow][++ic] == color)cnt[1]++;

    ir = lrow;
    ic = lcol;
    while(ic > 0 && ir > 0 && map_board[--ir][--ic] == color)cnt[2]++;
    ir = lrow;
    ic = lcol;
    while(ir < board_size - 1 && ic < board_size - 1 && map_board[++ir][++ic] == color)cnt[2]++;

    ir = lrow;
    ic = lcol;
    while(ir > 0 && ic < board_size - 1 && map_board[--ir][++ic] == color)cnt[3]++;
    ir = lrow;
    ic = lcol;
    while(ic > 1 && ir < board_size - 1 && map_board[++ir][--ic] == color)cnt[3]++;

    for(int i = 0; i < 4; i++){
        if(cnt[i] >= 5){
            isTerminate = true;
            bool isPlayerWin = color == (isPlayerBlack?1:2);
            emit hasTerminated(isPlayerWin);
            if (isPlayerWin){
                emit changeRival(this, isPlayerBlack);
                QMessageBox::information(this, "Title", "You win");
            }else{
                QMessageBox::information(this, "Title", "You loss");
            }
            return true;
        }
    }
    return false;

}

bool Board::getIsPlayerTurn() const
{
    return isPlayerTurn;
}

void Board::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(QColor(240, 222, 202)));
    //(255, 239, 224)
    //(224, 204, 180)
    painter.drawRect(20, 20, 420, 420);

    for(int i = 0; i < board_size; i++){
        painter.drawLine(fringe_size + i*cell_size, fringe_size, fringe_size + i*cell_size, fringe_size + cell_size*(board_size-1));
        painter.drawLine(fringe_size, fringe_size + i*cell_size, fringe_size + cell_size*(board_size-1), fringe_size + i*cell_size);
    }

    if(is_move && point_col!=-1 && point_row!=-1 && isPlayerTurn){
        if(map_board[point_row][point_col] == 0 && !isTerminate){
            painter.setBrush(Qt::red);
            painter.drawEllipse(fringe_size + point_col*cell_size-cell_size / 4, fringe_size + point_row*cell_size-cell_size / 4, cell_size / 2, cell_size / 2);
        }
    }

    for(int i = 0 ; i < board_size; i++){
        for(int j = 0; j < board_size; j++){
            if(map_board[i][j] == 1){
                painter.setBrush(Qt::black);
                painter.drawEllipse(fringe_size + j*cell_size-cell_size / 4, fringe_size + i*cell_size-cell_size / 4, cell_size / 2, cell_size / 2);
            }else if(map_board[i][j] == 2){
                painter.setBrush(Qt::white);
                painter.drawEllipse(fringe_size + j*cell_size-cell_size / 4, fringe_size + i*cell_size-cell_size / 4, cell_size / 2, cell_size / 2);
            }
        }
    }

}

void Board::mouseMoveEvent(QMouseEvent *event){

    int col = event->pos().x() - fringe_size;
    int row = event->pos().y() - fringe_size;
    int temp_point_col = -1;
    int temp_point_row = -1;
    for(int i=col/cell_size; i <= min(col/cell_size+1, board_size-1); i++){
        for(int j=row/cell_size ; j <= min(row/cell_size+1, board_size-1) ; j++){
            double dis = getDist(col, row, i*cell_size, j*cell_size);
            if(dis <= valid_r){
                temp_point_col = i;
                temp_point_row = j;
                break;
            }
        }
    }
    if(temp_point_col!=point_col || temp_point_row!=point_row){
        point_col = temp_point_col;
        point_row = temp_point_row;
        is_move = true;
        update();
    }else{
        is_move = false;
    }

}

void Board::mousePressEvent(QMouseEvent *event){

    if(point_col!=-1 && point_row!=-1 && ! isTerminate &&
        map_board[point_row][point_col]==0 && isPlayerTurn){
        isPlayerTurn ^= 1;
        emit cellClicked(this, point_row, point_col);
        checkTerminate(point_row, point_col, isPlayerBlack?1:2);
        if(!isTerminate){
            emit changeRival(this, isPlayerBlack);
        }
    }

}

void Board::changePlayer(int row, int col){
    isPlayerTurn ^= 1;
    setPoint(row, col, isPlayerBlack?2:1);
    //map_board[row][col] = isPlayerBlack?2:1;
    checkTerminate(row, col, isPlayerBlack?2:1);
}

int Board::getBoardSize(){
    return board_size;
}

int Board::getCellSize(){
    return cell_size;
}

int Board::getPointStatus(int row, int col){
    return this->map_board[row][col];
}

void Board::setPoint(int row, int col, int player){
    this->map_board[row][col] = player;
    update();
    QMediaPlayer *mediaPlayer = new QMediaPlayer;
    QAudioOutput *audioOutput = new QAudioOutput;
    mediaPlayer->setAudioOutput(audioOutput);
    mediaPlayer->setSource(QUrl::fromLocalFile("/root/mycode/QT/gomoku_v1.1/qizi.wav"));
    audioOutput->setVolume(100);
    mediaPlayer->play();
}

bool Board::getIsPlayerBlack()
{
    return isPlayerBlack;
}

void Board::setIsPlayerTurn(bool newIsPlayerTurn)
{
    isPlayerTurn = newIsPlayerTurn;
}

void Board::setIsPlayerBlack(bool newIsPlayerBlack)
{
    isPlayerBlack = newIsPlayerBlack;
}
