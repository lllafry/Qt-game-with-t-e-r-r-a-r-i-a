#ifndef WORKER_H
#define WORKER_H

#include "npc.h"

class worker : public NPC
{
    Q_OBJECT
public:
    worker(int x_, int y_);
    virtual ~worker() =default;

private:
    virtual void update_target() override;

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
