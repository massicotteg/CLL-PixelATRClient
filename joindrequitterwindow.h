#ifndef JOINDREQUITTERWINDOW_H
#define JOINDREQUITTERWINDOW_H

#include <QMainWindow>
#include "QtGui"
#include "QtNetwork"
#include "jeu.h"
#include "threadcomm.h"
#include "salonjoueurs.h"

namespace Ui {
    class JoindreQuitterWindow;
}

class JoindreQuitterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit JoindreQuitterWindow(QWidget *parent = 0);
    ~JoindreQuitterWindow();

signals:

private slots:
    void on_btnAfficherParties_clicked();

    void on_lbParties_currentTextChanged(QString currentText);

    void on_btnJoindre_clicked();

private:
    Ui::JoindreQuitterWindow *ui;
    QString IPServeur;
    int PortServeur;
    QTcpSocket *socket;
    QString Partie;
};

#endif // JOINDREQUITTERWINDOW_H
