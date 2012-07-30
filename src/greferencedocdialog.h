#ifndef GREFERENCEDOCDIALOG_H
#define GREFERENCEDOCDIALOG_H

#include "ui_greferencedocdialog.h"

#include <QDialog>


namespace Ui {
    class GReferenceDocDialog;
}

class GReferenceDocDialog : public QDialog {
	Q_OBJECT
	
    public:
	explicit GReferenceDocDialog(QWidget *parent = 0);
	~GReferenceDocDialog();

    protected slots:
	void adjustGeometry();
	
    private:
	Ui::GReferenceDocDialog *m_ui;
};

#endif // GREFERENCEDOCDIALOG_H
