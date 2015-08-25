TEMPLATE = app
TARGET = appindicator
CONFIG += link_pkgconfig
PKGCONFIG += appindicator-0.1

QT += widgets

INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++11 -g -pthread -Wno-unused-parameter
QMAKE_RPATHDIR += ../../appindicator
QMAKE_RPATHDIR += ../../dqtx
QMAKE_INCDIR += ../../appindicator/include
QMAKE_INCDIR += ../../dqtx/include
QMAKE_LIBDIR += ../../appindicator
QMAKE_LIBDIR += ../../dqtx
QMAKE_LIBS += -ldqtxAppIndicator
QMAKE_LIBS += -ldqtx

# Input
HEADERS += appindicator.hpp
SOURCES += appindicator.cpp main.cpp
