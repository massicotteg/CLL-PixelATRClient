#ifndef JEU_H
#define JEU_H

class Jeu
{
public:
    Jeu();
    enum CodesClients { RequeteParties, GameCreate, GameJoin, ClientDebutPartie, ClientGameData };
    enum CodesServeurs { DirPatie, RefreshPartiePoints, ServeurDebutPartie, ServeurGameData, FinPartie };
};

#endif // JEU_H
