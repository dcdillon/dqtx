TEMPLATE = app
TARGET = signalhandler

QT += widgets

INCLUDEPATH += .

QMAKE_CXXFLAGS += -std=c++11 -g -pthread -Wno-unused-parameter
QMAKE_RPATHDIR += ../../dqtx
QMAKE_INCDIR += ../../dqtx/include
QMAKE_LIBDIR += ../../dqtx
QMAKE_LIBS += -ldqtx

# Input
SOURCES += main.cpp \
           SignalProcessor.cpp

HEADERS += SignalProcessor.hpp
