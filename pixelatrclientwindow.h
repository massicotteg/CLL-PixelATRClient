#ifndef PIXELATRCLIENTWINDOW_H
#define PIXELATRCLIENTWINDOW_H

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
};

#endif // PIXELATRCLIENTWINDOW_H
