#include "pixelatrclientwindow.h"
#include "ui_pixelatrclientwindow.h"
#include "qpainter.h"

PixelATRClientWindow::PixelATRClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PixelATRClientWindow)
{
    ui->setupUi(this);
    m_Jeu = new thJeu();
}

PixelATRClientWindow::~PixelATRClientWindow()
{
    delete ui;
}

void PixelATRClientWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::darkGray));
    QRect rect(0, 0, LONGUEUR, HAUTEUR);
    painter.drawRect(rect);
    painter.setBrush(QBrush(Qt::gray, Qt::CrossPattern));
    painter.drawRect(rect);

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
