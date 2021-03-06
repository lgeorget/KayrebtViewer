#-------------------------------------------------
#
# Project created by QtCreator 2015-05-23T10:49:45
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KayrebtViewer
TEMPLATE = app
DEPENDPATH += . /usr/lib/graphviz
INCLUDEPATH += . /usr/include/graphviz
QMAKE_CXXFLAGS += -std=c++11

unix:!macx: LIBS += -L/usr/lib/graphviz/ `pkg-config libgvc --libs`


SOURCES += main.cpp\
    sourcetreewidget.cpp \
    databaseviewer.cpp \
    viewer.cpp \
    node.cpp \
    graph.cpp \
    element.cpp \
    edge.cpp \
    drawing.cpp \
    databasesortfilterproxymodel.cpp \
    preferencesdialog.cpp \
    hyperlinkactivatedevent.cpp \
    graphitem.cpp \
    graphitemmodel.cpp \
    sourcetextviewer.cpp \
    kernelcodehighlighter.cpp \
    nodehoverevent.cpp

HEADERS  += \
    sourcetreewidget.h \
    databaseviewer.h \
    viewer.h \
    node.h \
    graph.h \
    element.h \
    edge.h \
    drawing.h \
    databasesortfilterproxymodel.h \
    preferencesdialog.h \
    hyperlinkactivatedevent.h \
    graphitem.h \
    graphitemmodel.h \
    sourcetextviewer.h \
    kernelcodehighlighter.h \
    nodehoverevent.h \
    linenumberarea.h

FORMS    += \
    viewer.ui \
    preferencesdialog.ui \
    databaseviewer.ui

OTHER_FILES +=
