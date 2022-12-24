TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

HEADERS       = board.h \
                cell.h \
                piece.h \
                config.h \
                playerEnum.h \
                statusEnum.h
SOURCES       = main.cpp \
                board.cpp \
                cell.cpp \
                piece.cpp
