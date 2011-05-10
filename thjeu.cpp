#include "thjeu.h"

thJeu::thJeu(QObject *parent) :
    QThread(parent)
{
    socket = new QTcpSocket();

    joindreQuitterWindow = new JoindreQuitterWindow();
    salonJoueurs = new SalonJoueurs();

    connect(socket, SIGNAL(readyRead()), this, SLOT(socket_ReadyRead()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(socket_Disconnected()));
    connect(socket, SIGNAL(disconnected()), joindreQuitterWindow, SLOT(slDisconnected()));

    connect(this, SIGNAL(BadConnection()), joindreQuitterWindow, SLOT(BadConnection()));
    connect(joindreQuitterWindow, SIGNAL(eGamesRequest(QString,int)), this, SLOT(eGamesRequest(QString,int)));
    connect(joindreQuitterWindow, SIGNAL(eGameCreate(QString,int,QString,char)), this, SLOT(eGameCreate(QString,int,QString,char)));
    connect(joindreQuitterWindow, SIGNAL(eGameJoin(QString,QString)), this, SLOT(eGameJoin(QString,QString)));

    connect(this, SIGNAL(rGamesReply(QByteArray)), joindreQuitterWindow, SLOT(rGamesReply(QByteArray)));

    connect(salonJoueurs, SIGNAL(SetReady()), this, SLOT(eGameSetReady()));
    connect(salonJoueurs, SIGNAL(Quit()), this, SLOT(eGameQuit()));

    connect(this, SIGNAL(rGamePlayers(QByteArray)), salonJoueurs, SLOT(GamePlayers(QByteArray)));
}


bool thJeu::Connexion(QString IPServeur, int PortServeur)
{
    if (IPServeur != socket->peerAddress().toString() || PortServeur != socket->peerPort())
    {
        if (socket->state() == QTcpSocket::ConnectedState)
        {
            socket->disconnectFromHost();
            socket->reset();
        }

        socket->connectToHost(IPServeur, PortServeur);
        socket->waitForConnected(100);


        return (socket->state() == QTcpSocket::ConnectedState);
    }
    else
        return true;
}

void thJeu::socket_ReadyRead()
{
    QByteArray resultat = socket->readAll();
    switch (resultat[0])
    {
        case GameSData:
            emit rGameSData();
            break;
        case GamePlayers:
            emit rGamePlayers(resultat);
            break;
        case GamesReply:
            emit rGamesReply(resultat);
            break;
        case GameBegin:
            emit rGameBegin();
            salonJoueurs->close();
            joindreQuitterWindow->close();
            break;
        case GameEnd:
            emit rGameEnd();
            salonJoueurs->close();
            break;
    }
}
void thJeu::socket_Disconnected()
{
    salonJoueurs->close();
}

void thJeu::sGamesRequest()
{
    socket->write(QByteArray(1, GamesRequest));
    socket->waitForBytesWritten();
}
void thJeu::eGamesRequest(QString IPServeur, int PortServeur)
{
    if (!Connexion(IPServeur, PortServeur))
        emit BadConnection();
    else
        sGamesRequest();
}

void thJeu::eGameCreate(QString IPServeur, int PortServeur, QString Partie, char NoMap)
{
    if (!Connexion(IPServeur, PortServeur))
        emit BadConnection();
    else
    {
        QByteArray envoie = QByteArray(1, GameCreate);
        envoie.append(Partie + '\n' + NoMap);
        socket->write(envoie);
        socket->waitForBytesWritten();

        sGamesRequest();
    }
}

void thJeu::eGameJoin(QString nomJoueur, QString Partie)
{
    NomJoueur = nomJoueur;

    QByteArray envoi = QByteArray(1, GameJoin);
    envoi.append(NomJoueur + '\n' + Partie);

    socket->write(envoi);
    socket->waitForBytesWritten();

    salonJoueurs->show();
}

void thJeu::eGameSetReady()
{
    socket->write(QByteArray(1, GameSetReady));
    socket->waitForBytesWritten();
}

void thJeu::eGameQuit()
{
    socket->write(QByteArray(1, GameQuit));
    socket->waitForBytesWritten();
    socket->disconnectFromHost();
    socket->close();
}
