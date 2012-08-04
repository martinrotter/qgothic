#ifndef BOARD_H
#define BOARD_H

#include "figure.h"
#include "location.h"

#include <QList>


class Move;
class QString;

class Board {

    public:
	enum State {
	    WHITE_WON = 101,
	    BLACK_WON = 102,
	    DRAW = 103,
	    ORDINARY = 104
	};

	Board(int max_moves_without_jump = 30, int size = 8);
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

	void makeMove(const Move &move, bool update_state = true);
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
