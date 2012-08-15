#include "strategy.h"
#include "board.h"


Strategy::Strategy() {
}

int Strategy::simple(Board &board) {
    QList<Location> loc_black = board.getLocations(Figure::BLACK);
    QList<Location> loc_white = board.getLocations(Figure::WHITE);
    int value = loc_white.size()-loc_black.size();

    foreach (Location loc, loc_white) {
	value += loc.getY()+1;
	value += board(loc) == Figure::WHITE_QUEEN ? Figure::QUEEN : Figure::PAWN;
    }

    foreach (Location loc, loc_black) {
	value -= board.getSize() - loc.getY();
	value -= board(loc) == Figure::BLACK_QUEEN ? Figure::QUEEN : Figure::PAWN;
    }

    return value;
}


int Strategy::advanced(Board &board) {
    QList<Location> loc_black = board.getLocations(Figure::BLACK);
    QList<Location> loc_white = board.getLocations(Figure::WHITE);
    int value = (2*loc_white.size()) - (2*loc_black.size());

    foreach (Location loc, loc_white) {
	if (loc.getX() == 0 || loc.getX() == 7) {
	    value += 1;
	}
	value += board(loc) == Figure::WHITE_QUEEN ? 2*Figure::QUEEN : Figure::PAWN;
    }

    foreach (Location loc, loc_black) {
	if (loc.getX() == 0 || loc.getX() == 7) {
	    value -= 1;
	}
	value -= board(loc) == Figure::BLACK_QUEEN ? 2*Figure::QUEEN : Figure::PAWN;
    }

    return value;
}
