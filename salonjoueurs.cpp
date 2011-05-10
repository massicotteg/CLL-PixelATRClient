#include "salonjoueurs.h"
#include "ui_salonjoueurs.h"

SalonJoueurs::SalonJoueurs(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SalonJoueurs)
{
    ui->setupUi(this);
}

SalonJoueurs::~SalonJoueurs()
{
    delete ui;
}

void SalonJoueurs::on_btnPret_clicked()
{
    emit SetReady();

    if (ui->btnPret->text() == "Pret")
        ui->btnPret->setText("Non Pret");
    else
        ui->btnPret->setText("Pret");
}

void SalonJoueurs::on_btnQuitter_clicked()
{
    emit Quit();
}

void SalonJoueurs::GamePlayers(QByteArray resultat)
{
    QString buffer = resultat.remove(0, 1);
    QStringList Liste = buffer.split('\n');
    Liste.removeLast();
    ui->lbJoueurs->clear();
    ui->lbJoueurs->addItems(Liste);
}
