#include "greferencedocdialog.h"
#include "ginterface.h"

#include <QDesktopWidget>


GReferenceDocDialog::GReferenceDocDialog(const QString &title, const QString &url, QWidget *parent) :
    QDialog(parent), m_ui(new Ui::GReferenceDocDialog) {
    m_ui->setupUi(this);

    setWindowTitle(title);
    m_ui->m_webDocs->load(QUrl(url));
}

GReferenceDocDialog::~GReferenceDocDialog() {
    delete m_ui;
}
