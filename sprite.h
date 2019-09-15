#ifndef SPRITE_H
#define SPRITE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QDebug>
#include <QPixmap>
#include <QTimer>
#include <QVector>

class sprite : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    sprite(const QVector<QPixmap> &data_, int x_, int y_,
           int _fps);
    ~sprite() {}
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    QRectF boundingRect() const
    {
        return QRectF (x - 1, y - 1, lenx + 2, leny + 2);
    }

public slots:
    void next_frame()
    {
        cur = (cur >= frames-1) ? 0 : cur + 1;
    }

private:
    const QVector<QPixmap> &data;
    int frames, cur, x, y, lenx, leny, fps;
    QTimer tm_frame;

};

#endif // SPRITE_H
