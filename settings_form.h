#ifndef SETTINGS_FORM_H
#define SETTINGS_FORM_H

#include <QWidget>

namespace Ui {
class settings_form;
}

class settings_form : public QWidget
{
    Q_OBJECT

public:
    explicit settings_form(QWidget *parent = 0);
    void change_visible()
    {
        if (this->isVisible())
            this->hide();
        else
            this->show();
    }

    ~settings_form();

private slots:
    void on_button_set_clicked();

    void on_button_remain_clicked();

    void on_num_elevator_maxcrew_valueChanged(int arg1);

private:
    Ui::settings_form *ui;

signals:
    void set_val(int, int, int, int, int, int);
};

#endif // SETTINGS_FORM_H
