#include "generator.h"

#include <QDebug>
#include <QMetaType>


Generator::Generator(QObject *parent) : QThread(parent), m_ready(false) {
    // Start the worker thread.
    start();

    // Wait for the worker thread to be ready.
    while(!m_ready) {
	msleep(50);
    }
}

void Generator::searchMove(Player applicant, Board &board) {
    emit searchRequested(applicant, board);
}

void Generator::run() {
    qDebug() << "Entering thread with"
	     << Q_FUNC_INFO << "and code"
	     << QThread::currentThreadId() << ".";

    Intelligence worker;

    qRegisterMetaType<Move>("Move");
    qRegisterMetaType<Player>("Player");
    qRegisterMetaType<Board>("Board&");

    // Pass sorting requests to SorterWorker in the worker thread.
    connect(this, SIGNAL(searchRequested(Player,Board&)),
	    &worker, SLOT(computerMove(Player,Board&)));
    connect(&worker, SIGNAL(moveFound(Move)),
	    this, SIGNAL(moveFound(Move)));
    connect(&worker, SIGNAL(cancelled()), this, SIGNAL(cancelled()));
    connect(&worker, SIGNAL(countOfCalls(int)), this, SIGNAL(countOfCalls(int)));
    connect(&worker, SIGNAL(rankOfCall(int)), this, SIGNAL(rankOfCall(int)));
    connect(&worker, SIGNAL(moveForHumanFound(Move)), this, SIGNAL(moveForHumanFound(Move)));

    // Mark the worker thread as ready.
    m_ready = true;

    // Event loop (necessary to process signals).
    exec();
    qDebug() << "Leaving thread with"
	     << Q_FUNC_INFO << "and code"
	     << QThread::currentThreadId() << ".";
}
