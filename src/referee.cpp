#include "referee.h"
#include "board.h"

#include <cstdlib>


Referee::Referee() {
}

QList<Move> Referee::getMoves(const Figure::Color &color, Board &board) {
    QList<Location> locations = board.getLocations(color);
    QList<Move> moves;

    foreach (Location loc, locations) {
	moves.append(getAllMoves(loc, board));
    }
    return getBestMoves(moves);
}

QList<Move> Referee::getAllMoves(const Location &location, Board &board) {
    switch (board(location)) {
	case Figure::WHITE_PAWN:
	case Figure::BLACK_PAWN:
	    return getAllPawnMoves(location, board);
	case Figure::WHITE_QUEEN:
	case Figure::BLACK_QUEEN:
	    return getAllQueenMoves(location, board);
	default:
	    return QList<Move>();
    }
}

QList<Move> Referee::getAllQueenMoves(const Location &location, Board &board) {
    QList<Move> moves = getQueenJumps(location, board);
    if (moves.size() > 0) {
	return moves;
    }
    else {
	return getQueenMoves(location, board);
    }
}

QList<Move> Referee::getAllPawnMoves(const Location &location, Board &board) {
    QList<Move> moves = getPawnJumps(location, board);
    if (moves.size() > 0) {
	return moves;
    }
    else {
	return getPawnMoves(location, board);
    }
}

bool Referee::isInQueenArea(const Location &location, const Figure::Color &color_of_figure,
			    const Board &board) {

    switch (color_of_figure) {
	case Figure::WHITE:
	    return location.getY() == board.getSize()-1;
	case Figure::BLACK:
	    return location.getY() == 0;
	default:
	    return false;
    }
}

QList<Move> Referee::getBestMoves(const QList<Move> &moves) {
    QList<Move> list_of_moves;
    int best_price = 0;
    foreach (Move move, moves) {
	if (move.price() > best_price) {
	    best_price = move.price();
	    list_of_moves.clear();
	    list_of_moves.append(move);
	}
	else if (move.price() == best_price) {
	    list_of_moves.append(move);
	}
    }
    return list_of_moves;
}

QList<Move> Referee::getQueenJumps(const Location &location, Board &board) {
    QList<Move> moves;

    foreach (Location dir, getQueenDirections()) {
	Location current_location(location);
	int count = 0;
	do {
	    if (count++ > 100) {
		break;
	    }
	    current_location += dir;
	    if (current_location.isOnBoard(board) && Figure::getColorByType(board(current_location)) == Figure::negateColor(Figure::getColorByType(board(location)))) {
		Location next_location = current_location + dir;
		while (next_location.isOnBoard(board) && next_location.isFree(board)) {
		    Move move;
		    move.setFigureType(board(location));
		    move.setFrom(Location(location));
		    move.setTo(Location(next_location));
		    move.addJumpedFigure(current_location, board(current_location));
		    board.makeMove(move, false);
		    QList<Move> next_moves = getQueenJumps(next_location, board);
		    if (next_moves.size() > 0) {
			foreach (Move next_move, next_moves) {
			    Move final_move(move);
			    final_move.setTo(Location(next_move.getTo()));
			    final_move.addJumpedFigures(next_move.getJumpedFigures());
			    moves.append(final_move);
			}
		    }
		    else {
			moves.append(move);
		    }
		    board.makeInverseMove(move, false);
		    next_location += dir;
		}
		break;
	    }
	}
	while (current_location.isOnBoard(board) && current_location.isFree(board));
    }
    return moves;
}

