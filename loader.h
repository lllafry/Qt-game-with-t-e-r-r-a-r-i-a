#ifndef LOADER_H
#define LOADER_H

#include <QPixmap>
#include <QString>
#include <QMap>
#include <QVector>

#include <QDebug>



class loader
{
public:
    enum { anmt_citizen = 8 };


    static const QMap<QString, QVector<QPixmap> > &getData();


private:
    enum { frms_furnace = 6, frms_smoke = 6,
           frms_torch = 5 };
    QMap<QString, QVector<QPixmap> > data;


    loader();
    ~loader() =default;
    loader(const loader &) =delete;
    loader &operator=(const loader &) =delete;
};

#endif // LOADER_H
