#ifndef INTELLIGENCE_H
#define INTELLIGENCE_H

#include "move.h"
#include "player.h"

#include <QObject>


class Board;

/*!
 * \brief AI best Move selectors.
 * \ingroup Core
 */
class Intelligence : public QObject {
	Q_OBJECT

    signals:
	void moveForHumanFound(Move move);
	void moveFound(Move move);
	void countOfCalls(int count);
	void rankOfCall(int rank);
	void cancelled();

    public slots:
	void computerMove(Player applicant, Board &board);

    public:
	Intelligence(QObject *parent = 0);
	static bool m_cancelling;

	static void cancel(bool really_cancel) {
	    m_cancelling = really_cancel;
	}

	static bool isCancelling() {
	    return m_cancelling;
	}

    protected:
	/*!
	 * \brief Top-level method for finding best move according to CPU player difficulty.
	 * \param applicant Player, that wants to find a Move.
	 * \param board Board to seek the Move on.
	 * \return Method returns best available Move for this Player.
	 */

	/*!
	 * \brief Method for finding random move.
	 * \param applicant Player, that wants to find a Move.
	 * \param board Board to seek the Move on.
	 * \return Method returns random available Move for this Player.
	 */
	void randomMove(Player applicant, Board &board);

	/*!
	 * \brief Method for finding best available Move using traditional Minimax algorithm.
	 * \param applicant Player, that wants to find a Move.
	 * \param board Board to seek the Move on.
	 * \return Method returns best available Move for this Player.
	 */
	void minimaxMove(Player applicant, Board &board,
			 int depth, int (*eval_function)(Board &));

	void alfabetaMove(Player applicant, Board &board,
			  int depth, int (*eval_function)(Board &));

};

#endif // INTELLIGENCE_H
