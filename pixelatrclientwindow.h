#ifndef PIXELATRCLIENTWINDOW_H
#define PIXELATRCLIENTWINDOW_H

#define HAUTEUR 630
#define LONGUEUR 940

#include <QMainWindow>

namespace Ui {
    class PixelATRClientWindow;
}

class PixelATRClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PixelATRClientWindow(QWidget *parent = 0);
    ~PixelATRClientWindow();

private:
    Ui::PixelATRClientWindow *ui;
    void paintEvent(QPaintEvent *);
};

#endif // PIXELATRCLIENTWINDOW_H
