CONFIG(debug, debug|release) {
    DESTDIR = debug
}
CONFIG(release, debug|release) {
    DESTDIR = release
}


TEMPLATE = app
TARGET = RemoteControllerTest_v1
DEPENDPATH += .
INCLUDEPATH += .
CONFIG += c++11
OBJECTS_DIR = $$DESTDIR/objects
MOC_DIR = $$DESTDIR/moc
QT += network widgets

HEADERS =   CODE/MainWindow.h \
			CODE/TextEdit.h

SOURCES =   CODE/main.cpp \
			CODE/MainWindow.cpp \
			CODE/TextEdit.cpp

