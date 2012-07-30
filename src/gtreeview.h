#ifndef GTREEVIEW_H
#define GTREEVIEW_H

#include <QTreeView>


class HistoryModel;

class GTreeView : public QTreeView {
	Q_OBJECT

    public:
	explicit GTreeView(QWidget *parent = 0);

	void setCurrentModel(HistoryModel *model);

    protected:
	void currentChanged(const QModelIndex &current, const QModelIndex &previous);
	
    signals:
	void currentRowChanged(int current, int previous);

    public slots:
	void setCurrentRow(int index);
	
    private:
	HistoryModel *m_model;
	bool m_programmaticalChange;
};

#endif // GTREEVIEW_H
