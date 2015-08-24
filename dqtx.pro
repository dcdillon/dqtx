TEMPLATE = lib
TARGET = dqtx

CONFIG += link_pkgconfig
PKGCONFIG += appindicator-0.1

QT += widgets
 
DEPENDPATH += . include src include/dqtx
INCLUDEPATH += . ./include

DISTFILES += debian/changelog \
             debian/compat \
             debian/control \
             debian/copyright \
             debian/rules \
             debian/libdqtx0.dirs \
             debian/libdqtx0.install \
             debian/libdqtx-dev.dirs \
             debian/libdqtx-dev.install \
             debian/source/format

VERSION = 0.0.1

# Input
HEADERS += include/dqtx/QSparkLineWidget.hpp \
           include/dqtx/QSparkLineAndBarsWidget.hpp \
           include/dqtx/QDensityWidget.hpp \
           include/dqtx/QTextIconFactory.hpp \
           include/dqtx/QAppIndicator.hpp \
           include/dqtx/QAppIndicatorMenuItem.hpp \
           include/dqtx/QAppIndicatorMenu.hpp \
           include/dqtx/QIconTheme.hpp

SOURCES += src/QSparkLineWidget.cpp \
           src/QSparkLineAndBarsWidget.cpp \
           src/QDensityWidget.cpp \
           src/QTextIconFactory.cpp \
           src/QAppIndicator.cpp \
           src/QAppIndicatorMenuItem.cpp \
           src/QAppIndicatorMenu.cpp \
           src/QIconTheme.cpp
           
#QMAKE_CXXFLAGS += -g
           
target.path=/usr/lib
INSTALLS += target

includes.path=/usr/include/dqtx
includes.files=include/dqtx/*
INSTALLS += includes
