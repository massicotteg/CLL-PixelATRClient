#-------------------------------------------------
#
# Project created by QtCreator 2011-04-27T14:37:58
#
#-------------------------------------------------

QT       += core gui network

TARGET = PixelATRClient
TEMPLATE = app


SOURCES += main.cpp\
        pixelatrclientwindow.cpp \
    joindrequitterwindow.cpp \
    salonjoueurs.cpp \
    jeu.cpp \
    threadcomm.cpp

HEADERS  += pixelatrclientwindow.h \
    joindrequitterwindow.h \
    salonjoueurs.h \
    jeu.h \
    threadcomm.h

FORMS    += pixelatrclientwindow.ui \
    joindrequitterwindow.ui \
    salonjoueurs.ui
