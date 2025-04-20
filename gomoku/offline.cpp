#include "offline.h"
#include "ui_offline.h"
#include "playerai.h"
#include "board.h"
#include "mainwindow.h"
#include "time.h"
#include <QVBoxLayout>
Offline::Offline(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Offline)
{
    srand(time(0));
    ui->setupUi(this);
    board = new Board(this);
    AI = new PlayerAI();
    board->setFixedSize(460, 600);
    ui->verticalLayout->addWidget(board);
    ui->pushButton_surrender->setEnabled(false);
    this->ui->label_turn->setText("you");
    this->ui->label_rival->setText("AI");
    connect(board, &Board::cellClicked, this, &Offline::handleCellClicked);
    connect(board, &Board::changeRival, AI, &PlayerAI::doPoint);
    connect(AI, &PlayerAI::hasPoint, board, &Board::changePlayer);
    connect(board, &Board::hasTerminated, this, &Offline::handleBoardTerminated);

}

Offline::~Offline()
{
    delete ui;
}

void Offline::handleCellClicked(Board *board, int row, int col){
    if(board->getPointStatus(row, col) == 0){
        int player = board->getIsPlayerBlack() ? 1 : 2;
        board->setPoint(row, col, player);
        board->update();
    }
}

void Offline::on_pushButton_start_clicked()
{
    board->reset();
    if(rand() % 2 == 0){
        this->ui->label_color->setText("black");
        //this->ui->label_turn->setText("you");
        board->firstPoint();
    }else{
        this->ui->label_color->setText("white");
        //this->ui->label_turn->setText("rival");
        AI->doPoint(board, false);
    }
    this->ui->pushButton_start->setEnabled(false);
    ui->pushButton_surrender->setEnabled(true);
}

void Offline::setSets(int s[]){
    for(int i = 0; i < 3; i++){
        sets[i] = s[i];
        board->sets[i] = s[i];
    }
}

void Offline::on_pushButton_exit_clicked()
{
    emit gameTerminate();
    this->close();
}

void Offline::handleBoardTerminated(bool isPlayerWin)
{
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_surrender->setEnabled(false);
    if(isPlayerWin){
        int num = ui->lcdNumber_player->value();
        ui->lcdNumber_player->display(num+1);
    }else{
        int num = ui->lcdNumber_rival->value();
        ui->lcdNumber_rival->display(num+1);
    }
}


void Offline::on_pushButton_surrender_clicked()
{
    board->playerSurrender();
}

