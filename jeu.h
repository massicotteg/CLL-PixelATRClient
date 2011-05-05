#ifndef JEU_H
#define JEU_H

#include "QtNetwork"

class Jeu
{
public:
    Jeu();
    QTcpSocket *socket;
    QString NomJoueur;
    enum CodesClients { GamesRequest, GameCreate, GameJoin, GameSetReady, GameCData, GameQuit };
    enum CodesServeurs { GamesReply, GamePlayers, GameBegin, GameSData, GameEnd };
private:
};

#endif // JEU_H
