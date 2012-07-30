#ifndef HISTORY_H
#define HISTORY_H

#include "move.h"

#include <QDebug>


class HistoryItem;

class History : public QObject {
	Q_OBJECT

    public:
	History(QObject *parent = 0);
	~History();

	HistoryItem *at(int index) const;
	void clear();
	int count() const;
	bool canUndo() const;
	bool canRedo() const;

	void setIndex(int index);
	int getIndex() const;
	void increment();
	void decrement();

	void consolidate();

    public slots:
	void addMove(Move move, int moves_without_jumps);

    private:
	QList<HistoryItem*> *m_history;
	// označuje poslední provedený tah v historii
	// při přidání tahu do historie se nastaví na index právě přidaného prvku
	// je-li index 0-tak nelze už UNDO
	// je-li index roven poslednímu indexu z m_history tak nelze REDO
	int m_index;

    signals:
	void changed(int);

};

#endif // HISTORY_H
