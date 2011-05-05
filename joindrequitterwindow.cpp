#include "joindrequitterwindow.h"
#include "ui_joindrequitterwindow.h"

JoindreQuitterWindow::JoindreQuitterWindow(Jeu *jeu, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JoindreQuitterWindow)
{
    ui->setupUi(this);

    m_Jeu = jeu;

    m_Jeu->socket = new QTcpSocket();

   // connect(m_Jeu->socket, SIGNAL(disconnected()), this, SLOT(slDisconnected()));
}

JoindreQuitterWindow::~JoindreQuitterWindow()
{
    delete ui;
}

void JoindreQuitterWindow::on_btnAfficherParties_clicked()
{
    if (!Connexion())
    {
        QMessageBox::information(this, "Erreur", "Impossible de se connecter au serveur");
        ui->lbParties->clear();
    }
    else
        GamesRequest();
}

void JoindreQuitterWindow::on_lbParties_currentTextChanged(QString currentText)
{
    ui->btnJoindre->setEnabled(true);
    Partie = currentText;
}

void JoindreQuitterWindow::on_btnJoindre_clicked()
{
    GameJoin();

    salonJoueurs = new SalonJoueurs(m_Jeu, this);
    salonJoueurs->show();
}

void JoindreQuitterWindow::on_btnNouvellePartie_clicked()
{
    if (!Connexion())
    {
        QMessageBox::information(this, "Erreur", "Impossible de se connecter au serveur");
        ui->lbParties->clear();
    }
    else
    {
        Partie = QInputDialog::getText(this, "Nouvelle partie", "Nom", QLineEdit::Normal, "Partie sans nom");

        GameCreate();

        m_Jeu->socket->waitForBytesWritten();

        GamesRequest();
    }
}

bool JoindreQuitterWindow::Connexion()
{
    IPServeur = ui->txtAdresseServeur->text();
    PortServeur = ui->txtPort->text().toInt();

    if (m_Jeu->socket->state() == QTcpSocket::ConnectedState)
        m_Jeu->socket->disconnectFromHost();

    m_Jeu->socket->connectToHost(IPServeur, PortServeur);
    m_Jeu->socket->waitForConnected(100);

    return (m_Jeu->socket->state() == QTcpSocket::ConnectedState);
}

void JoindreQuitterWindow::GameCreate()
{
    QByteArray envoi;
    envoi.append(Jeu::GameCreate);
    envoi.append(Partie);
    envoi.append('\n');
    envoi.append((char)0);
    m_Jeu->socket->write(envoi);
}

void JoindreQuitterWindow::GameJoin()
{
    m_Jeu->NomJoueur = ui->txtNomJoueur->text();
    QByteArray envoi;
    envoi.append(Jeu::GameJoin);
    envoi.append(m_Jeu->NomJoueur);
    envoi.append('\n');
    envoi.append(Partie);

    m_Jeu->socket->write(envoi);
}

void JoindreQuitterWindow::GamesRequest()
{
    QByteArray envoi;
    envoi.append(Jeu::GamesRequest);
    m_Jeu->socket->write(envoi);
    m_Jeu->socket->waitForBytesWritten();
    m_Jeu->socket->waitForReadyRead(100);

    QByteArray resultat = m_Jeu->socket->readAll();
    if (resultat[0] == Jeu::GamesReply)
    {
        QString buffer = resultat.remove(0, 1);
        QStringList Liste = buffer.split('\n');
        Liste.removeLast();
        ui->lbParties->clear();
        ui->lbParties->addItems(Liste);
    }
}

void JoindreQuitterWindow::slDisconnected()
{
    QMessageBox::critical(this, "Erreur CRITIQUE !!!", "Vous avez été déconnecté du serveur !!!");
    salonJoueurs->close();
    this->close();
}
