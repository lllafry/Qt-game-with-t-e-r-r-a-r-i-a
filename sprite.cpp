#include "sprite.h"

sprite::sprite(const QVector<QPixmap> &data_, int x_, int y_,
               int _fps):
    data(data_), frames(data.length()), x(x_), y(y_), fps(_fps)
{
    lenx = data.length() ? data.at(0).width() : 0;
    leny = data.length() ? data.at(0).height() : 0;
    cur = qrand() % frames;
    connect(&tm_frame, SIGNAL(timeout()), this, SLOT(next_frame()));
    tm_frame.start(1000 / fps);
}

void sprite::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    painter->drawPixmap(x, y, data.at(cur));
    Q_UNUSED(option);
    Q_UNUSED(widget);
}
