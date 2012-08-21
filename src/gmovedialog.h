#ifndef MOVEDIALOG_H
#define MOVEDIALOG_H

#include "move.h"
#include "ui_gmovedialog.h"

#include <QDialog>
#include <QList>


namespace Ui {
    class GMoveDialog;
}

class GMoveDialog : public QDialog {
	Q_OBJECT
	
    public:
	explicit GMoveDialog(const QList<Move> &moves, QWidget *parent = 0);
	~GMoveDialog();

    public slots:
	int exec(int *index);

    protected slots:
	void updateIndex();
	
    private:
	Ui::GMoveDialog *m_ui;
	int *m_index;

};

#endif // MOVEDIALOG_H
