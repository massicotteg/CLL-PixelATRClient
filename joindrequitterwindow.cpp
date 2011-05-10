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
}

JoindreQuitterWindow::~JoindreQuitterWindow()
{
    delete ui;
}

void JoindreQuitterWindow::BadConnection()
{
    ui->lbParties->clear();
    QMessageBox::information(this, "Erreur", "Impossible de se connecter au serveur");
}
void JoindreQuitterWindow::on_btnAfficherParties_clicked()
{
    emit eGamesRequest(ui->txtAdresseServeur->text(), ui->txtPort->text().toInt());
}
void JoindreQuitterWindow::rGamesReply(QByteArray resultat)
{
    QString buffer = resultat.remove(0, 1);
    QStringList Liste = buffer.split('\n');
    Liste.removeLast();

    ui->lbParties->clear();
    ui->lbParties->addItems(Liste);
}

void JoindreQuitterWindow::on_btnNouvellePartie_clicked()
{
    emit eGameCreate(ui->txtAdresseServeur->text(), ui->txtPort->text().toInt(),
                QInputDialog::getText(this, "Nouvelle partie", "Nom", QLineEdit::Normal, "Partie sans nom"),
                (char)QInputDialog::getInt(this, "Nouvelle partie", "Taille de la map", MAP_DEFAULT, MAP_MIN, MAP_MAX));
}

void JoindreQuitterWindow::on_btnJoindre_clicked()
{
    if (ui->txtNomJoueur->text() != "")
        emit eGameJoin(ui->txtNomJoueur->text(), ui->lbParties->currentItem()->text());
    else
        QMessageBox::critical(this, "Pas de nom de joueur", "Veuillez specifier un nom de joueur !");
}

void JoindreQuitterWindow::slDisconnected()
{
    QMessageBox::critical(this, "Erreur CRITIQUE !!!", "Vous avez été déconnecté du serveur !!!");
}
void JoindreQuitterWindow::on_lbParties_currentRowChanged(int currentRow)
{
    if (currentRow != -1)
        ui->btnJoindre->setEnabled(true);
    else
        ui->btnJoindre->setEnabled(false);
}
