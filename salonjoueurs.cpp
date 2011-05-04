#include "salonjoueurs.h"
#include "ui_salonjoueurs.h"

SalonJoueurs::SalonJoueurs(QTcpSocket *socket, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SalonJoueurs)
{
    ui->setupUi(this);

    Socket = socket;
}

SalonJoueurs::~SalonJoueurs()
{
    delete ui;
}
