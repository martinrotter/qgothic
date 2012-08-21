#include "gmovedialog.h"


GMoveDialog::GMoveDialog(const QList<Move> &moves, QWidget *parent) : QDialog(parent), m_ui(new Ui::GMoveDialog) {
    m_ui->setupUi(this);

    // Prepare dialog.
    setFixedSize(sizeHint().width(), sizeHint().height());
    m_ui->m_movesList->setColumnWidth(0, 90);

    foreach (Move move, moves) {
	QString jumped_figures;
	QList<JumpedFigure> jumped_figures_list = move.getJumpedFigures();
	for (int i = 0; i < jumped_figures_list.size(); i++) {
	    if (i < jumped_figures_list.size()-1) {
		jumped_figures.append(jumped_figures_list.at(i).first.toString()).append(QLocale().decimalPoint()).append(" ");
	    }
	    else {
		jumped_figures.append(jumped_figures_list.at(i).first.toString());
	    }
	}

	QTreeWidgetItem *item = new QTreeWidgetItem(QStringList() << move.toString() << jumped_figures);
	item->setToolTip(0, move.toFiguresString());
	item->setToolTip(1, move.toFiguresString());
	m_ui->m_movesList->addTopLevelItem(item);
    }
}

GMoveDialog::~GMoveDialog() {
    delete m_ui;
}

void GMoveDialog::updateIndex() {
    if (m_index != NULL) {
	*m_index = m_ui->m_movesList->currentIndex().row();
	qDebug("Currently selected move is: %d", *m_index);
    }
}

int GMoveDialog::exec(int *index) {
    m_index = index;
    *m_index = -1;
    connect(m_ui->m_movesList, SIGNAL(itemSelectionChanged()), this, SLOT(updateIndex()));

    return QDialog::exec();
}
