TEMPLATE = lib
TARGET = 
DEPENDPATH += . include src include/dqtx
INCLUDEPATH += . ./include

VERSION = 0.0.1

# Input
HEADERS += include/dqtx/QSparkLineWidget.hpp \
           include/dqtx/QSparkBarWidget.hpp

SOURCES += src/QSparkLineWidget.cpp \
           src/QSparkBarWidget.cpp

target.path=/usr/local/lib
INSTALLS += target

includes.path=/usr/local/include/dqtx
includes.files=include/dqtx/*
INSTALLS += includes
