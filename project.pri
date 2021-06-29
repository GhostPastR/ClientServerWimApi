PATH = ./

CONFIG += c++17
CONFIG += console

defineReplace(setPath){
    PATH = $$1
    export(PATH)
    DESTDIR = $${PATH}lib/
    export(DESTDIR)
}

defineReplace(configLib){
    win32 {
        TEMPLATE = lib
    }
    win64 {
        TEMPLATE = lib
    }
    unix {
        TEMPLATE = plugin
    }
    export(TEMPLATE)
    CONFIG += lib
    export(CONFIG)
}

defineReplace(addLibs){
    for( lib, ARGS ){
        LIBS += -L$${PATH}lib/ -l$${lib}
    }
    export(LIBS)
    for( lib, ARGS ){
        INCLUDEPATH += $${PATH}/$${lib}/
    }
    export(INCLUDEPATH)
}
