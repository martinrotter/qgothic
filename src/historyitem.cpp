#include "historyitem.h"


HistoryItem::HistoryItem(Move move, int id, int moves) {
    this->m_move = new Move(move);
    m_ordinalNumber = id;
    m_movesWithoutJump = moves;
}

HistoryItem::~HistoryItem() {
    delete m_move;
}

Move *HistoryItem::getMove() const {
    return m_move;
}

int HistoryItem::getMovesWithoutJump() const {
    return m_movesWithoutJump;
}

int HistoryItem::getOrdinalNumber() const {
    return m_ordinalNumber;
}

QString HistoryItem::getInfo(HistoryItem::Section section) const {
    switch (section) {
	case HistoryItem::ID:
	    return QString::number(getOrdinalNumber());
	case HistoryItem::MOVE:
	    return getMove()->toString();
	case HistoryItem::PLAYER:
	    return Figure::getColorByType(getMove()->getFigureType()) == Figure::WHITE
		    ? QObject::tr("White")
		    : QObject::tr("Black");
	default:
	    return getMove()->toString();
    }
}
