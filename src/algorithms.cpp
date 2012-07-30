#include "algorithms.h"
#include "strategy.h"
#include "referee.h"

#include <QDebug>


Algorithms::Algorithms() {
}


int Algorithms::minimax(Figure::Color original_color, Figure::Color actual_color,
			Board &board, int depth, int (*eval_function)(Board &)) {

    // na tahu je černý hráč a bilý vyhrál nebo je na tahu bilý hráč a černý vyhrál
    if ((actual_color == Figure::BLACK && board.getState() == Board::WHITE_WON) || (actual_color == Figure::WHITE && board.getState() == Board::BLACK_WON)) {
	return Strategy::LOSING;
    }

    // na tahu je černý hráč a černý vyhrál nebo je na tahu bilý hráč a bilý vyhrál
    if ((actual_color == Figure::BLACK && board.getState() == Board::BLACK_WON) || (actual_color == Figure::WHITE && board.getState() == Board::WHITE_WON)) {
	return Strategy::WINNING;
    }

    if (board.getState() == Board::DRAW) {
	return Strategy::DRAW;
    }

    if (depth == 0) {
	if (actual_color == Figure::WHITE) {
	    return eval_function(board);
	}
	else {
	    return -eval_function(board);
	}
    }

    QList<Move> moves = Referee::getMoves(actual_color, board);
    int price = Strategy::LOSING;

    // Player can't make any moves.
    if (moves.size() == 0) {
	Board next(board);
	price = std::max(price, -minimax(original_color, Figure::negateColor(actual_color),
					 next, depth-1, eval_function));
	qDebug() << "NO MOVES AVAILABLE";
    }

    foreach (Move move, moves) {
	Board next(board);
	next.makeMove(move);
	price = std::max(price, -minimax(original_color, Figure::negateColor(actual_color),
					 next, depth-1, eval_function));
    }

    if (price > Strategy::ALMOST_WINNING) {
	price--;
    }
    if (price < Strategy::ALMOST_LOSING) {
	price++;
    }
    return price;
}
