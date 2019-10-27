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
    sprite(const QString &type_, int x_, int y_, int _fps);
    virtual ~sprite() =default;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

    virtual QRectF boundingRect() const override
    { return QRectF (x - 1, y - 1, lenx + 2, leny + 2); }

public slots:
    void next_frame()
    { cur = (cur >= frames-1) ? 0 : cur + 1; }

private:
    QString type;
    int frames, cur, x, y, lenx, leny, fps;
    QTimer tm_frame;


};

#endif // SPRITE_H
