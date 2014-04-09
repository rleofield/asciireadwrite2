TEMPLATE = app
CONFIG += console
CONFIG -= qt


#DESTDIR += bin
CONFIG  += x86_64

OBJECTS_DIR = .tmp
MOC_DIR = .tmp
UI_DIR = .tmp
RCC_DIR = .tmp

QMAKE_CXXFLAGS += -gdwarf-3

QMAKE_CXXFLAGS_DEBUG += -O0
QMAKE_CXXFLAGS_DEBUG += -std=c++0x
QMAKE_CXXFLAGS_DEBUG += -Wparentheses
QMAKE_CXXFLAGS_DEBUG += -Wreturn-type
QMAKE_CXXFLAGS_DEBUG += -Wshadow
QMAKE_CXXFLAGS_DEBUG += -Wextra
QMAKE_CXXFLAGS_DEBUG += -Wunused-parameter
QMAKE_CXXFLAGS_DEBUG -= -Wwrite-strings
QMAKE_CXXFLAGS_DEBUG -= +Wno-unused-variable
QMAKE_CXXFLAGS_DEBUG += -Weffc++

QMAKE_CXXFLAGS_RELEASE += -O2
QMAKE_CXXFLAGS_RELEASE += -std=c++0x
QMAKE_CXXFLAGS_RELEASE += -Wparentheses
QMAKE_CXXFLAGS_RELEASE += -Wreturn-type
QMAKE_CXXFLAGS_RELEASE += -Wshadow
QMAKE_CXXFLAGS_RELEASE += -Wextra
QMAKE_CXXFLAGS_RELEASE += -Wunused-parameter
QMAKE_CXXFLAGS_RELEASE -= -Wwrite-strings
QMAKE_CXXFLAGS_RELEASE += -Wno-unused-variable
QMAKE_CXXFLAGS_RELEASE += -Weffc++

message("qmake asciireadwrite2")


SOURCES += \
    src/main.cpp \
    src/test.cpp \
    src/hstring/gettokens.cpp \
    src/hstring/stringhelper.cpp

HEADERS += \
    src/binrw/wBin.h \
    src/binrw/rBin.h \
    src/txtrw/wList.h \
    src/txtrw/rList.h \
    src/test.h \
    src/hstring/gettokens.h \
    src/hstring/stringhelper.h

INCLUDEPATH += $$PWD/src
INCLUDEPATH += $$PWD/src/binrw
INCLUDEPATH += $$PWD/src/txtrw
INCLUDEPATH += $$PWD/src/hstring

LIBS += -lboost_system -lboost_filesystem

OTHER_FILES += \
    test_bin_write.txt \
    test_ascii_write.txt \
    readascii.txt \
    lgpl-3.0.txt \
    README.md

