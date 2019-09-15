#include "worker.h"

worker::worker(loader &data, int x_, int y_):
    NPC(data, -1, x_, y_)
{
    set_max_speed(5);
    target_x = home.x;
    connect(&tm_repair, SIGNAL(timeout()), this, SLOT(repair()));
    is_repairing = false;
}


void worker::new_task(int x)
{
    tasks.append(x);
    update_target();
}

void worker::repair()
{
    tm_repair.stop();
    is_repairing = false;
    emit fixed(tasks.at(0));
    tasks.removeAt(0);
    update_target();
}

void worker::update_target()
{
    target_x = tasks.length() ? tasks.at(0) :
                                     home.x;
    if (tasks.length())
        if (!is_repairing && tasks.at(0) == cur.x && is_resting())
        {
            is_repairing = true;
            tm_repair.start(1250 + qrand() % 2750);
        }
}
