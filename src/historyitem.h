#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include "move.h"


class HistoryItem {

    public:
	enum Section {
	    ID	    = 0,
	    PLAYER  = 1,
	    MOVE    = 2
	};

	HistoryItem(Move m_move, int id, int moves);
	~HistoryItem();

	Move *getMove() const;
	int getMovesWithoutJump() const;
	int getOrdinalNumber() const;

	QString getInfo(HistoryItem::Section section) const;

    private:
	Move *m_move;
	int m_movesWithoutJump;
	int m_ordinalNumber;

};

#endif // HISTORYITEM_H
