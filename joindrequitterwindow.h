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
    bool m_Voulue;
    ~JoindreQuitterWindow();

signals:
    void eDemandeParties(QString, int);
    void eCreePartie(QString, int, QString, char);
    void eJoinPartie(QString, QString);

private slots:
    void on_btnAfficherParties_clicked();
    void on_btnJoindre_clicked();
    void on_btnNouvellePartie_clicked();

    void slDisconnected();
    void ConnexionImpossible();
    void rReponseParties(QByteArray resultat);

    void on_lbParties_currentRowChanged(int currentRow);

private:
    Ui::JoindreQuitterWindow *ui;
    QString m_IPServeur;
    int m_PortServeur;
    QString m_Partie;

    void GameJoin();
    void GameCreate();
};

#endif // JOINDREQUITTERWINDOW_H
