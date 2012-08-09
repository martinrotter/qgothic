#include "generator.h"
#include "board.h"
#include "intelligence.h"

#include <QMutex>
#include <QDebug>


Generator::Generator(QObject *parent) : QThread(parent), m_active(false) {
}

void Generator::cancel() {
    terminate();
    qDebug() << "cancelled";
    m_active = false;
    emit cancelled();
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

void Generator::run() {
    if (m_active) {
	return;
    }
    else {
	//setPriority(QThread::TimeCriticalPriority);
	m_active = true;
	Move move = Intelligence::computerMove(m_applicant, *m_board);
	emit moveGenerated(move);
	//qDebug() << move.toString();
	m_active = false;
    }
}
