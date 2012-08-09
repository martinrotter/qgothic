#include "gspinbox.h"

#include <QKeyEvent>
#include <QLineEdit>


GSpinBox::GSpinBox(QWidget *parent) : QSpinBox(parent) {
}

void GSpinBox::setLineEditReadOnly(bool read_only) {
    lineEdit()->setReadOnly(read_only);
}

bool GSpinBox::isLineEditReadOnly() const {
    return lineEdit()->isReadOnly();
}
