#include "pixelatrclientwindow.h"
#include "ui_pixelatrclientwindow.h"

PixelATRClientWindow::PixelATRClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PixelATRClientWindow)
{
    ui->setupUi(this);
    int i = 0;
}

PixelATRClientWindow::~PixelATRClientWindow()
{
    delete ui;
}
