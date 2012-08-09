#ifndef INTELLIGENCE_H
#define INTELLIGENCE_H


class Move;
class Player;
class Board;

/*!
 * \brief AI best Move selectors.
 */
class Intelligence {

    public:
	/*!
	 * \brief Top-level method for finding best move according to CPU player difficulty.
	 * \param applicant Player, that wants to find a Move.
	 * \param board Board to seek the Move on.
	 * \return Method returns best available Move for this Player.
	 */
	static Move computerMove(Player applicant, Board &board);

	/*!
	 * \brief Method for finding random move.
	 * \param applicant Player, that wants to find a Move.
	 * \param board Board to seek the Move on.
	 * \return Method returns random available Move for this Player.
	 */
	static Move randomMove(Player applicant, Board &board);

	/*!
	 * \brief Method for finding best available Move using traditional Minimax algorithm.
	 * \param applicant Player, that wants to find a Move.
	 * \param board Board to seek the Move on.
	 * \return Method returns best available Move for this Player.
	 */
	static Move minimaxMove(Player applicant, Board &board,
				int depth, int (*eval_function)(Board &));

	static Move alfabetaMove(Player applicant, Board &board,
				 int depth, int (*eval_function)(Board &));

    private:
	Intelligence();

};

#endif // INTELLIGENCE_H
