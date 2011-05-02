#include "joindrequitterwindow.h"
#include "ui_joindrequitterwindow.h"

JoindreQuitterWindow::JoindreQuitterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JoindreQuitterWindow)
{
    ui->setupUi(this);
}

JoindreQuitterWindow::~JoindreQuitterWindow()
{
    delete ui;
}

void JoindreQuitterWindow::on_btnAfficherParties_clicked()
{
    IPServeur = ui->txtAdresseServeur->text();
    PortServeur = ui->txtPort->text().toInt();
}
