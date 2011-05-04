#include "salonjoueurs.h"
#include "ui_salonjoueurs.h"

SalonJoueurs::SalonJoueurs(QTcpSocket *socket, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SalonJoueurs)
{
    ui->setupUi(this);

    Socket = socket;

    QByteArray envoi;
    envoi.append(Jeu::GamesRequest);
    Socket->write(envoi);

    Socket->waitForReadyRead(100);

    QByteArray resultat = socket->readAll();
    if (resultat[0] == Jeu::GamePlayers)
    {
        QString buffer = resultat.remove(0, 1);
        QStringList Liste = buffer.split('\n');
        Liste.removeLast();
        ui->lbJoueurs->clear();
        ui->lbJoueurs->addItems(Liste);
    }
}

SalonJoueurs::~SalonJoueurs()
{
    delete ui;
}

void SalonJoueurs::on_btnPret_clicked()
{
    QByteArray envoi;
    envoi.append(Jeu::GameJoin);
    Socket->write(envoi);
}

void SalonJoueurs::on_btnQuitter_clicked()
{
    QByteArray envoi;
    envoi.append(Jeu::GameQuit);
    Socket->write(envoi);
}
