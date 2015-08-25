include(dqtx_common.pri)

TEMPLATE = subdirs
SUBDIRS = dqtx \
          appindicator

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
