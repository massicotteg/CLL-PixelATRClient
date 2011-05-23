#include "thjeu.h"

thJeu::thJeu()
{
    m_socket = new QTcpSocket();

    m_joindreQuitterWindow = new JoindreQuitterWindow();
    m_salonJoueurs = new SalonJoueurs();

    connect(m_socket, SIGNAL(readyRead()), this, SLOT(m_socket_ReadyRead()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(m_socket_Disconnected()));
    connect(m_socket, SIGNAL(disconnected()), m_joindreQuitterWindow, SLOT(slDisconnected()));

    connect(this, SIGNAL(ConnexionImpossible()), m_joindreQuitterWindow, SLOT(ConnexionImpossible()));
    connect(m_joindreQuitterWindow, SIGNAL(eDemandeParties(QString,int)), this, SLOT(eDemandeParties(QString,int)));
    connect(m_joindreQuitterWindow, SIGNAL(eCreePartie(QString,int,QString,char)), this, SLOT(eCreePartie(QString,int,QString,char)));
    connect(m_joindreQuitterWindow, SIGNAL(eJoinPartie(QString,QString)), this, SLOT(eJoinPartie(QString,QString)));

    connect(this, SIGNAL(rReponseParties(QByteArray)), m_joindreQuitterWindow, SLOT(rReponseParties(QByteArray)));

    connect(m_salonJoueurs, SIGNAL(siPret()), this, SLOT(eMettrePret()));
    connect(m_salonJoueurs, SIGNAL(Quitter()), this, SLOT(eQuitterPartie()));

    connect(this, SIGNAL(rJoueursPartie(QByteArray)), m_salonJoueurs, SLOT(rJoueursPartie(QByteArray)));

    m_joueurs = QList<Joueur>();
    m_MonNumero = 0;

    m_PartieCommancee = false;
}


bool thJeu::Connexion(QString IPServeur, int PortServeur)
{
    if (IPServeur != m_socket->peerName() || PortServeur != m_socket->peerPort())
    {
        m_joindreQuitterWindow->m_Voulue = true;
        if (m_socket->state() == QTcpSocket::ConnectedState)
        {
            m_socket->disconnectFromHost();
            m_socket->reset();
        }
        m_joindreQuitterWindow->m_Voulue = false;

        m_socket->connectToHost(IPServeur, PortServeur);
        return m_socket->waitForConnected(100);
    }
    else
        return true;
}

int thJeu::ToInt(QByteArray Donnees)
{
    int d[4];
    int Res = 0;
    d[0] = (((int)((uchar)Donnees[0])) << 24);
    d[1] = (((int)((uchar)Donnees[1])) << 16);
    d[2] = (((int)((uchar)Donnees[2])) << 8);
    d[3] = ((int)((uchar)Donnees[3]));
    for (int I = 0; I < 4; I++)
        Res += d[I];
    return Res;
}
void thJeu::TraitementDonneeServeur(QByteArray resultat)
{
    if (resultat[1] == '0')
    {
        int numero = 0;
        QString buf = resultat.remove(0, 2);
        QStringList tempTrame = buf.split('\n');
        tempTrame.removeLast();

        // Liste d'obstacles

        QStringList tempJoueurs;
        for (int i = 2; i< tempTrame.length(); i++)
        {
            tempJoueurs = tempTrame[i].split('\t');
            tempJoueurs.removeLast();

            Joueur temp;

            if (tempJoueurs.count() >= 2)
            {
                temp.m_Nom = tempJoueurs[0];
                temp.m_Couleur = (Qt::GlobalColor) tempJoueurs[1].toInt();
            }
            else
            {
                temp.m_Nom = "";
                temp.m_Couleur = Qt::white;
            }

            QStringList tempArmee;
            for (int j = 2; j < tempJoueurs.length(); j++)
            {
                Armee armee;
                tempArmee = tempJoueurs[j].split('\r');
                if (tempArmee.count() >= 3)
                {
                    armee.m_PosActuelle.setX(tempArmee[0].toInt());
                    armee.m_PosActuelle.setY(tempArmee[1].toInt());
                    armee.m_Pixels = tempArmee[2].toInt();
                }
                else
                {
                   armee.m_PosActuelle.setX(0);
                   armee.m_PosActuelle.setY(0);
                   armee.m_Pixels = 0;
                }
                temp.m_Armees.append(armee);
            }

            m_joueurs.append(temp);
            if (temp.m_Nom == m_NomJoueur)
                m_MonNumero = numero;

            numero++;
        }
    }
    else if (resultat[1] == '1')
    {
        QString buf = resultat.remove(0, 2);
        QStringList tempJoueurs = buf.split('\n');
        tempJoueurs.removeLast();

        QStringList tempArmees;
        for (int i = 0; i < tempJoueurs.length(); i++)
        {
            tempArmees = tempJoueurs[i].split('\t');
            tempArmees.removeLast();

            m_joueurs[i].m_Armees.clear();
            for (int j = 0; j < tempArmees.length(); j++)
            {
                QStringList tempPoints = tempArmees[j].split('\r');
                Armee armee = Armee();

                if (tempPoints.count() >= 3)
                {  
                    armee.m_PosActuelle.setX(tempPoints[0].toInt());
                    armee.m_PosActuelle.setY(tempPoints[1].toInt());
                    armee.m_Pixels = tempPoints[2].toInt();

                }
                else
                {
                    armee.m_PosActuelle.setX(0);
                    armee.m_PosActuelle.setY(0);
                    armee.m_Pixels = 0;
                }
                m_joueurs[i].m_Armees.append(armee);
            }
        }
    }
    emit siMetAJourAffichage();
}
void thJeu::m_socket_ReadyRead()
{
    QByteArray resultat = m_socket->read(ToInt(m_socket->read(4)));
    switch (resultat[0])
    {
        case DonneeServeur:
            emit rDonneeServeur();
            TraitementDonneeServeur(resultat);
            break;
        case JoueursPartie:
            emit rJoueursPartie(resultat);
            break;
        case ReponseParties:
            emit rReponseParties(resultat);
            break;
        case DebutPartie:
            emit rDebutPartie();
            m_salonJoueurs->close();
            m_joindreQuitterWindow->close();
            break;
        case FinPartie:
            emit rFinPartie();
            FonctionFinPartie();
            break;
    }
}
void thJeu::m_socket_Disconnected()
{
    m_NomJoueur = "";
    m_salonJoueurs->close();
    m_PartieCommancee = false;
    m_joueurs.clear();

    delete m_salonJoueurs;

    m_salonJoueurs = new SalonJoueurs();
    connect(m_salonJoueurs, SIGNAL(siPret()), this, SLOT(eMettrePret()));
    connect(m_salonJoueurs, SIGNAL(Quitter()), this, SLOT(eQuitterPartie()));

    connect(this, SIGNAL(rJoueursPartie(QByteArray)), m_salonJoueurs, SLOT(rJoueursPartie(QByteArray)));

    emit siMetAJourAffichage();
}

QByteArray thJeu::ToQByteArray(int Nombre)
{
    QByteArray Res = QByteArray(1, ((char)(Nombre >> 24)));
    Res.append(QByteArray(1, ((char)(Nombre >> 16))));
    Res.append(QByteArray(1, ((char)(Nombre >> 8))));
    Res.append(QByteArray(1, (char)(Nombre)));
    return  Res;
}
void thJeu::sDemandeParties()
{
    QByteArray envoie = QByteArray(1, DemandeParties);

    envoie.insert(0, ToQByteArray(envoie.count()));
    m_socket->write(envoie);
    m_salonJoueurs->close();
    m_socket->waitForBytesWritten();
}
void thJeu::eDemandeParties(QString IPServeur, int PortServeur)
{
    if (!Connexion(IPServeur, PortServeur))
        emit ConnexionImpossible();
    else
        sDemandeParties();
}

void thJeu::eCreePartie(QString IPServeur, int PortServeur, QString Partie, char NoMap)
{
    if (!Connexion(IPServeur, PortServeur))
        emit ConnexionImpossible();
    else
    {
        QByteArray envoie = QByteArray(1, CreePartie);
        envoie.append(Partie + '\n' + NoMap);

        envoie.insert(0, ToQByteArray(envoie.count()));
        m_socket->write(envoie);
        m_socket->waitForBytesWritten();

        sDemandeParties();
    }
}

void thJeu::eJoinPartie(QString nomJoueur, QString Partie)
{
    if(m_NomJoueur == "")
    {
        m_NomJoueur = nomJoueur;

        QByteArray envoie = QByteArray(1, JoinPartie);
        envoie.append(m_NomJoueur + '\n' + Partie);

        envoie.insert(0, ToQByteArray(envoie.count()));
        m_socket->write(envoie);
        m_socket->waitForBytesWritten();
    }

    m_salonJoueurs->show();
}

void thJeu::eMettrePret()
{
    QByteArray envoie = QByteArray(1, MettrePret);

    envoie.insert(0, ToQByteArray(envoie.count()));
    m_socket->write(envoie);
    m_socket->waitForBytesWritten();
}

void thJeu::eQuitterPartie()
{
    QByteArray envoie = QByteArray(1, QuitterPartie);

    envoie.insert(0, ToQByteArray(envoie.count()));
    m_socket->write(envoie);
    m_socket->waitForBytesWritten();
    m_joindreQuitterWindow->m_Voulue = true;
    m_socket->disconnectFromHost();
}

void thJeu::slSourisPressee(QList<QPoint> points)
{
    if (points.count() > 0)
    {
        int NoArmee = TrouveNoArmee(points[0]);
        if (NoArmee != -1)
        {
            QByteArray envoie = QByteArray(1, DonneesClient);
            envoie.append('\n');
            envoie.append(QString::number(NoArmee));
            envoie.append('\n');
            for (int i = 1; i < points.count(); i++)
            {
                envoie.append(QString::number(points[i].x()));
                envoie.append('\t');
                envoie.append(QString::number(points[i].y()));
                envoie.append('\t');
            }
            envoie.append('\n');
            envoie.append(m_Tick);

            envoie.insert(0, ToQByteArray(envoie.count()));
            m_socket->write(envoie);
            m_socket->waitForBytesWritten();
        }
    }
}

int thJeu::TrouveNoArmee(QPoint point)
{
    int i = 0;
    QPainterPath regionArmee;

    do
    {
        regionArmee = QPainterPath();
        regionArmee.addEllipse(m_joueurs[m_MonNumero].m_Armees[i].m_PosActuelle, -20 * pow(2, -m_joueurs[m_MonNumero].m_Armees[i].m_Pixels / 250.0) + 25, -20 * pow(2, -m_joueurs[m_MonNumero].m_Armees[i].m_Pixels / 250.0) + 25);
    }
    while (!regionArmee.intersects(QRect(point, point)) && ++i < m_joueurs[m_MonNumero].m_Armees.count());

    return i == m_joueurs[m_MonNumero].m_Armees.count() ? -1 : i;
}

void thJeu::FonctionFinPartie()
{
    QString Gagnant = "";
    int i = 0;

    while (Gagnant == "" && i < m_joueurs.count())
    {
        if (m_joueurs[i].m_Armees.count() > 1)
            Gagnant = m_joueurs[i].m_Nom;
        i++;
    }

    if (Gagnant == m_joueurs[m_MonNumero].m_Nom)
        QMessageBox::about(m_joindreQuitterWindow, "Victoire!", "Vous avez gagne!");
    else
        QMessageBox::about(m_joindreQuitterWindow, "Defaite", "Le joueur " + Gagnant + " a remporte!");

    m_joindreQuitterWindow->m_Voulue = true;
    m_socket->disconnectFromHost();
    m_joueurs.clear();
    emit siMetAJourAffichage();
}
