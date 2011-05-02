#ifndef JOINDREQUITTERWINDOW_H
#define JOINDREQUITTERWINDOW_H

#include <QMainWindow>

namespace Ui {
    class JoindreQuitterWindow;
}

class JoindreQuitterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit JoindreQuitterWindow(QWidget *parent = 0);
    ~JoindreQuitterWindow();

private:
    Ui::JoindreQuitterWindow *ui;
};

#endif // JOINDREQUITTERWINDOW_H
