######################################################################
# Automatically generated by qmake (3.0) so srp 20 21:14:01 2016
######################################################################

TEMPLATE = app
TARGET = run
INCLUDEPATH += .
QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare -Wno-unused-but-set-variable -Wno-reorder -Wno-maybe-uninitialized -Wno-empty-body -g
LIBS += -lpugixml -L/home/tomisoka/libs

QT = core gui multimedia

# Input
HEADERS += common.h window.h game.h render.h images.h level.h menu.h player.h enemy.h rat.h wolf.h device.h campfire.h trap.h unlock.h trader.h bear.h
SOURCES += main.cpp common.cpp window.cpp game.cpp render.cpp images.cpp level.cpp menu.cpp player.cpp enemy.cpp rat.cpp wolf.cpp device.cpp campfire.cpp trap.cpp unlock.cpp trader.cpp bear.cpp
