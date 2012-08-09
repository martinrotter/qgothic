#include "historymodel.h"

#include "history.h"
#include "historyitem.h"


HistoryModel::HistoryModel(History *history, QObject *parent)
    : QAbstractItemModel(parent), m_history(history), m_columnCount(3) {

    setInitialPlayerHeader(0);
}

void HistoryModel::setInitialPlayerHeader(int current_player) {
    switch (static_cast<Figure::Color>(current_player)) {
	case Figure::WHITE:
	    m_initialIndex.clear();
	    m_initialIndex << tr("-") << tr("White starts") << tr("No Move");
	    reset();
	    break;
	case Figure::BLACK:
	    m_initialIndex.clear();
	    m_initialIndex << tr("-") << tr("Black starts") << tr("No Move");
	    reset();
	    break;
	default:
	    break;
    }
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
	case Qt::DisplayRole: {
	    Move *move = m_history->at(index.row())->getMove();
	    if (move->isInvalid() == false) {
		return m_history->at(index.row())->getInfo((HistoryItem::Section)index.column());
	    }
	    else {
		return tr("skipped");
	    }
	}
	case Qt::ToolTipRole: {
	    Move *move = m_history->at(index.row())->getMove();
	    if (move->isInvalid() == false) {
		return m_history->at(index.row())->getMove()->toFiguresString();
	    }
	    else {
		return tr("No moves were available for this player in this game phase.");
	    }
	}
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
