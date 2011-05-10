#ifndef JOINDREQUITTERWINDOW_H
#define JOINDREQUITTERWINDOW_H

#include <QMainWindow>
#include "QtGui"

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
    void eGamesRequest(QString, int);
    void eGameCreate(QString, int, QString, char);
    void eGameJoin(QString, QString);

private slots:
    void on_btnAfficherParties_clicked();
    void on_btnJoindre_clicked();
    void on_btnNouvellePartie_clicked();

    void slDisconnected();
    void BadConnection();
    void rGamesReply(QByteArray resultat);

    void on_lbParties_currentRowChanged(int currentRow);

private:
    Ui::JoindreQuitterWindow *ui;
    QString IPServeur;
    int PortServeur;
    QString Partie;

    void GameJoin();
    void GameCreate();
};

#endif // JOINDREQUITTERWINDOW_H
