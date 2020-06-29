TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Tests.cpp \
        UnitUniqueRandom.cpp \
        main.cpp \
        moveable.cpp

HEADERS += \
    HashTable_DA.h \
    IHash.h \
    Tests.h \
    UnitUniqueRandom.h \
    moveable.h
