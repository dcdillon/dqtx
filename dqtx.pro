TEMPLATE = lib
TARGET = dqtx

QT += widgets
 
DEPENDPATH += . include src include/dqtx
INCLUDEPATH += . ./include

VERSION = 0.0.1

# Input
HEADERS += include/dqtx/QSparkLineWidget.hpp \
           include/dqtx/QSparkLineAndBarsWidget.hpp \
           include/dqtx/QDensityWidget.hpp

SOURCES += src/QSparkLineWidget.cpp \
           src/QSparkLineAndBarsWidget.cpp \
           src/QDensityWidget.cpp

target.path=/usr/local/lib
INSTALLS += target

includes.path=/usr/local/include/dqtx
includes.files=include/dqtx/*
INSTALLS += includes
