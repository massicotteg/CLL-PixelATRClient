#include "joindrequitterwindow.h"
#include "ui_joindrequitterwindow.h"

JoindreQuitterWindow::JoindreQuitterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JoindreQuitterWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket();
}

JoindreQuitterWindow::~JoindreQuitterWindow()
{
    delete ui;
}

void JoindreQuitterWindow::on_btnAfficherParties_clicked()
{
    IPServeur = ui->txtAdresseServeur->text();
    PortServeur = ui->txtPort->text().toInt();

    socket->connectToHost(IPServeur, PortServeur);

    QByteArray data;
    data.append(Jeu::GamesRequest);
    socket->write(data);
    socket->waitForBytesWritten();
    socket->waitForReadyRead(100);

    QByteArray resultat = socket->readAll();
    if (resultat[0] == Jeu::GamesReply)
    {
        QString buffer = resultat.remove(0, 1);
        QStringList Liste = buffer.split('\n');
        ui->lbParties->addItems(Liste);
    }
}

void JoindreQuitterWindow::on_lbParties_currentTextChanged(QString currentText)
{
    ui->btnJoindre->setEnabled(true);
    Partie = currentText;
}

void JoindreQuitterWindow::on_btnJoindre_clicked()
{
    QByteArray data;
    data.append(Jeu::GameJoin);
    data.append("Bob");
    data.append('\n');
    data.append(Partie);

    socket->write(data);

    SalonJoueurs *salonjoueurs = new SalonJoueurs(socket, this);
    salonjoueurs->show();
}
