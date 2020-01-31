TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        avlindex.cpp \
        docref.cpp \
        documentparser.cpp \
        hashindex.cpp \
        hashtable.cpp \
        indexhandler.cpp \
        main.cpp \
        porter2_stemmer-master/porter2_stemmer.cpp \
        query.cpp \
        queryengine.cpp \
        userinterface.cpp \
        word.cpp

HEADERS += \
    avlindex.h \
    avltree.h \
    docref.h \
    documentparser.h \
    hashindex.h \
    hashtable.h \
    indexhandler.h \
    json.hpp \
    porter2_stemmer-master/porter2_stemmer.h \
    query.h \
    queryengine.h \
    userinterface.h \
    word.h
