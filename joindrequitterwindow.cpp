#include "joindrequitterwindow.h"
#include "ui_joindrequitterwindow.h"

JoindreQuitterWindow::JoindreQuitterWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JoindreQuitterWindow)
{
    ui->setupUi(this);
}

JoindreQuitterWindow::~JoindreQuitterWindow()
{
    delete ui;
}
