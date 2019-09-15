#ifndef WORKER_H
#define WORKER_H

#include "npc.h"
#include "loader.h"

class worker : public NPC
{
    Q_OBJECT
public:
    worker(loader &data, int x_, int y_);
    virtual ~worker() {}

private:
    void update_target();

    QVector<int> tasks;
    QTimer tm_repair;
    bool is_repairing;

public slots:
    void new_task(int x);
    void repair();

signals:
    void fixed(int);
};

#endif // WORKER_H
