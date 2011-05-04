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

    if (socket->state() == QTcpSocket::ConnectedState)
        socket->disconnectFromHost();

    socket->connectToHost(IPServeur, PortServeur);
    socket->waitForConnected(100);

    if (socket->state() != QTcpSocket::ConnectedState)
    {
        QMessageBox::information(this, "Erreur", "Impossible de se connecter au serveur");
        ui->lbParties->clear();
    }
    else
    {
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
            Liste.removeLast();
            ui->lbParties->clear();
            ui->lbParties->addItems(Liste);
        }
    }
}

void JoindreQuitterWindow::on_lbParties_currentTextChanged(QString currentText)
{
    ui->btnJoindre->setEnabled(true);
    Partie = currentText;
}

void JoindreQuitterWindow::on_btnJoindre_clicked()
{
    QByteArray envoi;
    envoi.append(Jeu::GameJoin);
    envoi.append("Bob");
    envoi.append('\n');
    envoi.append(Partie);

    socket->write(envoi);

    SalonJoueurs *salonjoueurs = new SalonJoueurs(socket, this);
    salonjoueurs->show();
}

void JoindreQuitterWindow::on_btnNouvellePartie_clicked()
{
    Partie = QInputDialog::getText(this, "Nouvelle partie", "Nom");

    QByteArray envoi;
    envoi.append(Jeu::GameCreate);
    envoi.append(Partie);
    envoi.append((char)0);
    socket->write(envoi);

    ui->lbParties->addItem(Partie);

    envoi.clear();
    envoi.append(Jeu::GameJoin);
    envoi.append("Bob");
    envoi.append("\n");
    envoi.append(Partie);
    socket->write(envoi);

    SalonJoueurs *salonjoueurs = new SalonJoueurs(socket, this);
    salonjoueurs->show();
}
