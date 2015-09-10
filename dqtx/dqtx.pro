include(../dqtx_common.pri)

TEMPLATE = lib
TARGET = dqtx

QT += widgets
 
DEPENDPATH += . include src include/dqtx
INCLUDEPATH += . ./include

DISTFILES += dqtx.pc

# Input
HEADERS += include/dqtx/QSparkLineWidget.hpp \
           include/dqtx/QSparkLineAndBarsWidget.hpp \
           include/dqtx/QDensityWidget.hpp \
           include/dqtx/QTextIconFactory.hpp \
           include/dqtx/QIconTheme.hpp \
           include/dqtx/QSparkLineIconFactory.hpp \
           include/dqtx/QUnixSignalHandler.hpp

SOURCES += src/QSparkLineWidget.cpp \
           src/QSparkLineAndBarsWidget.cpp \
           src/QDensityWidget.cpp \
           src/QTextIconFactory.cpp \
           src/QIconTheme.cpp \
           src/QSparkLineIconFactory.cpp \
           src/QUnixSignalHandler.cpp
           
target.path=$$PREFIX/lib
INSTALLS += target

includes.path=$$PREFIX/include/dqtx
includes.files=include/dqtx/*
INSTALLS += includes

pkgconfig.path=$$PREFIX/lib/pkgconfig
pkgconfig.files=dqtx.pc
INSTALLS += pkgconfig
