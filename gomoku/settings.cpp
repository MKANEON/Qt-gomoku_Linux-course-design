#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent, bool sound, bool chess, bool prompt)
    : QWidget(parent)
    , ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->ui->checkBox_sound->setChecked(sound);
    this->ui->checkBox_chess->setChecked(chess);
    this->ui->checkBox_prompt->setChecked(prompt);
    this->sound = sound;
    this->chess = chess;
    this->prompt = prompt;

}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_checkBox_sound_clicked(bool checked)
{
    sound = checked;
}

void Settings::on_checkBox_chess_clicked(bool checked)
{
    chess = checked;
}

void Settings::on_checkBox_prompt_clicked(bool checked)
{
    prompt = checked;
}

void Settings::on_pushButton_back_clicked()
{
    emit backSettings(this);
}

