#include "npc.h"

NPC::NPC(int type, int x_, int y_, bool is_set_home):
    cur(x_, y_)
{
    QString cur_npc = "npc_" +
            (type >= 0 ? QString::number(type % loader::anmt_citizen) : "w");
    data_l = loader::getData()[cur_npc];
    data_r = loader::getData()[cur_npc + "_m"];
    lenx = loader::getData()[cur_npc].length() ? data_l.at(0).width() : 0;
    leny = loader::getData()[cur_npc].length() ? data_l.at(0).height() : 0;
    if (is_set_home)
        home = cur;
}

void NPC::next_tick()
{
    if (move.is_waiting())
        return;
    // state:
    // 0 - бездействие; при появлении != цели в 1 или 3 [при видимости]
    // 1 - процесс невидимый -> видимый (переход в движение) дальше в 3;
    // 2 - процесс видимый -> невидимый (переход в отдых) дальше в 0;
    // 3 - движение
    switch (move.state) {
    case 0:
        if (cur.x != target_x)
            move.state = 1;
        break;
    case 1:
        move.speed_boost = qrand() % 2;
        move.is_left = target_x < cur.x;
        move.is_visible = true;
        move.wait = 5 + qrand() % 13;
        move.state = 3;
        break;
    case 2:
        move.is_visible = false;
        move.state = 0;
        break;
    case 3:
    {
        // при изменении цели определить, является ли она домом
        //(будет ли прятаться при достижении)
        if (cur.x == target_x) // если npc у своей текущей цели
        {
            if (move.speed != 0) // только что "остановился"
            {
                move.speed = 0;
                if (cur == home)
                {
                    // нужно спрятаться с паузой
                    move.wait = 10 + qrand() % 30;
                    move.state = 2;
                }
                else
                    move.state = 0;
                update_target(); // обновляет цель
            }
        }
        else // если npc не у своей цели
        {
            // если движется не в ту сторону
            if (move.is_left != (target_x < cur.x))
            {
                if (move.speed)
                    move.speed -= 1; // уменьшить скорость
                else
                    move.is_left = !move.is_left;
            }
            else // если движется правильно
            {
                int delta = abs(target_x - cur.x);
                if (delta < move.speed * move.speed * move.speed + 1) // приближение к цели
                {
                    if (delta < move.speed)
                        move.speed = delta;
                    else
                    {
                        if (move.speed > 1)
                            --move.speed;
                    }
                }
                else // если цель ещё далеко
                {
                    if (move.can_up_speed() && move.speed < max_speed &&
                            qrand() % (3 + move.speed) == 0)
                        move.wait_up_speed = 3 * ++move.speed;
                }
            }
        }
        int dlt = move.speed; // модуль перемещения
        // чтобы не все одинаково ходили
        if (move.speed && abs(target_x - cur.x) > move.speed)
            dlt += qrand() % (move.speed_boost + (1 + move.speed) / 2);
        cur.x += move.is_left ? -dlt : dlt;
        // изменение картинки персонажа (процесс шаганий)
        if (dlt)
        {
            move.sum_frame += dlt;
            if (move.sum_frame > delta_to_next_frame)
            {
                move.cur_frame += move.sum_frame / delta_to_next_frame;
                move.sum_frame %= delta_to_next_frame;
                if (move.cur_frame > amnt_move_frames)
                    move.cur_frame %= amnt_move_frames;
            }
        }
        else
            move.cur_frame = -1;
        break;
    }
    default:
        throw std::logic_error("NPC::next_tick");

    }
}

void NPC::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (move.is_visible)
        painter->drawPixmap(cur.x, cur.y-4, move.is_left ?
                                data_l.at(move.cur_frame + 1) :
                                data_r.at(move.cur_frame + 1));
}
