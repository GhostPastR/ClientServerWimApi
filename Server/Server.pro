TEMPLATE = app

SOURCES += \
        main.cpp

HEADERS +=

include(../project.pri)
out = $$setPath(../)
out = $$addLibs(libs ClientServer)
