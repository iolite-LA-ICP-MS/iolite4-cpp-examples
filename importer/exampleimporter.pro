QT       -= gui
TEMPLATE = lib
CONFIG += plugin
TARGET = ExampleImporter

SOURCES +=  exampleimporter.cpp
HEADERS += exampleimporter.h

# Path to armadillo includes if not already available
INCLUDEPATH += /path/to/armadillo/include

# Path to iolite 4's core lib, e.g.
LIBS += /Applications/iolite4beta.app/Contents/Frameworks/libs/libiolite_core.1.dylib
