#include "gdocdialog.h"
#include "ginterface.h"

#include <QDesktopWidget>


GDocDialog::GDocDialog(const QString &title, const QString &url, QWidget *parent) : QDialog(parent), m_ui(new Ui::GDocDialog) {
    m_ui->setupUi(this);

    // Prepare dialog.
    setFixedSize(width(), height());

    setWindowTitle(title);
    m_ui->m_webDocs->load(QUrl(url));
}

GDocDialog::~GDocDialog() {
    delete m_ui;
}
