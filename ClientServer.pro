TEMPLATE = subdirs

SUBDIRS += \
    ClientServer \
    UnitTest \
    libs \
    Client \
    Server \

ClientServer.depends = libs
Client.depends = ClientServer
Server.depends = ClientServer
UnitTest.depends = ClientServer
