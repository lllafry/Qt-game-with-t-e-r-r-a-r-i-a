#include <QApplication>
#include <QDateTime>
#include <QDesktopWidget>

#include "town.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QDateTime::currentMSecsSinceEpoch());
    town mytown;
    if (QApplication::desktop()->width() > town::width
            || QApplication::desktop()->height() > town::height)
        mytown.show();
    else
        mytown.showFullScreen();

    return a.exec();
}
