#ifndef SALONJOUEURS_H
#define SALONJOUEURS_H

#include <QMainWindow>
#include "QtGui"

namespace Ui {
    class SalonJoueurs;
}

class SalonJoueurs : public QMainWindow
{
    Q_OBJECT

public:
    explicit SalonJoueurs(QWidget *parent = 0);
    ~SalonJoueurs();

signals:
    void SetReady();
    void Quit();

private slots:
    void on_btnPret_clicked();
    void on_btnQuitter_clicked();
    void GamePlayers(QByteArray resultat);

private:
    Ui::SalonJoueurs *ui;
};

#endif // SALONJOUEURS_H
