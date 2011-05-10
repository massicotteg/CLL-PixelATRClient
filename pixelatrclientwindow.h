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

private slots:
    void on_btnJoindreQuitter_clicked();

private:
    Ui::PixelATRClientWindow *ui;
    void paintEvent(QPaintEvent *);
    thJeu *m_Jeu;
};

#endif // PIXELATRCLIENTWINDOW_H
