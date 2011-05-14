#include "pixelatrclientwindow.h"
#include "ui_pixelatrclientwindow.h"
#include "qpainter.h"

PixelATRClientWindow::PixelATRClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PixelATRClientWindow)
{
    ui->setupUi(this);
    m_Jeu = new thJeu();

    timer = new QTimer(this);

    connect(this, SIGNAL(siMouseClick(QList<QPoint>)), m_Jeu, SLOT(slMouseClick(QList<QPoint>)));
    connect(timer, SIGNAL(timeout()), this, SLOT(slTimeOut()));
}

PixelATRClientWindow::~PixelATRClientWindow()
{
    delete ui;
}

void PixelATRClientWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
  //  painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setBrush(QBrush(Qt::darkGray));
    QRect rect(0, 0, LONGUEUR, HAUTEUR);
    painter.drawRect(rect);
    painter.setBrush(QBrush(Qt::gray, Qt::CrossPattern));
    painter.drawRect(rect);

    painter.setPen(QPen(QBrush(Qt::white), 2));

    for (int i = 0; i < points.count() - 1; i++)
        painter.drawLine(points[i], points[i + 1]);

    for (int i = 0; i < m_Jeu->joueurs.count(); i++)
    {
        painter.setPen(QPen(QBrush(m_Jeu->joueurs[i].Couleur), 4));
        for (int j = 0; j < m_Jeu->joueurs[i].Armees.count(); j++)
            painter.drawPoint(m_Jeu->joueurs[i].Armees[j].PosActuelle);
    }

    /*
    QVector<QRect> rects = QVector<QRect>();
    rects.append(QRect(10, 10, 2, 2));
    rects.append(QRect(50, 50, 2, 2));

    painter.setBrush(QBrush(Qt::yellow));
    painter.setPen(QPen(Qt::yellow));
    painter.drawRects(rects);

    rects.clear();
    painter.setBrush(QBrush(Qt::red));
    painter.setPen(QPen(Qt::red));

    rects.append(QRect(30, 50, 2, 2));
    rects.append(QRect(90, 20, 2, 2));
    painter.drawRects(rects);
    */
}

void PixelATRClientWindow::on_btnJoindreQuitter_clicked()
{
    m_Jeu->joindreQuitterWindow->show();
}

void PixelATRClientWindow::mousePressEvent(QMouseEvent *event)
{
    points.append(event->pos());

    timer->start(10);
}

void PixelATRClientWindow::slTimeOut()
{
    points.append(this->cursor().pos() - this->geometry().topLeft());
    this->update();
}

void PixelATRClientWindow::mouseReleaseEvent(QMouseEvent *event)
{
    points.append(event->pos());

    emit siMouseClick(points);
    points.clear();
    this->update();

    timer->stop();
}
