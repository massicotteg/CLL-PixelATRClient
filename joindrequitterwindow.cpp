#include "joindrequitterwindow.h"
#include "ui_joindrequitterwindow.h"

#define MAP_DEFAULT 100
#define MAP_MIN 50
#define MAP_MAX 200

JoindreQuitterWindow::JoindreQuitterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JoindreQuitterWindow)
{
    ui->setupUi(this);
    m_Voulue = false;
}

JoindreQuitterWindow::~JoindreQuitterWindow()
{
    delete ui;
}

void JoindreQuitterWindow::ConnexionImpossible()
{
    ui->lbParties->clear();
    QMessageBox::information(this, "Erreur", "Impossible de se connecter au serveur");
}
void JoindreQuitterWindow::on_btnAfficherParties_clicked()
{
    emit eDemandeParties(ui->txtAdresseServeur->text(), ui->txtPort->text().toInt());
}
void JoindreQuitterWindow::rReponseParties(QByteArray resultat)
{
    QString buffer = resultat.remove(0, 1);
    QStringList Liste = buffer.split('\n');
    Liste.removeLast();

    ui->lbParties->clear();

    if (Liste.count() == 0)
        QMessageBox::information(this, "Aucune partie", "Il n'y a aucune partie de creee sur le serveur");
    else
        ui->lbParties->addItems(Liste);
}

void JoindreQuitterWindow::on_btnNouvellePartie_clicked()
{
    emit eCreePartie(ui->txtAdresseServeur->text(), ui->txtPort->text().toInt(),
                QInputDialog::getText(this, "Nouvelle partie", "Nom", QLineEdit::Normal, "Partie sans nom"),
                (char)100);//(char)QInputDialog::getInt(this, "Nouvelle partie", "Taille de la map", MAP_DEFAULT, MAP_MIN, MAP_MAX));
}

void JoindreQuitterWindow::on_btnJoindre_clicked()
{
    if (ui->txtNomJoueur->text() != "")
    {
        emit eJoinPartie(ui->txtNomJoueur->text(), ui->lbParties->currentItem()->text());
        ui->txtNomJoueur->setEnabled(false);
    }
    else
        QMessageBox::critical(this, "Pas de nom de joueur", "Veuillez specifier un nom de joueur !");
}

void JoindreQuitterWindow::slDisconnected()
{
    if (!m_Voulue)
        QMessageBox::critical(this, "Connection interrompue !!!", "Vous avez ete deconnecte du serveur !!!");
    m_Voulue = false;
    ui->lbParties->clear();
    ui->btnJoindre->setEnabled(false);
    ui->txtNomJoueur->setEnabled(true);
}
void JoindreQuitterWindow::on_lbParties_currentRowChanged(int currentRow)
{
    if (currentRow != -1)
        ui->btnJoindre->setEnabled(true);
    else
        ui->btnJoindre->setEnabled(false);
}
