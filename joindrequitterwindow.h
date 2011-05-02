#ifndef JOINDREQUITTERWINDOW_H
#define JOINDREQUITTERWINDOW_H

#include <QMainWindow>
#include "QtNetwork"
#include "jeu.h"

namespace Ui {
    class JoindreQuitterWindow;
}

class JoindreQuitterWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit JoindreQuitterWindow(QWidget *parent = 0);
    ~JoindreQuitterWindow();

private slots:
    void on_btnAfficherParties_clicked();

private:
    Ui::JoindreQuitterWindow *ui;
    QString IPServeur;
    int PortServeur;
};

#endif // JOINDREQUITTERWINDOW_H