QList<Move> Referee::getPawnJumps(const Location &location, Board &board) {
    QList<Move> moves;

    foreach (Location dir, getPawnDirections()) {
	Location current_location(location);
	current_location += Figure::getColorByType(board(location)) == Figure::WHITE ? dir : !dir;

	if (current_location.isOnBoard(board) && current_location.isFree(board) == false) {
	    if (Figure::getColorByType(board(current_location)) == Figure::negateColor(Figure::getColorByType(board(location)))) {
		Location next_location = current_location + (Figure::getColorByType(board(location)) == Figure::WHITE ? dir : !dir);
		if (next_location.isOnBoard(board) && next_location.isFree(board)) {
		    Move move;
		    move.setFigureType(board(location));
		    move.setFrom(Location(location));
		    move.setTo(Location(next_location));
		    move.addJumpedFigure(current_location, board(current_location));

		    if (isInQueenArea(move.getTo(), Figure::getColorByType(board(move.getFrom())), board) && !isInQueenArea(move.getFrom(), Figure::getColorByType(board(move.getFrom())), board)) {
			move.setPromoted(true);
			moves.append(move);
			next_location += Figure::getColorByType(board(location)) == Figure::WHITE ? dir : !dir;
			continue;
		    }

		    board.makeMove(move, false);
		    QList<Move> next_moves = getPawnJumps(next_location, board);
		    if (next_moves.size() > 0) {
			foreach (Move next_move, next_moves) {
			    Move final_move(move);
			    final_move.setTo(Location(next_move.getTo()));
			    final_move.addJumpedFigures(next_move.getJumpedFigures());
			    final_move.setPromoted(next_move.getPromoted());
			    moves.append(final_move);
			}
		    }
		    else {
			moves.append(move);
		    }
		    board.makeInverseMove(move, false);
		    next_location += Figure::getColorByType(board(location)) == Figure::WHITE ? dir : !dir;
		}
	    }
	}
    }
    return moves;
}

QList<Move> Referee::getPawnMoves(const Location &location, Board &board) {
    QList<Move> moves;
    foreach (Location dir, getPawnDirections()) {
	Location current_location = Figure::getTypeByColor(Figure::WHITE).contains(board(location)) ?
					location + dir :
					location + !dir;
	if (current_location.isOnBoard(board)) {
	    if (board(current_location) == Figure::EMPTY) {
		Move move;
		move.setFrom(location);
		move.setTo(current_location);
		move.setFigureType(board(location));
		if (isInQueenArea(move.getTo(), Figure::getColorByType(board(move.getFrom())), board)) {
		    move.setPromoted(true);
		}
		moves.append(move);
	    }
	}
    }
    return moves;
}

QList<Move> Referee::getQueenMoves(const Location &location, Board &board) {
    QList<Move> moves;
    foreach (Location dir, getQueenDirections()) {
	Location current_location(location);
	for (int i = 0; i < board.getSize()-1; i++) {
	    current_location += dir;
	    if (current_location.isOnBoard(board)) {
		if (board(current_location) == Figure::EMPTY) {
		    Move move;
		    move.setFrom(location);
		    move.setTo(current_location);
		    move.setFigureType(board(location));
		    moves.append(move);
		}
		else {
		    break;
		}
	    }
	    else {
		break;
	    }
	}
    }
    return moves;
}

const QList<Location> &Referee::getPawnDirections() {
    if (s_pawnDirections == NULL) {
	s_pawnDirections = new QList<Location>();
	//*s_pawnDirections << Location(1, 0);
	*s_pawnDirections << Location(1, 1);
	//*s_pawnDirections << Location(0, 1);
	*s_pawnDirections << Location(-1, 1);
	//*s_pawnDirections << Location(-1, 0);
    }
    return *s_pawnDirections;
}

const QList<Location> &Referee::getQueenDirections() {
    if (s_queenDirections == NULL) {
	s_queenDirections = new QList<Location>();
	//*s_queenDirections << Location(1, 0);
	*s_queenDirections << Location(1, 1);
	//*s_queenDirections << Location(0, 1);
	*s_queenDirections << Location(-1, 1);
	//*s_queenDirections << Location(-1, 0);
	*s_queenDirections << Location(-1, -1);
	//*s_queenDirections << Location(0, -1);
	*s_queenDirections << Location(1, -1);
    }
    return *s_queenDirections;
}
