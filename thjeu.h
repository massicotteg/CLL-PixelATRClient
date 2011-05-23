#ifndef THJEU_H
#define THJEU_H

#include <QThread>
#include "QtNetwork"
#include "salonjoueurs.h"
#include "joindrequitterwindow.h"
#include "joueur.h"

class thJeu : public QObject
{
    Q_OBJECT
public:
    explicit thJeu();
    QTcpSocket *m_socket;
    JoindreQuitterWindow *m_joindreQuitterWindow;
    SalonJoueurs *m_salonJoueurs;
    QString m_NomJoueur;
    enum CodesClients { DemandeParties, CreePartie, JoinPartie, MettrePret, DonneesClient, QuitterPartie };
    enum CodesServeurs { ReponseParties, JoueursPartie, DebutPartie, DonneeServeur, FinPartie };
    bool Connexion(QString IPServeur, int PortServeur);
    int TrouveNoArmee(QPoint);
    QList<Joueur> m_joueurs;
    int m_MonNumero;
    bool m_PartieCommancee;

signals:
    void rReponseParties(QByteArray);
    void rJoueursPartie(QByteArray);
    void rDebutPartie();
    void rDonneeServeur();
    void rFinPartie();
    void ConnexionImpossible();
    void siMetAJourAffichage();

private:
    void sDemandeParties();
    int m_Tick;
    QByteArray ToQByteArray(int);
    int ToInt(QByteArray);
    void TraitementDonneeServeur(QByteArray);
    void FonctionFinPartie();


public slots:
    void m_socket_ReadyRead();
    void m_socket_Disconnected();
    void eDemandeParties(QString, int);
    void eCreePartie(QString, int, QString, char);
    void eJoinPartie(QString, QString);
    void eMettrePret();
    void eQuitterPartie();
    void slSourisPressee(QList<QPoint>);
};

#endif // THJEU_H
