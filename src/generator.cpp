#include "generator.h"
#include "board.h"
#include "intelligence.h"

#include <QDebug>


Generator::Generator(QObject *parent) : QThread(parent), m_active(false) {
}



void Generator::cancel() {
    qDebug() << "Move search cancelled.";
    m_active = false;
    terminate();
}

void Generator::searchMove(Player applicant, Board *board) {
    if (m_active) {
	return;
    }
    else {
	m_applicant = applicant;
	m_board = board;
	start();
    }
}

bool Generator::isActive() const {
    return m_active;
}

void Generator::run() {
    if (m_active) {
	return;
    }
    else {
	m_active = true;
	Move move = Intelligence::computerMove(m_applicant, *m_board);
	emit moveGenerated(move);
	//qDebug() << move.toString();
	m_active = false;
    }
}
