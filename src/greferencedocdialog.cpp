#include "greferencedocdialog.h"
#include "ginterface.h"

#include <QDesktopWidget>


GReferenceDocDialog::GReferenceDocDialog(QWidget *parent) :
    QDialog(parent), m_ui(new Ui::GReferenceDocDialog) {
    m_ui->setupUi(this);
    m_ui->m_webDocs->load(QUrl("qrc:/support/doc/index.html"));
}

GReferenceDocDialog::~GReferenceDocDialog() {
    delete m_ui;
}
