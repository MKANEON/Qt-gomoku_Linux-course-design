#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include "board.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = nullptr);

private:

    Ui::MainWindow *ui;
signals:

};

#endif // GAMEWIDGET_H
