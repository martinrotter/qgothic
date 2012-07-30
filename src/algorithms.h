#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "figure.h"
#include "board.h"


class Algorithms {

    public:
	Algorithms();

	static int minimax(Figure::Color original_color, Figure::Color actual_color,
			   Board &board, int depth, int (*eval_function)(Board &));

};

#endif // ALGORITHMS_H
