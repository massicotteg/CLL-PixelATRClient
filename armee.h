#ifndef ARMEE_H
#define ARMEE_H

#include "QtGui"

class Armee
{
public:
    Armee();
    QPoint m_PosActuelle;
    QList<QPoint> m_PtsDeplacement;
    int m_Pixels;
};

#endif // ARMEE_H
