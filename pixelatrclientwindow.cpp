#include "pixelatrclientwindow.h"
#include "ui_pixelatrclientwindow.h"
#include "qpainter.h"

PixelATRClientWindow::PixelATRClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PixelATRClientWindow)
{
    ui->setupUi(this);
    m_Jeu = new thJeu();

    m_timerSouris = new QTimer(this);

    connect(this, SIGNAL(siSourisPressee(QList<QPoint>)), m_Jeu, SLOT(slSourisPressee(QList<QPoint>)));
    connect(m_timerSouris, SIGNAL(timeout()), this, SLOT(slTimeOutSouris()));
    connect(m_Jeu, SIGNAL(rDebutPartie()), this, SLOT(slDebutPartie()));
    connect(m_Jeu, SIGNAL(siMetAJourAffichage()), SLOT(slMetAJourAffichage()));

    m_points = QList<QPoint>();
}

PixelATRClientWindow::~PixelATRClientWindow()
{
    delete ui;
}

void PixelATRClientWindow::paintEvent(QPaintEvent *)
{
    QPainter objetDessin(this);
  //  objetDessin.setRenderHint(QPainter::Antialiasing, true);

    // Arrière-plan de la fenêtre
    objetDessin.setBrush(QBrush(Qt::darkGray));
    QRect rect(0, 0, LONGUEUR, HAUTEUR);
    objetDessin.drawRect(rect);
    objetDessin.setBrush(QBrush(Qt::gray, Qt::CrossPattern));
    objetDessin.drawRect(rect);

    objetDessin.setPen(QPen(QBrush(Qt::white), 2));

    // Dessin de la ligne de trajectoire
    for (int i = 0; i < m_points.count() - 1; i++)
        objetDessin.drawLine(m_points[i], m_points[i + 1]);

    for (int i = 0; i < m_Jeu->m_joueurs.count(); i++)
    {
        QBrush brush = QBrush(m_Jeu->m_joueurs[i].m_Couleur);
        objetDessin.setPen(m_Jeu->m_joueurs[i].m_Couleur);

        // Dessin de la base
        QRect rect(m_Jeu->m_joueurs[i].m_Armees[0].m_PosActuelle.x(), m_Jeu->m_joueurs[i].m_Armees[0].m_PosActuelle.y(), -20 * pow(2, -m_Jeu->m_joueurs[i].m_Armees[0].m_Pixels / 250.0) + 25, -20 * pow(2, -m_Jeu->m_joueurs[i].m_Armees[0].m_Pixels / 250.0) + 25);
        objetDessin.fillRect(rect, brush);

        // Dessin des autres armées
        for (int j = 1; j < m_Jeu->m_joueurs[i].m_Armees.count(); j++)
        {
            QPainterPath path = QPainterPath();
            path.addEllipse(m_Jeu->m_joueurs[i].m_Armees[j].m_PosActuelle, -20 * pow(2, -m_Jeu->m_joueurs[i].m_Armees[j].m_Pixels / 250.0) + 25, -20 * pow(2, -m_Jeu->m_joueurs[i].m_Armees[j].m_Pixels / 250.0) + 25);
            objetDessin.fillPath(path, brush);
        }
    }

    for (int i = 0; i < m_Jeu->m_joueurs.count(); i++)
    {
        if (m_Jeu->m_joueurs[i].m_Armees.count() > 1)
        {
            objetDessin.setPen(m_Jeu->m_joueurs[i].m_Couleur);
            objetDessin.drawText(LONGUEUR + 10, 45 + 20 * i, m_Jeu->m_joueurs[i].m_Nom + (m_Jeu->m_joueurs[i].m_Nom == m_Jeu->m_NomJoueur ? " <<" : ""));
        }
    }
}

void PixelATRClientWindow::on_btnJoindreQuitter_clicked()
{
    if (m_Jeu->m_PartieCommancee)
    {
        m_Jeu->m_socket->write(QByteArray(1, m_Jeu->QuitterPartie));
        m_Jeu->m_socket->waitForBytesWritten();
        m_Jeu->m_socket->disconnectFromHost();
    }
    else
        m_Jeu->m_joindreQuitterWindow->show();
}

void PixelATRClientWindow::AjoutPoint(QPoint pt)
{
    if (pt.x() < LONGUEUR && pt.y() < HAUTEUR && pt.x() > 0 && pt.y() > 0)
        m_points.append(pt);
}

void PixelATRClientWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePressEvent";
    if (m_Jeu->m_PartieCommancee)
    {
        AjoutPoint(event->pos());

        m_timerSouris->start(10);
    }
}

void PixelATRClientWindow::slTimeOutSouris()
{
    qDebug() << "slTimeOutSouris";
    if (m_Jeu->m_PartieCommancee)
    {
        AjoutPoint(this->cursor().pos() - this->geometry().topLeft());
        this->update();
    }
}

void PixelATRClientWindow::slDebutPartie()
{
    m_Jeu->m_PartieCommancee = true;
}

void PixelATRClientWindow::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mouseReleaseEvent";
    if (m_Jeu->m_PartieCommancee)
    {
        AjoutPoint(event->pos());

        emit siSourisPressee(m_points);
        m_points.clear();
        this->update();

        m_timerSouris->stop();
    }
}

void PixelATRClientWindow::slMetAJourAffichage()
{
    this->update();
}

void PixelATRClientWindow::closeEvent(QCloseEvent *)
{
    m_Jeu->m_socket->write(QByteArray(1, m_Jeu->QuitterPartie));
    m_Jeu->m_socket->waitForBytesWritten();
    m_Jeu->m_joindreQuitterWindow->m_Voulue = true;
    m_Jeu->m_socket->disconnectFromHost();
}
