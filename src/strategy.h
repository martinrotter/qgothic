#ifndef STRATEGY_H
#define STRATEGY_H


class Board;

class Strategy {

    public:
	enum State {
	    WINNING = 200,
	    ALMOST_WINNING = 191,
	    LOSING = -200,
	    ALMOST_LOSING = -191,
	    DRAW = 0
	};

	Strategy();

	static int simple(Board &board);

};

#endif // STRATEGY_H
