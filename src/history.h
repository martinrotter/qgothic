#ifndef HISTORY_H
#define HISTORY_H

#include "move.h"

#include <QDebug>


class HistoryItem;

/*!
 * \brief History container.
 *
 * This class contains entire History needed to saving/loading of games.
 */
class History : public QObject {
	Q_OBJECT

    public:
	History(QObject *parent = 0);
	~History();

	/*!
	 * \brief Accessor for each HistoryItem of this History.
	 * \param index Index of HistoryItem.
	 * \return Method returns HistoryItem at specified index.
	 */
	HistoryItem *at(int index) const;

	/*!
	 * \brief Clears the entire History.
	 */
	void clear();

	/*!
	 * \brief Size of History.
	 * \return Method returns the size of history.
	 */
	int count() const;

	/*!
	 * \brief Undo indicator.
	 * \return Method returns true if there can be undo done.
	 */
	bool canUndo() const;

	/*!
	 * \brief Redo indicator.
	 * \return Method returns true if there can be redo done.
	 */
	bool canRedo() const;

	/*!
	 * \brief Sets active index. Index is used to indicate "active" item of History.
	 * \param index Index to set as active.
	 */
	void setIndex(int index);

	/*!
	 * \brief Gets active index.
	 * \return Method returns index.
	 */
	int getIndex() const;

	/*!
	 * \brief Increments index.
	 */
	void increment();

	/*!
	 * \brief Decrements index.
	 */
	void decrement();

	/*!
	 * \brief History consolidator.
	 *
	 * This method crops HistoryItems, which are "behind" active index.
	 * This means that of we did redo to the middle of History, (count is eg. 30, index is eg. 15)
	 * then all HistoryItems from second half of History are removed and last HistoryItem becomes
	 * active.
	 */
	void consolidate();

    public slots:
	/*!
	 * \brief Adds Move to History.
	 * \param move Move to add.
	 * \param moves_without_jumps Counter of Moves without jumps. This is useful for undo and redo.
	 */
	void addMove(Move move, int moves_without_jumps);

    private:
	QList<HistoryItem*> *m_history;
	// označuje poslední provedený tah v historii
	// při přidání tahu do historie se nastaví na index právě přidaného prvku
	// je-li index 0-tak nelze už UNDO
	// je-li index roven poslednímu indexu z m_history tak nelze REDO
	int m_index;

    signals:
	/*!
	 * \brief Emitted whenever the History is changed.
	 *
	 * \note This is primarily used by any MVC items, which need to reflect changes in History.
	 */
	void changed(int);

};

#endif // HISTORY_H
