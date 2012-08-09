#include "board.h"
#include "move.h"

#include <QString>
#include <QDebug>


Board::Board(int max_moves_without_jump, int size) :
    m_size(size), m_maxMovesNoJump(max_moves_without_jump),
    m_actualMovesNoJump(0), m_state(Board::ORDINARY) {
    m_field = new Figure::Type*[m_size];
    for (int i = 0; i < m_size; i++) {
	m_field[i] = new Figure::Type[m_size];
    }

    setInitialPositions();
}

Board::Board(const Board &board) {
    m_size = board.getSize();
    m_maxMovesNoJump = board.getMaxMovesNoJump();
    m_actualMovesNoJump = board.getActualMovesNoJump();
    m_state = board.getState();
    m_field = new Figure::Type*[m_size];
    for (int i = 0; i < m_size; i++) {
	m_field[i] = new Figure::Type[m_size];
    }

    for (int i = 0; i < m_size; i++) {
	for (int j = 0; j < m_size; j++) {
	    operator()(j, i) = board(j, i);
	}
    }
}

Board::~Board() {
    for (int i = 0; i < m_size; i++) {
	delete [] m_field[i];
    }
    delete [] m_field;
}

Figure::Type &Board::operator()(int column, int row) {
    return m_field[row][column];
}

Figure::Type Board::operator()(int column, int row) const{
    return m_field[row][column];
}

Figure::Type &Board::operator()(Location location) {
    return operator()(location.getX(), location.getY());
}

Figure::Type Board::operator()(Location location) const {
    return operator()(location.getX(), location.getY());
}

QList<Location> Board::getLocations(Figure::Color color) {
    QList<Location> list;
    for (int i = 0; i < m_size; i++) {
	for (int j = 0; j < m_size; j++) {
	    if (Figure::getTypeByColor(color).contains(m_field[i][j])) {
		list.append(Location(j, i));
	    }
	}
    }
    return list;
}

void Board::updateState() {
    if (m_actualMovesNoJump >= m_maxMovesNoJump) {
	//qDebug() << "max_jumps exceeded";
	int black_count = Board::getLocations(Figure::BLACK).size();
	int white_count = Board::getLocations(Figure::WHITE).size();
	if (black_count > white_count) {
	    m_state = Board::BLACK_WON;
	}
	else if (white_count > black_count) {
	    m_state = Board::WHITE_WON;
	}
	else {
	    m_state = Board::DRAW;
	}
    }
    else if (Board::getLocations(Figure::BLACK).size() == 0) {
	m_state = Board::WHITE_WON;
	//qDebug() << "black_count: 0";
    }
    else if (Board::getLocations(Figure::WHITE).size() == 0) {
	m_state = Board::BLACK_WON;
	//qDebug() << "white_count: 0";
    }
    else {
	m_state = Board::ORDINARY;
    }
}

int Board::getSize() const {
    return m_size;
}

Board::State Board::getState() const {
    return m_state;
}

void Board::setState(State state) {
    m_state = state;
}

int Board::getActualMovesNoJump() const {
    return m_actualMovesNoJump;
}

void Board::setActualMovesNoJump(int moves_without_jump) {
    m_actualMovesNoJump = moves_without_jump;
}

void Board::setMaxMovesNoJump(int moves_without_jump) {
    m_maxMovesNoJump = moves_without_jump;
}

int Board::getMaxMovesNoJump() const {
    return m_maxMovesNoJump;
}

void Board::setInitialPositions() {    
    for (int i = 0; i < m_size; i++) {
	for (int j = 0; j < m_size; j++) {
	    switch (i) {
		case 0:
		case 1:
		    m_field[i][j] = Figure::WHITE_PAWN;
		    break;
		case 6:
		case 7:
		    m_field[i][j] = Figure::BLACK_PAWN;
		    break;
		default:
		    m_field[i][j] = Figure::EMPTY;
		    break;
	    }
	}
    }
}

void Board::makeMove(const Move &move, bool update_state) {
    if (move.getJumpedFigures().size() > 0) {
	m_actualMovesNoJump = 0;
    }
    else {
	m_actualMovesNoJump++;
    }

    if (move.isInvalid() == false) {
	Figure::Type current = operator()(move.getFrom());
	operator()(move.getFrom()) = Figure::EMPTY;

	foreach (JumpedFigure fig, move.getJumpedFigures()) {
	    operator()(fig.first) = Figure::EMPTY;
	}

	if (move.getPromoted()) {
	    operator()(move.getTo()) = Figure::promote(current);
	}
	else {
	    operator()(move.getTo()) = current;
	}
    }

    if (update_state) {
	updateState();
    }
}

void Board::makeInverseMove(const Move &move, bool update_state) {
    if (move.isInvalid() == false) {
	if (move.getPromoted()) {
	    operator()(move.getFrom()) = Figure::degrade(operator()(move.getTo()));
	}
	else {
	    operator()(move.getFrom()) = operator()(move.getTo());
	}
	operator()(move.getTo()) = Figure::EMPTY;

	foreach (JumpedFigure fig, move.getJumpedFigures()) {
	    operator()(fig.first) = fig.second;
	}
    }

    if (update_state) {
	updateState();
    }
}
