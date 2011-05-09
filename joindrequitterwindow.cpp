#include "joindrequitterwindow.h"
#include "ui_joindrequitterwindow.h"

#define MAP_DEFAULT 100
#define MAP_MIN 50
#define MAP_MAX 200

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
    if (ui->txtNomJoueur->text() != "")
    {
        GameJoin();

        m_Jeu->salonJoueurs = new SalonJoueurs(m_Jeu, this);
        m_Jeu->salonJoueurs->show();
    }
    else
        QMessageBox::critical(this, "Pas de nom de joueur", "Veuillez specifier un nom de joueur !");
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
    {
        m_Jeu->socket->disconnectFromHost();
        m_Jeu->socket->close();
    }

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
    envoi.append((char)QInputDialog::getInt(this, "Nouvelle partie", "Taille de la map", MAP_DEFAULT, MAP_MIN, MAP_MAX));
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
    m_Jeu->socket->waitForBytesWritten();
}

void JoindreQuitterWindow::GamesRequest()
{
    QByteArray envoi;
    envoi.append(Jeu::GamesRequest);
    m_Jeu->socket->write(envoi);
    m_Jeu->socket->waitForBytesWritten();
}

void JoindreQuitterWindow::slDisconnected()
{
    QMessageBox::critical(this, "Erreur CRITIQUE !!!", "Vous avez été déconnecté du serveur !!!");
    salonJoueurs->close();
    this->close();
}

void JoindreQuitterWindow::GamesReply(QByteArray resultat)
{
    QString buffer = resultat.remove(0, 1);
    QStringList Liste = buffer.split('\n');
    Liste.removeLast();
    ui->lbParties->clear();
    if (Liste.count() == 0)
    {
        QMessageBox::information(this, "Pas de parties", "Il n'y a aucune partie cree sur le serveur");
        ui->btnJoindre->setEnabled(false);
    }
    else
        ui->lbParties->addItems(Liste);
}
