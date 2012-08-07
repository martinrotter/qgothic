#ifndef GTREEVIEW_H
#define GTREEVIEW_H

#include <QTreeView>


class HistoryModel;

/*!
 * \brief Viewer for History.
 *
 * \note This class is part of MVC.
 */
class GTreeView : public QTreeView {
	Q_OBJECT

    public:
	explicit GTreeView(QWidget *parent = 0);

	/*!
	 * \brief Sets active HistoryModel for this view.
	 * \param model HistoryModel to set as active.
	 */
	void setCurrentModel(HistoryModel *model);

    protected:
	void currentChanged(const QModelIndex &current, const QModelIndex &previous);
	
    signals:
	/*!
	 * \brief Emitted when user clicks certain HistoryItem in this view.
	 * \param current Currently active item.
	 * \param previous Previously active item.
	 */
	void currentRowChanged(int current, int previous);

    public slots:
	/*!
	 * \brief Sets currently active row in this view.
	 * \param index Index marks row index to set as active. Note that this index has something to do
	 * with index in History class.
	 */
	void setCurrentRow(int index);
	
    private:
	HistoryModel *m_model;
	bool m_programmaticalChange;
};

#endif // GTREEVIEW_H
