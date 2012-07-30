#ifndef INTELLIGENCE_H
#define INTELLIGENCE_H


class Move;
class Player;
class Board;

class Intelligence {

    public:
	Intelligence();

	static Move computerMove(Player applicant, Board &board);
	static Move randomMove(Player applicant, Board &board);
	static Move minimaxMove(Player applicant, Board &board,
				int depth, int (*eval_function)(Board &));

};

#endif // INTELLIGENCE_H
