#ifndef PIXELATRCLIENTWINDOW_H
#define PIXELATRCLIENTWINDOW_H

#define HAUTEUR 630
#define LONGUEUR 940

#include <QMainWindow>
#include <thjeu.h>

namespace Ui {
    class PixelATRClientWindow;
}

class PixelATRClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PixelATRClientWindow(QWidget *parent = 0);
    ~PixelATRClientWindow();

signals:
    void siMouseClick(QList<QPoint>);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void closeEvent(QCloseEvent *);

private slots:
    void on_btnJoindreQuitter_clicked(); 
    void slTimeOutSouris();
    void slGameBegin();
    void slUpdateAffichage();

private:
    Ui::PixelATRClientWindow *ui;
    void paintEvent(QPaintEvent *);
    thJeu *m_Jeu;
    QTimer *timerSouris;
    QList<QPoint> points;
    void AjoutPoint(QPoint pt);
};

#endif // PIXELATRCLIENTWINDOW_H
