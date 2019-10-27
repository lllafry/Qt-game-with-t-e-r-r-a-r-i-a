#include "lift.h"

lift::lift(int x_, int y_):
    x(x_), y(y_), home_y(y_)
{

    is_work = true;
    is_up = is_pause = is_reverse_mode = false;
    bef_level = cur_level = 1;
    state = crew = 0;
    max_load_crew = 5;
    max_crew = 9;
    draw_crew_position = x + (max_crew > 9 ? 34 : 42);


    crash_rate = crash_rank + 1 + qrand() % 20; // == 0 - error

    lift_on = loader::getData()["lift_on"].at(0);
    lift_off = loader::getData()["lift_off"].at(0);
    lift_clk = loader::getData()["lift_clk"].at(0);
    lenx = lift_on.width();
    leny = lift_on.height();

    tm_pause.setInterval(750);
    tm_crash.start(1500 + qrand() % 500);
    connect(&tm_pause, SIGNAL(timeout()), this, SLOT(end_pause()));
    connect(&tm_crash, SIGNAL(timeout()), this, SLOT(tick_crash()));
}


void lift::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (is_work)
        painter->drawPixmap(x, y, lift_on);
    else
        painter->drawPixmap(x, y, lift_off);
    for (CALLS &call : calls)
        painter->drawPixmap(x-16,
                            home_y+128-call.path * 16 - 80*call.level,
                            lift_clk);
    painter->drawText(draw_crew_position, y + 26, QString::number(crew));
    if (is_pause)
        painter->drawText(x + 42, y + 38, "p");
    if (state) // если не ожидает новых целей
        painter->drawText(x + 18, y + 26, is_up ? "↑" : "↓");
}

void lift::move()
{
    if (!is_work)
        return;

    // подсчёт текущего этажа (последний целый) - нужно для упрощения дальнейшего кода
    if (abs(home_y - y) % 80 == 0)
        bef_level = cur_level = (home_y - y) / 80 + 1;
    else
        cur_level = -1;

    // is_up определяет направление
    //  state:
    // 0 - простой : ожидание первого вызова / обновление направления
    //       переходит в 0 при отсутствии возможности продолжения пути по
    //       заданному направлению
    // 1 - ожидание с открытыми дверями : возможна посадка; const is_up
    // 3 - движение : возможна остановка для промежуточного вызова
    //       желающего; const is_up
    switch (state) {
    case 0:
        if (!calls.length())
            break;
        // если новая цель появилась прямо в месте положения лифта
        // лифт получает направление цели
        for (int i = 0; i < calls.length(); ++i)
            if (calls.at(i).level == bef_level)
            {
                is_up = calls.at(i).path;
                state = 1;
                start_pause();
                break;
            }
        // иначе если при сохранении направления будет цель, то лифт начнёт
        // "слепое" движение вперёд
        if (state == 0 && can_move_toward())
            state = 2;
        // в следующих вариантах в конечном итоге потребуется смена направления
        if (state == 0)
        {
            // ищет людей, path которых не совпадает с is_up лифта
            int target_level = get_some_target();
            // если цели только по типу:
            // лифт 5up, цель 3up
            if (target_level == -1)
            {
                // то меняем ситуацию на тип
                // лифт 5down, цель 3up
                // который описан ниже
                is_up = !is_up;
                target_level = get_some_target();
            }
            // цель по тому же направлению, что у лифта сейчас
            // но ей в другую сторону
            // пример: лифт 3up, цель 5down
            else if (is_up ^ (cur_level > target_level))
            {
                is_reverse_mode = true;
                // добавление псевдо цели
                tasks.insert(target_level);
                state = 2;
            }
            // оставшийся по типу: лифт 5up, цель 3down
            //  обработка на следующем шаге (5down 3down)
            else
                is_up = !is_up;
        }
        break;
    case 1:
        if (!is_pause)
        {
            invite_list.clear();
            state = crew ? 2 : 0;
        }
        else if (crew < max_crew)
        {
            CALLS temp(cur_level, is_up);
            for (CALLS &call : calls)
                if (call == temp)
                {
                    invite_list = call.ID;
                    if (invite_list.length() > max_crew - crew)
                        invite_list.resize(max_crew - crew);
                }
        }
        break;
    case 2:
    {
        if (cur_level != -1) // если ровно на определенном этаже
        {
            if (tasks.contains(cur_level)) // по задачам
            {
                tasks.remove(cur_level);
                if (is_reverse_mode)
                {
                    is_up = !is_up;
                    is_reverse_mode = false;
                }
                state = 1;
                start_pause();
            }
            else if (crew < max_load_crew) // по возможным вызовам
            {
                CALLS temp(cur_level, is_up);
                for (CALLS &call : calls)
                    if (call == temp)
                    {
                        state = 1;
                        start_pause();
                        break;
                    }
            }
        }
        if (state == 2)
            is_up ? y -= 4 : y += 4;
        break;
    }
    default:
        throw std::logic_error("lift::move in switch default state");
    }
    emit current_pos(x, y, is_up, invite_list);
}

