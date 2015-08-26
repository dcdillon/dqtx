TEMPLATE = app
TARGET = cpumonitor
CONFIG += link_pkgconfig
PKGCONFIG += appindicator-0.1

QT += widgets

INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++11 -g -pthread -Wno-unused-parameter
QMAKE_RPATHDIR += ../../dqtx
QMAKE_RPATHDIR += ../../appindicator
QMAKE_INCDIR += ../../dqtx/include
QMAKE_INCDIR += ../../appindicator/include
QMAKE_LIBDIR += ../../dqtx
QMAKE_LIBDIR += ../../appindicator
QMAKE_LIBS += -ldqtx -ldqtxAppIndicator

# Input
HEADERS += cpumonitor.hpp
SOURCES += cpumonitor.cpp main.cpp
