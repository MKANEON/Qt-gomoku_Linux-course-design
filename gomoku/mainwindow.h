#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "board.h"
#include "settings.h"
#include "connect.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool sound = true;
    bool chess = true;
    bool prompt = true;
    int sets[3] = {1, 1, 1};
    ~MainWindow();

private slots:
    void handleSettings(Settings *settings);
    void handleConnect(P2PSocket *socket);
    void showMainwindow();
    void on_pushButton_offline_clicked();
    void on_pushButton_online_clicked();
    void on_pushButton_settings_clicked();
    void on_pushButton_exit_clicked();

private:
    int isPlayerTurn;
    int isPlayerBlack;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
