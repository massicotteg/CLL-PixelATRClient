#include "salonjoueurs.h"
#include "ui_salonjoueurs.h"

SalonJoueurs::SalonJoueurs(Jeu *jeu, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SalonJoueurs)
{
    ui->setupUi(this);

    m_Jeu = jeu;
}

SalonJoueurs::~SalonJoueurs()
{
    delete ui;
}

void SalonJoueurs::on_btnPret_clicked()
{
    QByteArray envoi;
    envoi.append(Jeu::GameSetReady);
    m_Jeu->socket->write(envoi);

    if (ui->btnPret->text() == "Pret")
        ui->btnPret->setText("Non Pret");
    else
        ui->btnPret->setText("Pret");
}

void SalonJoueurs::on_btnQuitter_clicked()
{
    QByteArray envoi;
    envoi.append(Jeu::GameQuit);
    m_Jeu->socket->write(envoi);
    m_Jeu->socket->disconnectFromHost();
    m_Jeu->socket->close();
}

void SalonJoueurs::GamePlayers(QByteArray resultat)
{
    QString buffer = resultat.remove(0, 1);
    QStringList Liste = buffer.split('\n');
    Liste.removeLast();
    ui->lbJoueurs->clear();
    ui->lbJoueurs->addItems(Liste);
}
