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

    joueurs = QList<Joueur>();
}


bool thJeu::Connexion(QString IPServeur, int PortServeur)
{
    if (IPServeur != socket->peerName() || PortServeur != socket->peerPort())
    {
        joindreQuitterWindow->Voulue = true;
        if (socket->state() == QTcpSocket::ConnectedState)
        {
            socket->disconnectFromHost();
            socket->reset();
        }
        joindreQuitterWindow->Voulue = false;

        socket->connectToHost(IPServeur, PortServeur);
        return socket->waitForConnected(100);
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
            if (resultat[1] == '0')
            {
                QString buf = resultat.remove(0, 2);
                QStringList tempTrame = buf.split('\n');
                tempTrame.removeLast();

                // Liste d'obstacles

                QStringList tempJoueurs;
                for (int i = 2; i< tempTrame.length(); i++)
                {
                    tempJoueurs = tempTrame[i].split('\t');
                    tempJoueurs.removeLast();

                    Joueur temp;
                    temp.Nom = tempJoueurs[0];
                    temp.Couleur = (Qt::GlobalColor) tempJoueurs[1].toInt();

                    QStringList tempArmee;
                    for (int j = 2; j < tempJoueurs.length(); j++)
                    {
                        Armee armee;
                        tempArmee = tempJoueurs[j].split('\r');
                        armee.PosActuelle.setX(tempArmee[0].toInt());
                        armee.PosActuelle.setY(tempArmee[1].toInt());
                        armee.Pixels = tempArmee[2].toInt();
                        temp.Armees.append(armee);
                    }

                    joueurs.append(temp);
                    if (temp.Nom == NomJoueur)
                        joueurs.move(i - 2, 0);
                }
            }
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
            joindreQuitterWindow->Voulue = true;
            socket->disconnectFromHost();
            break;
    }
}
void thJeu::socket_Disconnected()
{
    NomJoueur = "";
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
    if(NomJoueur == "")
    {
        NomJoueur = nomJoueur;

        QByteArray envoi = QByteArray(1, GameJoin);
        envoi.append(NomJoueur + '\n' + Partie);

        socket->write(envoi);
        socket->waitForBytesWritten();
    }

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
    joindreQuitterWindow->Voulue = true;
    socket->disconnectFromHost();
}

void thJeu::slMouseClick(QList<QPoint> points)
{
    QByteArray envoi = QByteArray(1, GameCData);
    envoi.append('\n');
  //  envoi.append(QString::number(NoArmee(points[0])));
    envoi.append('\n');
    for (int i = 1; i < points.count(); i++)
    {
        envoi.append(QString::number(points[i].x()));
        envoi.append('\r');
        envoi.append(QString::number(points[i].y()));
        envoi.append('\t');
    }
    envoi.append('\n');
    envoi.append(Tick);
    socket->write(envoi);
}

int thJeu::NoArmee(QPoint point)
{
    int i = 0;

    // code

    return i == joueurs[0].Armees.count() ? -1 : i;
}
