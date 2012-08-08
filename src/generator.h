#ifndef GENERATOR_H
#define GENERATOR_H

#include "move.h"
#include "player.h"

#include <QThread>


class Board;

/*!
 * \brief Thread-separated Move Generator.
 *
 * This class subclasses QThread and is able to generate Moves.
 */
class Generator : public QThread {
	Q_OBJECT

    public:
	explicit Generator(QObject *parent = 0);
	/*!
	 * \brief Main code generator container.
	 *
	 * \note This is the code, that is executed in separated thread.
	 */
	void run();
	void cancel();

    signals:
	/*!
	 * \brief Emitted when Move is found.
	 * \param move Generated Move.
	 */
	void moveGenerated(Move move);
	
    public slots:
	/*!
	 * \brief Top-level method for finding Moves.
	 * \param applicant Player, which is looking for Move.
	 * \param board Board to look for the Move on.
	 */
	void searchMove(Player applicant, Board *board);

    private:
	bool m_active;
	Player m_applicant;
	Board *m_board;

    signals:
	void cancelled();
	
};

#endif // GENERATOR_H
