#include "gaboutdialog.h"
#include "definitions.h"

#include <QDate>


GAboutDialog::GAboutDialog(QWidget *parent) : QDialog(parent), m_ui(new Ui::GAboutDialog) {
    m_ui->setupUi(this);

    // Setup connections.
    connect(m_ui->m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));

    // Filling in the information.
    m_ui->m_lblDescription->setText(tr("%1 %2<br><br>"
				       "<b>Compilation date:</b> %3 (%4)<br>"
				       "<b>Website:</b> %5<br>"
				       "<b>Author:</b> %7 &lt;%8&gt;<br>"
				       "<b>Qt Version:</b> %9 (compiled against %10)").arg(APP_NAME, APP_VERSION,
											   QLocale(QLocale::C).toDate(QString(__DATE__).simplified(),
														      QLatin1String("MMM d yyyy")).toString(Qt::DefaultLocaleLongDate),
											   QLocale(QLocale::C).toTime(QString(__TIME__).simplified(),
														      QLatin1String("hh:mm:ss")).toString(Qt::DefaultLocaleShortDate),
											   APP_URL,
											   APP_AUTHOR,
											   APP_EMAIL,
											   qVersion(),
											   QT_VERSION_STR));
}

GAboutDialog::~GAboutDialog() {
    delete m_ui;
}
