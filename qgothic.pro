#------------------------------------------------------
# qgothic
# this is project file
# author: Martin Rotter <rotter.martinos(at)gmail.com>
#------------------------------------------------------

TARGET	    = qgothic
TEMPLATE    = app
QT	    += widgets core webkitwidgets xml	    # Remove widgets when compiling with Qt4.
#CONFIG	    += console			    # Uncomment to see real-time debug messages.
CONFIG	    -= app_bundle
DEFINES	    *= QT_USE_QSTRINGBUILDER


DESTDIR	    = build
OBJECTS_DIR = build
MOC_DIR	    = build
UI_DIR	    = build
INCLUDEPATH = src
INCLUDEPATH += src/3rd-party
SUBDIRS	    += extensions
CODECFORTR  = UTF-8
LANGUAGE    = C++


SOURCES += src/main.cpp \
    src/location.cpp \
    src/player.cpp \
    src/figure.cpp \
    src/board.cpp \
    src/move.cpp \
    src/referee.cpp \
    src/algorithms.cpp \
    src/strategy.cpp \
    src/intelligence.cpp \
    src/game.cpp \
    src/generator.cpp \
    src/gboard.cpp \
    src/gmainwindow.cpp \
    src/gsettingsdialog.cpp \
    src/ginterface.cpp \
    src/history.cpp \
    src/historyitem.cpp \
    src/historymodel.cpp \
    src/gtreeview.cpp \
    src/gsettings.cpp \
    src/3rd-party/simplecrypt.cpp \
    src/gspinbox.cpp \
    src/gaboutdialog.cpp \
    src/debug.cpp \
    src/gdocdialog.cpp \
    src/gmovedialog.cpp

HEADERS += \
    src/location.h \
    src/player.h \
    src/figure.h \
    src/board.h \
    src/move.h \
    src/referee.h \
    src/algorithms.h \
    src/strategy.h \
    src/intelligence.h \
    src/game.h \
    src/generator.h \
    src/gboard.h \
    src/definitions.h \
    src/gmainwindow.h \
    src/gsettingsdialog.h \
    src/ginterface.h \
    src/history.h \
    src/historyitem.h \
    src/historymodel.h \
    src/gtreeview.h \
    src/gsettings.h \
    src/3rd-party/simplecrypt.h \
    src/documentation.h \
    src/gspinbox.h \
    src/gaboutdialog.h \
    src/debug.h \
    src/gdocdialog.h \
    src/gmovedialog.h

FORMS += \
    ui/gmainwindow.ui \
    ui/gsettingsdialog.ui \
    ui/gaboutdialog.ui \
    ui/gdocdialog.ui \
    ui/gmovedialog.ui

RESOURCES += \
    resources/qgothic.qrc

TRANSLATIONS    = "resources/translations/English (en).ts" \
    "resources/translations/Cestina (cs).ts"

# OS dependent settings
win32 {
    win32:RC_FILE = resources/windows/qgothic.rc
}

OTHER_FILES += \
    "resources/translations/English (en).ts" \
    "resources/translations/Cestina (cs).ts"
