#ifndef STRATEGY_H
#define STRATEGY_H


class Board;

/*!
 * \brief Artificial intelligence strategy container.
 *
 * This class holds all prearranged strategies. Strategy is defined by board-evaluating function.
 */
class Strategy {

    public:

	/*!
	 * \brief Board rating enumerator.
	 */
	enum State {
	    WINNING = 200,
	    ALMOST_WINNING = 191,
	    LOSING = -200,
	    ALMOST_LOSING = -191,
	    DRAW = 0
	};

	/*!
	 * \brief Very simple board-evaluating functions.
	 *
	 * This functions prefers statical portion of board rating. That makes AI with this strategy
	 * rather defensive.
	 * \param board Board to rate.
	 * \warning Rating just statical part of the board could make this strategy "blind" as sometimes
	 * count of figures is not everything. But it does basic job pretty well.
	 * \return Method returns rating of board from the view of active player. The higher integer the
	 * better position for active player.
	 */
	static int simple(Board &board);

    private:
	Strategy();

};

#endif // STRATEGY_H
