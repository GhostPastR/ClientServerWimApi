TARGET = libs
TEMPLATE = lib

QMAKE_CXXFLAGS_DEBUG += -O3
QMAKE_CFLAGS_DEBUG  += -O3

SOURCES += \
    datasend.cpp

HEADERS += \
    datasend.h

include(../project.pri)
out = $$setPath(../)
out = $$configLib()
