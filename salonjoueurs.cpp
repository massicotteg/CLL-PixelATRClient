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
