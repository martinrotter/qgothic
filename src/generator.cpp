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
    // Problem s generatorem. Končil na použití generátoru, které nedávalo smysl.
    qDebug() << "Starting move search. Thread is running:" << isRunning();
    if (m_active) {
	qWarning() << "Generator is currently in use.";
	return;
    }
    else {
	m_applicant = applicant;
	m_board = board;
	start();
    }
}

void Generator::run() {
    //setPriority(QThread::TimeCriticalPriority);
    m_active = true;
    Move move = Intelligence::computerMove(m_applicant, *m_board);
    //qDebug() << move.toString();
    m_active = false;

    emit moveGenerated(move);
    //terminate();
    //wait();

    // Patrně je třeba nejdříve nastavit stav generátoru na false a až pak generovat signál,
    // protože podle dokumentace není v nefrontovém S/S spojení definováno pořadí vykonání
    // listenerů a aktuálního kódu v této metodě.

}
