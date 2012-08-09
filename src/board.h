#ifndef BOARD_H
#define BOARD_H

#include "figure.h"
#include "location.h"
#include "definitions.h"

#include <QList>
#include "gsettings.h"


class Move;
class QString;

/*!
 * \brief Internal Board implementation.
 *
 * Not a graphical Board.
 */
class Board {

    public:
	/*!
	 * \brief All possible states of the Board.
	 */
	enum State {
	    WHITE_WON = 101,
	    BLACK_WON = 102,
	    DRAW = 103,
	    ORDINARY = 104
	};

	/*!
	 * \brief Constructs new Board.
	 */
	Board(int max_moves_without_jump = GSettings::value(SET_GAME,
							    "max_moves_without_jump", 60).toInt(),
	      int size = 8);

	/*!
	 * \brief Constructs new Board by copying board.
	 */
	Board(const Board &board);
	~Board();

	QList<Location> getLocations(Figure::Color color);
	void updateState();
	void setInitialPositions();
	int getSize() const;
	Board::State getState() const;
	void setState(Board::State state);
	int getActualMovesNoJump() const;
	void setActualMovesNoJump(int moves_without_jump);
	int getMaxMovesNoJump() const;
	void setMaxMovesNoJump(int moves_without_jump);

	/*!
	 * \brief Makes Move on this Board.
	 * \param move Move to make.
	 * \param update_state If true, then properties of this Board are updated after the Move is take.
	 */
	void makeMove(const Move &move, bool update_state = true);

	/*!
	 * \brief Makes inverse Move on this Board.
	 *
	 * This is used by undo functionality and by AI algorithms.
	 * \param move Move to undo.
	 * \param update_state If true, then properties of this Board are updated after the Move is taken.
	 */
	void makeInverseMove(const Move &move, bool update_state = true);

	Figure::Type &operator()(int column, int row);
	Figure::Type operator()(int column, int row) const;

	Figure::Type &operator()(Location location);
	Figure::Type operator()(Location location) const;

    private:
	int m_size;
	int m_maxMovesNoJump;
	int m_actualMovesNoJump;
	Board::State m_state;

	Figure::Type **m_field;

};

#endif // BOARD_H
