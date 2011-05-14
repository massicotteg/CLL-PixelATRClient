#ifndef JOUEUR_H
#define JOUEUR_H

#include "QtGui"
#include "armee.h"

class Joueur
{
public:
    Joueur();
    QColor Couleur;
    QList<Armee> Armees;
    QString Nom;
};

#endif // JOUEUR_H
