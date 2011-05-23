#ifndef JOUEUR_H
#define JOUEUR_H

#include "QtGui"
#include "armee.h"

class Joueur
{
public:
    Joueur();
    QColor m_Couleur;
    QList<Armee> m_Armees;
    QString m_Nom;
};

#endif // JOUEUR_H
