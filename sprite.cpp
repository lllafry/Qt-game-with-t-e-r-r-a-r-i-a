#include "loader.h"
#include "sprite.h"

sprite::sprite(const QString &type_, int x_, int y_,
               int _fps):
    type(type_), frames(loader::getData()[type].length()), x(x_), y(y_), fps(_fps)
{
    lenx = frames ? loader::getData()[type].at(0).width() : 0;
    leny = frames ? loader::getData()[type].at(0).height() : 0;
    cur = qrand() % frames;
    connect(&tm_frame, SIGNAL(timeout()), this, SLOT(next_frame()));
    tm_frame.start(1000 / fps);
}

void sprite::paint(QPainter *painter, const QStyleOptionGraphicsItem */*option*/,
                  QWidget */*widget*/)
{
    painter->drawPixmap(x, y, loader::getData()[type].at(cur));
}
