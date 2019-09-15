#ifndef LIFT_H
#define LIFT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <algorithm>

#include <loader.h>

class lift: public QObject, public QGraphicsItem
{
    Q_OBJECT

    struct CALLS
    {
        int level, path;
        QVector<int> ID;
        CALLS() {}
        CALLS(int level_, int path_)
        {
            level = level_;
            path = path_;
        }
        CALLS(int level_, int path_, int ID_)
        {
            level = level_;
            path = path_;
            ID.append(ID_);
        }
        bool operator ==(CALLS const &other)
        {
            return this->level == other.level && this->path == other.path;
        }
        void set(int level_, int path_)
        {
            level = level_;
            path = path_;
        }
    };

public:
    lift(loader &data_, int x_, int y_);
    ~lift() {}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    QRectF boundingRect() const
    {
        return QRectF (x, y, lenx, leny);
    }
    void set_max_load_crew(int value)
    {
        max_load_crew = value > 0 ? value : 1;
    }
    void set_max_crew(int value)
    {
        max_crew = value > 0 ? value : 1;
        draw_crew_position = x + (max_crew > 9 ? 34 : 42);
    }


public slots:
    void move();
    void tick_crash();
    void fixed(int x_);
    void elevator_call(int x_, int level, int path, int npc_id);
    void entry(int x_, int to_level, int ID);
    void leave(int x_);

private:
    bool can_move_toward();
    int get_some_target();
    void start_pause()
    {
        tm_pause.start();
        is_pause = true;
    }
    int max_load_crew, max_crew;
    enum { crash_rank = 30 };
    QPixmap lift_on, lift_off, lift_clk;
    QTimer tm_crash, tm_pause;
    QVector<CALLS> calls;
    QVector<int> invite_list;
    QSet<int> tasks;
    int x, y, crash_rate, cur_level, bef_level, crew,
        home_y, lenx, leny, state, draw_crew_position;
    bool is_work, is_up, is_pause, is_reverse_mode;

private slots:
    void end_pause()
    {
        tm_pause.stop();
        is_pause = false;
    }



signals:
    void need_worker(int position_x);
    void current_pos(int position_x, int position_y, bool state, QVector<int> list);

};
#endif // LIFT_H
