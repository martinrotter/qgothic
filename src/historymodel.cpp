#include "historymodel.h"

#include "history.h"
#include "historyitem.h"


HistoryModel::HistoryModel(History *history, QObject *parent)
    : QAbstractItemModel(parent), m_history(history), m_columnCount(3) {

    m_initialIndex << tr("1") << tr("White starts") << tr("No Move");
}

int HistoryModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_columnCount;
}

int HistoryModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return m_history->count();
}

QModelIndex HistoryModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

QModelIndex HistoryModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) {
	return QModelIndex();
    }
    return createIndex(row, column);
}

QVariant HistoryModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= m_history->count() || index.column() >= m_columnCount) {
	return QVariant();
    }

    if ((role == Qt::DisplayRole || role == Qt::ToolTipRole) && index.row() == 0) {
	return m_initialIndex.at(index.column());
    }

    switch (role) {
	case Qt::DisplayRole:
	    return m_history->at(index.row())->getInfo((HistoryItem::Section)index.column());
	case Qt::ToolTipRole:
	    return m_history->at(index.row())->getMove()->toFiguresString();
	default:
	    return QVariant();
    }
}

QVariant HistoryModel::headerData(int section, Qt::Orientation orientation, int role) const {
    Q_UNUSED(orientation);
    if (role != Qt::DisplayRole) {
	return QVariant();
    }
    switch (section) {
	case 0:
	    return tr("No.");
	case 1:
	    return tr("Player");
	case 2:
	    return tr("Last Move");
	default:
	    return QVariant();
    }
}
