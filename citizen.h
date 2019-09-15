#ifndef CITIZEN_H
#define CITIZEN_H

#include <QTimer>
#include <array>
#include "npc.h"
#include "loader.h"

class citizen : public NPC
{
    Q_OBJECT

    struct ELEVATOR
    {
        bool is_in, is_can_enter, direction, is_invited;
        inline ELEVATOR()
        {
            is_in = is_can_enter = direction = is_invited = false;
        }
    };

public:
    citizen(loader &data_, int type, int place_);
    virtual ~citizen() {}

public slots:
    void check_elevator(int x_, int y_, bool state, QVector<int> list);

private:
    void update_target();
    static int& count_npc()
    {
        static int counter = 0;
        return counter;
    }
    int time_to_task()
    {
        return 1000 + qrand() % 2000;
    }

    enum {num_places = 21};
    static const std::array<int, num_places * 2> places_xy;
    QVector<int> way;
    QTimer tm_new_task;
    int ID;
    ELEVATOR elev;

private slots:
    void task();

signals:
    void enter_elevator(int x, int to_level, int ID_);
    void leave_elevator(int x);
    void call(int x, int from_level, int path, int ID_);
};

#endif // CITIZEN_H
