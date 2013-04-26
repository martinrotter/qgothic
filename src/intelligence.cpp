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


Intelligence::Intelligence(QObject *parent) : QObject(parent) {
}

void Intelligence::computerMove(Player applicant, Board &board) {
    srand((unsigned int)time(0));

    switch (applicant.getState()) {
	case Player::HUMAN: {
	    switch (GSettings::value(SET_GAME, "strategy-best-move", 0).toInt()) {
		case 1:
		    qDebug() << "Advising move with advanced strategy.";
		    minimaxMove(applicant, board, 2, Strategy::advanced);
		    break;
		case 0:
		default:
		    qDebug() << "Advising move with default strategy.";
		    minimaxMove(applicant, board, 2, Strategy::simple);
		    break;
	    }
	    break;
	}
	case Player::EASY:
	    randomMove(applicant, board);
	    break;
	case Player::MEDIUM:
	    // alfabetaMove(applicant, board, 6, Strategy::advanced);
	    //alfabetaMove(applicant, board, 6, Strategy::simple);
	    minimaxMove(applicant, board, 4, Strategy::simple);
	    break;
	case Player::HARD:
	    // sem dát 2
	    // minimaxMove
	    // alfabetaMove
	    alfabetaMove(applicant, board, 6, Strategy::advanced);
	    //minimaxMove(applicant, board, 4, Strategy::advanced);
	    break;
	default:
	    randomMove(applicant, board);
	    break;
    }
}

void Intelligence::randomMove(Player applicant, Board &board) {
    QList<Move> moves = Referee::getMoves(applicant.getColor(), board);
    int size = moves.size();
    if (size == 0) {
	emit moveFound(Move::getInvalidMove());
    }
    else {
	emit moveFound(moves[rand() % size]);
    }
}

void Intelligence::alfabetaMove(Player applicant, Board &board,
				int depth, int (*eval_function)(Board &)) {
    int actual_price;
    int alfa = Strategy::LOSING;
    QList<int> best_indices;
    QList<Move> moves = Referee::getMoves(applicant.getColor(), board);

    // Player can't make any moves.
    if (moves.size() == 0) {
	qDebug() << "NO MOVES AVAILABLE FOR THIS PLAYER" << "\a";
	emit moveFound(Move::getInvalidMove());
	return;
    }

    qDebug("Moves for alfabeta: %d", moves.size());
    emit countOfCalls(moves.size());

    for (int i = 0; i < moves.size(); i++) {
	if (Intelligence::isCancelling() == true) {
	    qDebug() << "Move search cancelled.";
	    Intelligence::cancel(false);
	    emit cancelled();
	    return;
	}

	Board next(board);
	next.makeMove(moves[i]);
	actual_price = -Algorithms::alfabeta(applicant.getColor(), Figure::negateColor(applicant.getColor()),
					     next, depth-1, eval_function, INT_MIN, Algorithms::alfabeta_further(-alfa));
	if (actual_price > alfa) {
	    alfa = actual_price;
	    best_indices.clear();
	    best_indices.append(i);
	}

	emit rankOfCall(i+1);
    }

    if (applicant.getState() == Player::HUMAN) {
	qDebug("Returning advised move for human player.");
	emit moveForHumanFound(moves[best_indices[rand() % best_indices.size()]]);
    }
    else {
	emit moveFound(moves[best_indices[rand() % best_indices.size()]]);
    }
}

void Intelligence::minimaxMove(Player applicant, Board &board,
			       int depth, int (*eval_function)(Board &)) {
    int actual_price, best_price = Strategy::LOSING;
    QList<int> best_indices;
    QList<Move> moves = Referee::getMoves(applicant.getColor(), board);

    // Player can't make any moves.
    if (moves.size() == 0) {
	qDebug() << "NO MOVES AVAILABLE FOR THIS PLAYER" << "\a";
	Move invalid_move = Move::getInvalidMove(Figure::getTypeByColor(applicant.getColor()).at(0));
	emit moveFound(invalid_move);
	return;
    }

    qDebug("Moves for minimax: %d", moves.size());
    emit countOfCalls(moves.size());

    for (int i = 0; i < moves.size(); i++) {
	if (Intelligence::isCancelling() == true) {
	    qDebug() << "Move search cancelled.";
	    Intelligence::cancel(false);
	    emit cancelled();
	    return;
	}

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

	emit rankOfCall(i+1);
    }

    if (applicant.getState() == Player::HUMAN) {
	qDebug("Returning advised move for human player.");
	emit moveForHumanFound(moves[best_indices[rand() % best_indices.size()]]);
    }
    else {
	emit moveFound(moves[best_indices[rand() % best_indices.size()]]);
    }
}
