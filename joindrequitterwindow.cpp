#include "joindrequitterwindow.h"
#include "ui_joindrequitterwindow.h"

JoindreQuitterWindow::JoindreQuitterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JoindreQuitterWindow)
{
    ui->setupUi(this);

    socket = QTcpSocket();
}

JoindreQuitterWindow::~JoindreQuitterWindow()
{
    delete ui;
}

void JoindreQuitterWindow::on_btnAfficherParties_clicked()
{
    IPServeur = ui->txtAdresseServeur->text();
    PortServeur = ui->txtPort->text().toInt();
    socket.connectToHost(IPServeur, PortServeur);

    socket.write(Jeu::RequeteParties);
}
