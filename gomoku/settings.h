#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(
        QWidget *parent = nullptr,
        bool sound = true, bool check = true, bool prompt = true);
    bool sound;
    bool chess;
    bool prompt;
    ~Settings();

private slots:
    void on_checkBox_sound_clicked(bool checked);

    void on_checkBox_chess_clicked(bool checked);

    void on_checkBox_prompt_clicked(bool checked);

    void on_pushButton_back_clicked();
signals:
    void backSettings(Settings *settings);
private:
    Ui::Settings *ui;


};

#endif // SETTINGS_H
