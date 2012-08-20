#ifndef GREFERENCEDOCDIALOG_H
#define GREFERENCEDOCDIALOG_H

#include "ui_gdocdialog.h"

#include <QDialog>


namespace Ui {
    class GReferenceDocDialog;
}

/*!
 * \brief Dialog containing documentation for QGothic.
 * \ingroup Gui
 */
class GDocDialog : public QDialog {
	Q_OBJECT
	
    public:
	explicit GDocDialog(const QString &title,
				     const QString &url, QWidget *parent = 0);
	~GDocDialog();

    private:
	Ui::GDocDialog *m_ui;

};

#endif // GREFERENCEDOCDIALOG_H