void lift::tick_crash()
{
    if (crash_rate > 1 && (qrand() % 4 == 0))
        crash_rate -= 1;
    if ((qrand() % crash_rate) == 0 && is_work)
    {
        is_work = false;
        emit need_worker(x);
    }
}

void lift::fixed(int x_)
{
    if (x != x_)
        return;
    is_work = true;
    crash_rate = crash_rank + 1 + qrand() % 20;
}

void lift::elevator_call(int x_, int level, int path, int npc_id)
{
    if (abs(x_ - x) > 30)
        return;
    CALLS temp_call(level, path, npc_id);
    bool not_finded = true;
    for (CALLS &call : calls)
        if (call == temp_call)
        {
            not_finded = false;
            call.ID.append(npc_id);
            break;
        }
    if (not_finded)
        calls.append(temp_call);
}

void lift::entry(int x_, int to_level, int ID)
{
    if (abs(x_ - x) > 30)
        return;
    crew += 1;
    if (is_reverse_mode)
    {
        // лифт реверсивно двигался пустым по ложной цели
        // но в это время вошёл человек
        // убрать флаг реверсии и очистить ложную цель
        tasks.clear();
        is_reverse_mode = false; //
    }
    // очистка из списка вызовов ID человека, который зашёл
    // если в списке текущего этажа более нет людей (ID.legnth() == 0),
    //  то удаление текущего вызова (кнопка гаснет)
    auto lambda = [ID](CALLS &call) {
        call.ID.erase(std::remove_if(call.ID.begin(), call.ID.end(),
                                     [ID](int id){return id == ID;}), call.ID.end());
        return !call.ID.length(); };

    calls.erase(std::remove_if(calls.begin(), calls.end(), lambda),
                calls.end());

    tasks.insert(to_level);
}

void lift::leave(int x_)
{
    if (abs(x_ - x) > 30)
        return;
    crew -= 1;
}

bool lift::can_move_toward()
{
    /*возвращает факт наличия вызовов, таких что
    1) добраться до них можно не меняя направление движения
    2) цель вызова: двигаться по тому же направлению
    3) вызовы с bef_level этажа игноруются
    пример: 3up -> 4up
    игнорируются: 3down -> 3down, 3up -> 2up, 1up -> 4down*/
    for (int i = 0; i < calls.length(); ++i)
        if (calls.at(i).path == is_up &&
                ((is_up && calls.at(i).level > bef_level) ||
                 (!is_up && calls.at(i).level < bef_level)))
            return true;
    return false;
}

int lift::get_some_target()
{/*
Функция должна найти вызов, который:
1) не совпадает с текущим направлением лифта
2) если развернуть лифт, то будет самой дальней прямой целью
*/
    bool is_finded = false;
    int temp;
    for (int i = 0; i < calls.length(); ++i)
        if (calls.at(i).path != is_up && calls.at(i).level != bef_level)
        {
            if (is_finded)
            {
                if (is_up != (calls.at(i).level < temp))
                    temp = calls.at(i).level;
            }
            else
            {
                temp = calls.at(i).level;
                is_finded = true;
            }
        }
    return is_finded ? temp : -1;
}
