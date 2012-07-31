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
	case Player::EASY:
	    return randomMove(applicant, board);
	case Player::MEDIUM:
	case Player::HUMAN:
	    return minimaxMove(applicant, board, 2, Strategy::simple);
	case Player::HARD:
	    return minimaxMove(applicant, board, 3, Strategy::simple);
	default:
	    return randomMove(applicant, board);
    }
}

Move Intelligence::randomMove(Player applicant, Board &board) {
    QList<Move> moves = Referee::getMoves(applicant.getColor(), board);
    return moves[rand() % moves.size()];
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
	qDebug() << "NO MOVES AVAILABLE";
    }

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
