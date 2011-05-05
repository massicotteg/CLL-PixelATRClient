#ifndef SALONJOUEURS_H
#define SALONJOUEURS_H

#include <QMainWindow>
#include "QtNetwork"
#include "jeu.h"

namespace Ui {
    class SalonJoueurs;
}

class SalonJoueurs : public QMainWindow
{
    Q_OBJECT

public:
    explicit SalonJoueurs(Jeu *jeu, QWidget *parent = 0);
    ~SalonJoueurs();

private slots:
    void on_btnPret_clicked();

    void on_btnQuitter_clicked();

    void slReadyRead();

private:
    Ui::SalonJoueurs *ui;
    Jeu *m_Jeu;
};

#endif // SALONJOUEURS_H
