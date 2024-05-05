#ifndef MEASURE_H
#define MEASURE_H

#include <QObject>
#include <QMetaEnum>

#include <QDebug>
#include <QString>
#include <QStringList>
#include <QList>



class measure
{
public:
    measure();
    static void debug();
    static void readMeasureListFromFile();
};


#endif // MEASURE_H
