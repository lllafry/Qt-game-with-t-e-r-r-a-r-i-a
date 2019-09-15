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
    loader();
    ~loader() {}
    const QVector<QPixmap> &operator[](const QString &key)
    {
        return data[key];
    }

    enum { anmt_citizen = 8 };

private:
    enum { frms_furnace = 6, frms_smoke = 6,
           frms_torch = 5 };

    QMap<QString, QVector<QPixmap> > data;
};

#endif // LOADER_H
