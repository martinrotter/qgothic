#include "intelligence.h"
#include "player.h"
#include "board.h"
#include "referee.h"
#include "move.h"
#include "algorithms.h"
#include "strategy.h"

#include <ctime>
#include <cstdlib>

#include <QDebug>


Intelligence::Intelligence() {
}

Move Intelligence::computerMove(Player applicant, Board &board) {
    srand((unsigned int)time(0));

    switch (applicant.getState()) {
	case Player::HUMAN: {
	    switch (GSettings::value(SET_GAME, "strategy-best-move", 0).toInt()) {
		case 1:
		    qDebug() << "advanced strategy";
		    return minimaxMove(applicant, board, 2, Strategy::advanced);
		case 0:
		default:
		    qDebug() << "defauult strategy";
		    return minimaxMove(applicant, board, 2, Strategy::simple);
	    }
	}
	case Player::EASY:
	    return randomMove(applicant, board);
	case Player::MEDIUM:
	    return minimaxMove(applicant, board, 2, Strategy::simple);
	case Player::HARD:
	    // sem dát 2
	    // minimaxMove
	    // alfabetaMove
	    return minimaxMove(applicant, board, 2, Strategy::advanced);
	default:
	    return randomMove(applicant, board);
    }
}

Move Intelligence::randomMove(Player applicant, Board &board) {
    QList<Move> moves = Referee::getMoves(applicant.getColor(), board);
    int size = moves.size();
    if (size == 0) {
	return Move::getInvalidMove();
    }
    else {
	return moves[rand() % size];
    }
}

Move Intelligence::alfabetaMove(Player applicant, Board &board,
				int depth, int (*eval_function)(Board &)) {
    int actual_price;
    int alfa = Strategy::LOSING;
    QList<int> best_indices;
    QList<Move> moves = Referee::getMoves(applicant.getColor(), board);

    // Player can't make any moves.
    if (moves.size() == 0) {
	//Board next(board);
	//actual_price = -Algorithms::alfabeta(applicant.getColor(), Figure::negateColor(applicant.getColor()),
	//next, depth-1, eval_function, -);
	qDebug() << "NO MOVES AVAILABLE FOR THIS PLAYER" << "\a";
	return Move::getInvalidMove();
    }

    for (int i = 0; i < moves.size(); i++) {
	Board next(board);
	next.makeMove(moves[i]);
	actual_price = -Algorithms::alfabeta(applicant.getColor(), Figure::negateColor(applicant.getColor()),
					     next, depth-1, eval_function, INT_MIN, Algorithms::alfabeta_further(-alfa));
	if (actual_price > alfa) {
	    alfa = actual_price;
	    best_indices.clear();
	    best_indices.append(i);
	}
	/*else if (actual_price == alfa) {
	    best_indices.append(i);
	}*/
    }
    return moves[best_indices[rand() % best_indices.size()]];
}

Move Intelligence::minimaxMove(Player applicant, Board &board,
			       int depth, int (*eval_function)(Board &)) {
    int actual_price, best_price = Strategy::LOSING;
    QList<int> best_indices;
    QList<Move> moves = Referee::getMoves(applicant.getColor(), board);

    // Player can't make any moves.
    if (moves.size() == 0) {
	Board next(board);
	actual_price = -Algorithms::minimax(applicant.getColor(), Figure::negateColor(applicant.getColor()),
					    next, depth-1, eval_function);
	qDebug() << "NO MOVES AVAILABLE FOR THIS PLAYER" << "\a";
	return Move::getInvalidMove();
    }

    qDebug() << "minimax tahy: " << moves.size();
    for (int i = 0; i < moves.size(); i++) {
	Board next(board);
	next.makeMove(moves[i]);
	actual_price = -Algorithms::minimax(applicant.getColor(), Figure::negateColor(applicant.getColor()),
					    next, depth-1, eval_function);
	if (actual_price > best_price) {
	    best_price = actual_price;
	    best_indices.clear();
	    best_indices.append(i);
	}
	else if (actual_price == best_price) {
	    best_indices.append(i);
	}
    }
    return moves[best_indices[rand() % best_indices.size()]];
}
