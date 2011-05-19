#include "pixelatrclientwindow.h"
#include "ui_pixelatrclientwindow.h"
#include "qpainter.h"

PixelATRClientWindow::PixelATRClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PixelATRClientWindow)
{
    ui->setupUi(this);
    m_Jeu = new thJeu();

    timerSouris = new QTimer(this);

    connect(this, SIGNAL(siMouseClick(QList<QPoint>)), m_Jeu, SLOT(slMouseClick(QList<QPoint>)));
    connect(timerSouris, SIGNAL(timeout()), this, SLOT(slTimeOutSouris()));
    connect(m_Jeu, SIGNAL(rGameBegin()), this, SLOT(slGameBegin()));
    connect(m_Jeu, SIGNAL(siUpdateAffichage()), SLOT(slUpdateAffichage()));
}

PixelATRClientWindow::~PixelATRClientWindow()
{
    delete ui;
}

void PixelATRClientWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
  //  painter.setRenderHint(QPainter::Antialiasing, true);

    // Arrière-plan de la fenêtre
    painter.setBrush(QBrush(Qt::darkGray));
    QRect rect(0, 0, LONGUEUR, HAUTEUR);
    painter.drawRect(rect);
    painter.setBrush(QBrush(Qt::gray, Qt::CrossPattern));
    painter.drawRect(rect);

    painter.setPen(QPen(QBrush(Qt::white), 2));

    // Dessin de la ligne de trajectoire
    for (int i = 0; i < points.count() - 1; i++)
        painter.drawLine(points[i], points[i + 1]);

    for (int i = 0; i < m_Jeu->joueurs.count(); i++)
    {
        QBrush brush = QBrush(m_Jeu->joueurs[i].Couleur);
        painter.setPen(m_Jeu->joueurs[i].Couleur);

        // Dessin de la base
        QRect rect(m_Jeu->joueurs[i].Armees[0].PosActuelle.x(), m_Jeu->joueurs[i].Armees[0].PosActuelle.y(), -20 * pow(2, -m_Jeu->joueurs[i].Armees[0].Pixels / 250.0) + 25, -20 * pow(2, -m_Jeu->joueurs[i].Armees[0].Pixels / 250.0) + 25);
        painter.fillRect(rect, brush);

        // Dessin des autres armées
        for (int j = 1; j < m_Jeu->joueurs[i].Armees.count(); j++)
        {
            QPainterPath path = QPainterPath();
            path.addEllipse(m_Jeu->joueurs[i].Armees[j].PosActuelle, -20 * pow(2, -m_Jeu->joueurs[i].Armees[j].Pixels / 250.0) + 25, -20 * pow(2, -m_Jeu->joueurs[i].Armees[j].Pixels / 250.0) + 25);
            painter.fillPath(path, brush);
        }
    }

    for (int i = 0; i < m_Jeu->joueurs.count(); i++)
    {
        painter.setPen(m_Jeu->joueurs[i].Couleur);
        painter.drawText(LONGUEUR + 10, 45 + 20 * i, m_Jeu->joueurs[i].Nom + (m_Jeu->joueurs[i].Nom == m_Jeu->NomJoueur ? " <<" : ""));
    }
}

void PixelATRClientWindow::on_btnJoindreQuitter_clicked()
{
    m_Jeu->joindreQuitterWindow->show();
}

void PixelATRClientWindow::AjoutPoint(QPoint pt)
{
    if (pt.x() < LONGUEUR && pt.y() < HAUTEUR && pt.x() > 0 && pt.y() > 0)
        points.append(pt);
}

void PixelATRClientWindow::mousePressEvent(QMouseEvent *event)
{
    if (m_Jeu->PartieCommancee)
    {
        AjoutPoint(event->pos());

        timerSouris->start(10);
    }
}

void PixelATRClientWindow::slTimeOutSouris()
{
    if (m_Jeu->PartieCommancee)
    {
        AjoutPoint(this->cursor().pos() - this->geometry().topLeft());
        this->update();
    }
}

void PixelATRClientWindow::slGameBegin()
{
    m_Jeu->PartieCommancee = true;
}

void PixelATRClientWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_Jeu->PartieCommancee)
    {
        AjoutPoint(event->pos());

        emit siMouseClick(points);
        points.clear();
        this->update();

        timerSouris->stop();
    }
}

void PixelATRClientWindow::slUpdateAffichage()
{
    this->update();
}
