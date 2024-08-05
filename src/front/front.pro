QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../lib/s21_creditcal.c \
    creditwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    ../lib/s21_datatypes.c \
    ../lib/s21_lexeme_parser.c \
    ../lib/s21_polish.c \
    ../lib/s21_validate.c \
    qcustomplot.cpp

HEADERS += \
    creditwindow.h \
    mainwindow.h \
    ../lib/s21_datatypes.h \
    ../lib/s21_lexeme_parser.h \
    ../lib/s21_polish.h \
    ../lib/s21_validate.h \
    ../s21_smartcal.h \
    qcustomplot.h

FORMS += \
    creditwindow.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
