#include "citizen.h"

const std::array<int, citizen::num_places * 2> citizen::places_xy = {
    -80,  672, //  1 слева за границей
    1760, 672, //  2 справа за границей
    96,   672, //  3 у механника
    496,  592, //  4 этаж 1
    656,  592, //  5 этаж 1
    1104, 592, //  6 этаж 1
    1200, 592, //  7 этаж 1
    496,  512, //  8 этаж 2
    656,  512, //  9 этаж 2
    1104, 512, // 10 этаж 2
    1200, 512, // 11 этаж 2
    496,  432, // 12 этаж 3
    656,  432, // 13 этаж 3
    1104, 432, // 14 этаж 3
    1200, 432, // 15 этаж 3
    496,  352, // 16 этаж 4
    656,  352, // 17 этаж 4
    1104, 352, // 18 этаж 4
    1200, 352, // 19 этаж 4
    1104, 272, // 20 этаж 5
    1200, 272};// 21 этаж 5

citizen::citizen(loader &data_, int type, int place_):
    NPC(data_, abs(type), 0, 0, false)
{
    set_max_speed(2000);
    target_x = cur.x = places_xy[2 * (place_ % num_places)];
    cur.y = places_xy[2 * (place_ % num_places) + 1];
    home = cur;
    connect(&tm_new_task, SIGNAL(timeout()), this, SLOT(task()));
    tm_new_task.start(time_to_task());

    ID = count_npc()++;
}

void citizen::task()
{
    if (way.length() || (qrand() % 10 < 9)) // 90%
        return;

    int rndm_place = qrand() % num_places;
    POS new_home(places_xy[2 * rndm_place], places_xy[2 * rndm_place + 1]);
    if (new_home == home)
        return;
    // создание путя QVector<int> way
    // распределение path:
    //       1   2
    //0 0            0
    //================
    const int level[2] = {(672 - home.y) / 80 + 1, (672 - new_home.y) / 80 + 1};
    const int adjust = 8;
    int lift[2] = {560+adjust, 1328+adjust};
    int path[2] = {0, 0};
    if (home.y != 672)
        path[0] = home.x > 1000 ? 2 : 1;
    if (new_home.y != 672)
        path[1] = new_home.x > 1000 ? 2 : 1;
    home = new_home;
    // перебор комбинаций
    // для лифта используются числа 1aabbc
    // aa - конечные этаж
    // bb - начальный этаж
    // c - подъем (1) или спуск (0)
    if (path[0] == 0 && path[1] == 0)
        way.append(home.x);
    else if (path[0] == 0)
    {
        way.append(lift[path[1] - 1]);
        way.append(100011 + level[1]*1000);
        way.append(home.x);
    }
    else if (path[1] == 0)
    {
        way.append(lift[path[0] - 1]);
        way.append(100000 + level[0]*10 + level[1]*1000);
        way.append(home.x);
    }
    else if (path[0] == path[1] && level[0] == level[1])
        way.append(home.x);
    else if ((path[0] == 1 && path[1] == 2) || (path[0] == 2 && path[1] == 1))
    {
        way.append(lift[path[0] - 1]);
        way.append(101000 + level[0]*10);
        way.append(lift[path[1] - 1]);
        way.append(100011 + level[1]*1000);
        way.append(home.x);
    }
    else if (path[0] == path[1])
    {
        way.append(lift[path[0] - 1]);
        if (level[0] > level[1])
            way.append(100000 + level[0]*10 + level[1]*1000);
        else
            way.append(100001 + level[0]*10 + level[1]*1000);
        way.append(home.x);
    }
    else
        throw std::logic_error("logic error at citizen::task");
    tm_new_task.stop();
    update_target();
}

void citizen::update_target()
{
    if (!way.length())
        return;
    if (way.at(0) < 99999)
    {
        if (way.at(0) != cur.x)
            target_x = way.at(0);
        else
        {
            way.removeAt(0);
            if (!way.length())
                tm_new_task.start(time_to_task());
        }
    }
    if (!way.length())
        return;
    if (way.at(0) / 100000 == 1)
    {
        emit call(cur.x, (way.at(0) / 10) % 100, way.at(0) % 10, ID);
        way.replace(0, way.at(0) + 100000);
    }
    if (!elev.is_in && way.at(0) / 100000 >= 1)
    {
        if (elev.is_can_enter && elev.is_invited &&
                elev.direction == bool(way.at(0) % 10))
        {
            emit enter_elevator(cur.x, (way.at(0) / 1000) % 100, ID);
            elev.is_in = true;
            elev.is_can_enter = false;
        }

    }
    elev.is_invited = false;
}

void citizen::check_elevator(int x_, int y_, bool state, QVector<int> list)
{
    if (x_+8 != cur.x)
        return;

    if (!elev.is_in)
    {
        elev.is_can_enter = (cur.y == y_+32) && ((state == 0) || (state == 1));
        elev.direction = state;
        if (elev.is_can_enter)
        {
            elev.is_invited = list.contains(ID);
            if (elev.is_invited)
                update_target();
        }
    }
    else
    {
        cur.y = y_+32;
        if (y_+32 == 672 - ((way.at(0) / 1000) % 100 - 1) * 80)
        {
            emit leave_elevator(cur.x);
            way.removeAt(0);
            elev.is_in = elev.is_can_enter = false;
            update_target();
        }
    }
}

