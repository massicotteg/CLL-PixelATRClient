#ifndef JEU_H
#define JEU_H

class Jeu
{
public:
    Jeu();
    enum CodesClients { GamesRequest, GameCreate, GameJoin, GameSetReady, GameCData };
    enum CodesServeurs { GamesReply, GamePlayers, GameBegin, GameSData, GameEnd };
};

#endif // JEU_H
