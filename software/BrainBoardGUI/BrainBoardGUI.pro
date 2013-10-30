include (/home/jay/Documents/QwtGUI/qwt-6.1.0/qwt.prf)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG += qwt

#QMAKE_CFLAGS_RELEASE += -mfloat-abi=hard -march=armv7-a -mcpu=cortex-a8 -mfpu=neon -lm
#QMAKE_CXXFLAGS_RELEASE += -mfloat-abi=hard -march=armv7-a -mcpu=cortex-a8 -mfpu=neon -lm

SOURCES += main.cpp      \
    brainwindow.cpp      \
    brainplot.cpp        \
    bbuart.c             \
    braincollector.cpp   \
    brainuart.cpp \
    smrx.cpp \
    smtx.cpp

HEADERS +=               \
    brainwindow.h        \
    brainplot.h          \
    bbuart.h             \
    brainsource.h        \
    braincollector.h     \
    brainuart.h \
    smrx.h\
    smtx.h

