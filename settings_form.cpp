#include "settings_form.h"
#include "ui_settings_form.h"

settings_form::settings_form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settings_form)
{
    ui->setupUi(this);
}

void settings_form::on_button_set_clicked()
{
    this->hide();
    ui->num_citizen->setEnabled(false);
    emit set_val(ui->num_citizen->value(),
                 ui->num_game_speed->value(),
                 ui->num_speed_citizen->value(),
                 ui->num_speed_worker->value(),
                 ui->num_elevator_maxcrew->value(),
                 ui->num_elevator_loadcrew->value());
}

settings_form::~settings_form()
{
    delete ui;
}

void settings_form::on_button_remain_clicked()
{
    this->hide();
}

void settings_form::on_num_elevator_maxcrew_valueChanged(int arg1)
{
    ui->num_elevator_loadcrew->setMaximum(arg1);
    if (ui->num_elevator_loadcrew->value() > arg1)
        ui->num_elevator_loadcrew->setValue(arg1);
}
