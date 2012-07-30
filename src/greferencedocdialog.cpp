#include "greferencedocdialog.h"
#include "ginterface.h"

#include <QDesktopWidget>


GReferenceDocDialog::GReferenceDocDialog(QWidget *parent) :
    QDialog(parent), m_ui(new Ui::GReferenceDocDialog) {
    m_ui->setupUi(this);


    connect(m_ui->m_webDocs, SIGNAL(loadFinished(bool)), this, SLOT(adjustGeometry()));
    m_ui->m_webDocs->load(QUrl("qrc:/support/doc/index.html"));
}

GReferenceDocDialog::~GReferenceDocDialog() {
    delete m_ui;
}

void GReferenceDocDialog::adjustGeometry() {
    QRect screen_resolution = QApplication::desktop()->screenGeometry();
    resize(screen_resolution.width()*0.85, screen_resolution.height()*0.8);
    GInterface::centerWidgetOnScreen(this);
}
