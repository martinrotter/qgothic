#include "history.h"
#include "historyitem.h"

#include <QDebug>


History::History(QObject *parent)
    : QObject(parent), m_history(new QList<HistoryItem*>()), m_index(0) {
    emit changed(m_index);
}

History::~History() {
    clear();
    delete m_history;
}

void History::clear() {
    m_history->clear();
    m_index = 0;

    m_history->append(new HistoryItem(Move(), 1, 0));

    emit changed(m_index);
}

HistoryItem *History::at(int index) const {
    return m_history->at(index);
}

int History::count() const {
    return m_history->size();
}

bool History::canUndo() const {
    return m_index > 0;
}

bool History::canRedo() const {
    return m_index < m_history->size()-1;
}

void History::setIndex(int index) {
    m_index = index;
    emit changed(m_index);
}

int History::getIndex() const {
    return m_index;
}

void History::increment() {
    m_index++;
    emit changed(m_index);
}

void History::decrement() {
    m_index--;
    emit changed(m_index);
}

void History::consolidate() {
    if (m_index >= 0) {
	bool chopped = false;
	while (m_history->size() > m_index+1) {
	    delete m_history->last();
	    m_history->removeLast();
	    chopped = true;
	}
	if (chopped) {
	    emit changed(m_index);
	}

    }
}

void History::addMove(Move move, int moves_without_jumps) {
    HistoryItem *new_item = new HistoryItem(move, m_history->size()+1, moves_without_jumps);

    m_history->append(new_item);
    m_index = m_history->size()-1;

    emit changed(m_index);

    /*
    qDebug() << m_history->size();
    foreach (HistoryItem *item, *m_history) {
	qDebug() << "historie " << item->getMove()->toString();
    }
    */

}
