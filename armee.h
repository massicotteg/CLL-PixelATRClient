#ifndef ARMEE_H
#define ARMEE_H

#include "QtGui"

class Armee
{
public:
    Armee();
    QPoint PosActuelle;
    QList<QPoint> PtsDeplacement;
    int Pixels;
};

#endif // ARMEE_H
