#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractItemModel>


class History;

/*!
 * \brief Model for abstracting History for MVC architecture.
 *
 * This model is used in conjunction with competent View, usually QTreeView or its subclass.
 *
 * \note This class is part of MVC.
 */
class HistoryModel : public QAbstractItemModel {
	Q_OBJECT

    public:
	explicit HistoryModel(History *history, QObject *parent = 0);

	int columnCount(const QModelIndex &parent) const;
	int rowCount(const QModelIndex &parent) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	QVariant data(const QModelIndex &index, int role) const;
	QModelIndex index(int row, int column, const QModelIndex &parent) const;
	QModelIndex parent(const QModelIndex &child) const;
	
    public slots:
	void setInitialPlayerHeader(int current_player);

    private:
	History *m_history;
	int m_columnCount;
	QList<QString> m_initialIndex;

};

#endif // HISTORYMODEL_H
