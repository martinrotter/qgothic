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
    QModelIndex index_in_view = m_model->index(index, 0, QModelIndex());

    m_programmaticalChange = true;
    setCurrentIndex(index_in_view);
    selectionModel()->clearSelection();
    selectionModel()->select(index_in_view,
			     QItemSelectionModel::Select | QItemSelectionModel::Rows);
    m_programmaticalChange = false;
}

void GTreeView::currentChanged(const QModelIndex &current, const QModelIndex &previous) {
    QTreeView::currentChanged(current, previous);
    if (m_programmaticalChange == false && current.row() != previous.row()) {
	emit currentRowChanged(current.row(), previous.row());
    }
}
