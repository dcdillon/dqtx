TEMPLATE = app
TARGET = cpumonitor

QT += widgets

INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++11 -g -pthread -Wno-unused-parameter
QMAKE_RPATHDIR += ../../dqtx
QMAKE_INCDIR += ../../dqtx/include
QMAKE_LIBDIR += ../../dqtx
QMAKE_LIBS += -ldqtx

# Input
HEADERS += cpumonitor.hpp
SOURCES += cpumonitor.cpp main.cpp
