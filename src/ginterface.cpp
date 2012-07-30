#include "ginterface.h"

#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>


GInterface::GInterface() {
}

void GInterface::centerWidgetOnScreen(QWidget *widget) {
    QRect screen_resolution = QApplication::desktop()->screenGeometry();
    widget->move(screen_resolution.center()-widget->rect().center());
}
