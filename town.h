#ifndef TOWN_H
#define TOWN_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QTimer>

#include "citizen.h"
#include "sprite.h"
#include "settings_form.h"
#include "lift.h"
#include "loader.h"
#include "worker.h"

#include <QDebug>
#include <QTime>

class town : public QGraphicsView
{
    Q_OBJECT
public:
    town();
    virtual ~town();

    enum { height = 912, width = 1672 };

private:
    virtual void keyPressEvent(QKeyEvent* event);

    QGraphicsScene scene;
    QTimer tm_tick, tm_update;
    settings_form settings;
    QVector<sprite*> sprites;
    QVector<lift*> elevators;
    QVector<worker*> workers;
    QVector<citizen*> citizens;

private slots:
    void update_settings(int num_citizen,
                         int num_game_speed,
                         int num_speed_citizen,
                         int num_speed_worker,
                         int num_elevator_maxcrew,
                         int num_elevator_loadcrew);
};

#endif // TOWN_H
