TEMPLATE = lib
TARGET = dqtx

CONFIG += link_pkgconfig
PKGCONFIG += appindicator-0.1

QT += widgets
 
DEPENDPATH += . include src include/dqtx
INCLUDEPATH += . ./include

VERSION = 0.0.1

# Input
HEADERS += include/dqtx/QSparkLineWidget.hpp \
           include/dqtx/QSparkLineAndBarsWidget.hpp \
           include/dqtx/QDensityWidget.hpp \
           include/dqtx/QTextIconFactory.hpp \
           include/dqtx/QAppIndicator.hpp \
           include/dqtx/QIconTheme.hpp

SOURCES += src/QSparkLineWidget.cpp \
           src/QSparkLineAndBarsWidget.cpp \
           src/QDensityWidget.cpp \
           src/QTextIconFactory.cpp \
           src/QAppIndicator.cpp \
           src/QIconTheme.cpp
           
target.path=/usr/local/lib
INSTALLS += target

includes.path=/usr/local/include/dqtx
includes.files=include/dqtx/*
INSTALLS += includes
