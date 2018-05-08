QT -= gui
TEMPLATE = lib
CONFIG += plugin
TARGET = ExampleImporter

SOURCES +=  exampleimporter.cpp
HEADERS += exampleimporter.h

macx {
INCLUDEPATH += /Applications/iolite4beta.app/Contents/include
LIBS += /Applications/iolite4beta.app/Contents/Frameworks/libiolite_core.1.dylib
}

win32 {
INCLUDEPATH += "C:/Program Files/iolite-software/iolite4beta/include"
LIBS += "C:/Program Files/iolite-software/iolite4beta/iolite_core.dll"
}



