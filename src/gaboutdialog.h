#ifndef DIALOGABOUT_H
#define DIALOGABOUT_H

#include "ui_gaboutdialog.h"

#include <QDialog>


namespace Ui {
    class DialogAbout;
}

class GAboutDialog : public QDialog {
	Q_OBJECT
	
    public:
	GAboutDialog(QWidget *parent = 0);
	~GAboutDialog();
	
    private:
	Ui::GAboutDialog *m_ui;

};

#endif // DIALOGABOUT_H
