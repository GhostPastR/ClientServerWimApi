TARGET = ClientServer
TEMPLATE = lib

QMAKE_CXXFLAGS_DEBUG += -O3
QMAKE_CFLAGS_DEBUG  += -O3

SOURCES += \
    clientsharedmemory.cpp \
    controlsm.cpp \
    iprocess.cpp \
    serversharedmemory.cpp \
    storagedata.cpp


HEADERS += \
    clientsharedmemory.h \
    controlsm.h \
    iprocess.h \
    serversharedmemory.h \
    storagedata.h

include(../project.pri)
out = $$setPath(../)
out = $$addLibs(libs)
