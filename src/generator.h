#ifndef GENERATOR_H
#define GENERATOR_H

#include "move.h"
#include "player.h"
#include "board.h"
#include "intelligence.h"
#include "referee.h"
#include "algorithms.h"
#include "strategy.h"

#include <ctime>

#include <QThread>


/*!
 * \brief Thread-separated Move Generator.
 *
 * This class subclasses QThread and is able to generate Moves.
 * \ingroup Core
 */
class Generator : public QThread {
	Q_OBJECT

    public:
	Generator(QObject *parent = 0);

	/*!
	 * \brief Top-level method for finding Moves.
	 * \param applicant Player, which is looking for Move.
	 * \param board Board to look for the Move on.
	 */
	void searchMove(Player applicant, Board &board);

    signals:
	/*!
	 * \brief Emitted when Move search should be started.
	 *
	 * This is forwarded signal to the Intelligence worker.
	 * \param applicant Player, which is looking for Move.
	 * \param board Board to look for the Move on.
	 */
	void searchRequested(const Player &applicant, Board &board);

	/*!
	 * \brief Emitted when Move is found.
	 *
	 * This is forwarded signal from the Intelligence worker.
	 * \param move Generated Move.
	 */
	void moveFound(Move move);

	/*!
	 * \brief Emitted when Move search is cancelled.
	 *
	 * This is forwarded signal from the Intelligence worker.
	 */
	void cancelled();

	void countOfCalls(int count);
	void rankOfCall(int rank);
	void moveForHumanFound(Move move);

    public slots:
	void cancel() {
	    Intelligence::cancel(true);
	}

    protected:
	/*!
	 * \brief Main code generator container.
	 *
	 * Event loop is started for this thread and passing of signals is therefore enabled.
	 * \note This is the code, that is executed in separated thread.
	 */
	void run();

    private:
	bool m_ready;

};

#endif // GENERATOR_H
