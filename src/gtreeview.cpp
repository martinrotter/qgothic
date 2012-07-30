#include "gtreeview.h"
#include "historymodel.h"

#include <QDebug>


GTreeView::GTreeView(QWidget *parent) : QTreeView(parent), m_programmaticalChange(true) {
}

void GTreeView::setCurrentModel(HistoryModel *model) {
    m_model = model;
    setModel(m_model);

    setColumnWidth(0, 30);
    setColumnWidth(1, 90);

    reset();
}

void GTreeView::setCurrentRow(int index) {
    reset();
    //qDebug() << "index v historii " << index;
    QModelIndex index_in_view = m_model->index(index, 0, QModelIndex());
    //scrollTo(index_in_view);
    //qDebug() << "validni index " << index_in_view.isValid();
    //if (index_in_view.isValid()) {
    m_programmaticalChange = true;
    setCurrentIndex(index_in_view);
    //}
    selectionModel()->clearSelection();
    selectionModel()->select(index_in_view,
			     QItemSelectionModel::Select | QItemSelectionModel::Rows);
    m_programmaticalChange = false;
}

void GTreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous) {
    QTreeView::currentChanged(current, previous);
    if (m_programmaticalChange == false && current.row() != previous.row()) {
	//qDebug() << "zmenena polozka - soucasna: " << current.row() << " minula: " << previous.row();
	emit currentRowChanged(current.row(), previous.row());
    }
}
