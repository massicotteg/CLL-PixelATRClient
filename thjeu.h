#ifndef THJEU_H
#define THJEU_H

#include <QThread>
#include "QtNetwork"
#include "salonjoueurs.h"
#include "joindrequitterwindow.h"
#include "joueur.h"

class thJeu : public QThread
{
    Q_OBJECT
public:
    explicit thJeu(QObject *parent = 0);
    QTcpSocket *socket;
    JoindreQuitterWindow *joindreQuitterWindow;
    SalonJoueurs *salonJoueurs;
    QString NomJoueur;
    enum CodesClients { GamesRequest, GameCreate, GameJoin, GameSetReady, GameCData, GameQuit };
    enum CodesServeurs { GamesReply, GamePlayers, GameBegin, GameSData, GameEnd };
    bool Connexion(QString IPServeur, int PortServeur);
    int NoArmee(QPoint);
    QList<Joueur> joueurs;

signals:
    void rGamesReply(QByteArray);
    void rGamePlayers(QByteArray);
    void rGameBegin();
    void rGameSData();
    void rGameEnd();
    void BadConnection();

private:
    void sGamesRequest();
    int Tick;


public slots:
    void socket_ReadyRead();
    void socket_Disconnected();
    void eGamesRequest(QString, int);
    void eGameCreate(QString, int, QString, char);
    void eGameJoin(QString, QString);
    void eGameSetReady();
    void eGameQuit();
    void slMouseClick(QList<QPoint>);
};

#endif // THJEU_H
