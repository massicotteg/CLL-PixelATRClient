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
    explicit SalonJoueurs(QTcpSocket *socket, QWidget *parent = 0);
    ~SalonJoueurs();

private slots:
    void on_btnPret_clicked();

    void on_btnQuitter_clicked();

private:
    Ui::SalonJoueurs *ui;
    QTcpSocket *Socket;
};

#endif // SALONJOUEURS_H
