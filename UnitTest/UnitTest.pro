TEMPLATE = app

SOURCES += \
        main.cpp

include(../project.pri)
out = $$setPath(../)
out = $$addLibs(libs ClientServer)

HEADERS += \
    test_client_server.h \
    test_data_send.h
