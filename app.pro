include(version.pri)

TEMPLATE = app
TARGET = coex-pidgin
DESTDIR = bin/
OBJECTS_DIR = tmp/
QT -= gui

SOURCES += \
	src/app.cpp \
	src/task.cpp \
	src/coex/config.cpp \
	src/coex/typeos.cpp \

HEADERS += \
	src/coex/coex.h \
	src/coex/config.h \
	src/coex/typeos.h \
	src/task.h


