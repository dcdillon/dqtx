include(../dqtx_common.pri)

TEMPLATE = lib
TARGET = dqtxAppIndicator

CONFIG += link_pkgconfig
PKGCONFIG += appindicator-0.1

QT += widgets
 
DEPENDPATH += . include src
INCLUDEPATH += . ./include

DISTFILES += dqtxAppIndicator.pc

# Input
HEADERS += include/dqtx/QAppIndicator.hpp \
           include/dqtx/QAppIndicatorMenuItem.hpp \
           include/dqtx/QAppIndicatorMenu.hpp

SOURCES += src/QAppIndicator.cpp \
           src/QAppIndicatorMenuItem.cpp \
           src/QAppIndicatorMenu.cpp \
           
target.path=$$PREFIX/lib
INSTALLS += target

includes.path=$$PREFIX/include/dqtx
includes.files=include/dqtx/*
INSTALLS += includes

pkgconfig.path=$$PREFIX/lib/pkgconfig
pkgconfig.files=dqtxAppIndicator.pc
INSTALLS += pkgconfig
