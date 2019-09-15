#ifndef NPC_H
#define NPC_H

#include <QGraphicsItem>
#include <QPainter>
#include <QTimer>
#include <QDateTime>
#include <QDebug>
#include <QMap>
#include <QString>
#include <QVector>
#include <random>
#include <loader.h>

struct POS
{
    int x, y;
    POS() {}
    inline POS(int x_, int y_)
    {
        x = x_;
        y = y_;
    }
    inline bool operator==(POS &eq)
    {
        return this->x == eq.x && this->y == eq.y;
    }
    inline bool operator!=(POS &eq)
    {
        return this->x != eq.x || this->y != eq.y;
    }
};

class NPC : public QObject, public QGraphicsItem
{
    Q_OBJECT

    struct MOVE
    {
        bool is_left, is_visible;
        int speed, sum_frame, cur_frame, state, speed_boost,
            wait, wait_up_speed;
        MOVE()
        {
            is_visible = false;
            is_left = true;
            speed = sum_frame = state = speed_boost = 0;
            wait = wait_up_speed = 0;
            cur_frame = -1; // равнозначно стоянию; [0...max] - движение
        }
        inline bool can_up_speed()
        {
            if (wait_up_speed > -1)
                --wait_up_speed;
            return wait_up_speed == -1;
        }

        inline bool is_waiting()
        {
            if (wait > 0)
            {
                --wait;
                return true;
            }
            return false;
        }
    } move;

public:
    NPC(loader data, int type, int x_, int y_, bool is_set_home = true);
    virtual ~NPC() {}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QRectF boundingRect() const
    {
        return QRectF (cur.x, cur.y, lenx, leny);
    }
    void set_max_speed(int max_speed_)
    {
        max_speed = max_speed_ > 0 ? max_speed_ : 1;
    }

public slots:
    void next_tick();

protected:
    virtual void update_target()=0;
    bool is_resting()
    {
        return move.state == 0;
    }

    enum {amnt_move_frames = 11, delta_to_next_frame = 4};
    POS cur, home;
    int target_x;

private:
    QVector<QPixmap> data_l;
    QVector<QPixmap> data_r;
    int lenx, leny, max_speed;

};

#endif // NPC_H
