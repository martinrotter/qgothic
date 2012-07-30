#ifndef HISTORYMODEL_H
#define HISTORYMODEL_H

#include <QAbstractItemModel>


class History;

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
	
    signals:
	
    public slots:

    private:
	History *m_history;
	int m_columnCount;
	QList<QString> m_initialIndex;

};

#endif // HISTORYMODEL_H
