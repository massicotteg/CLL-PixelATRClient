#ifndef SALONJOUEURS_H
#define SALONJOUEURS_H

#include <QMainWindow>

namespace Ui {
    class SalonJoueurs;
}

class SalonJoueurs : public QMainWindow
{
    Q_OBJECT

public:
    explicit SalonJoueurs(QWidget *parent = 0);
    ~SalonJoueurs();

private:
    Ui::SalonJoueurs *ui;
};

#endif // SALONJOUEURS_H
