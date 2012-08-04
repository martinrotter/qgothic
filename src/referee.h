#ifndef REFEREE_H
#define REFEREE_H

#include "location.h"
#include "move.h"

#include <QList>


/*!
 * \brief Moves/jumps generator.
 *
 * This class has one top-level method called getMoves which should be used primarily for obtaining
 * available moves for Figure::Color in argument.
 */
class Referee {

    public:
	Referee();

	/*!
	 * \brief Directions for pawn move.
	 * \return Method returns list with all posibble directions for pawn move.
	 */
	const static QList<Location> &getPawnDirections();


	/*!
	 * \brief Directions for queen move.
	 * \return Method returns list with all posibble directions for queen move.
	 */
	const static QList<Location> &getQueenDirections();

	/*!
	 * \brief Finds all posibble simple moves (without jumps) for queen.
	 * \param location Position of queen on the board.
	 * \param board Board to search the moves on.
	 * \return Method returns list of found moves or empty list if no moves are available.
	 */
	static QList<Move> getQueenMoves(const Location &location, Board &board);

	/*!
	 * \brief Finds all posibble simple moves (without jumps) for pawn.
	 * \param location Position of pawn on the board.
	 * \param board Board to search the moves on.
	 * \return Method returns list of found moves or empty list if no moves are available.
	 */
	static QList<Move> getPawnMoves(const Location &location, Board &board);

	/*!
	 * \brief Finds all posibble jumps for queen.
	 * \param location Position of queen on the board.
	 * \param board Board to search the moves on.
	 * \return Method returns list of found jumps or empty list if no jumps are available.
	 */
	static QList<Move> getQueenJumps(const Location &location, Board &board);

	/*!
	 * \brief Finds all posibble jumps for pawn.
	 * \param location Position of pawn on the board.
	 * \param board Board to search the moves on.
	 * \return Method returns list of found jumps or empty list if no jumps are available.
	 */
	static QList<Move> getPawnJumps(const Location &location, Board &board);

	/*!
	 * \brief Best moves selector.
	 *
	 * Method selects longest available moves from input list. Longest move means move with
	 * the highest count of jumped figures. If more moves have the same highest count of jumps,
	 * then all figures are included in result.
	 * \param moves Moves to filter.
	 * \return Method returns filtered list of longest moves.
	 */
	static QList<Move> getBestMoves(const QList<Move> &moves);

	/*!
	 * \brief Finds both jumps/moves for queen.
	 * \param location Position of queen on the board.
	 * \param board Board to search the moves on.
	 * \return Method returns found jumps/moves.
	 */
	static QList<Move> getAllQueenMoves(const Location &location, Board &board);

	/*!
	 * \brief Finds both jumps/moves for pawn.
	 * \param location Position of pawn on the board.
	 * \param board Board to search the moves on.
	 * \return Method returns found jumps/moves.
	 */
	static QList<Move> getAllPawnMoves(const Location &location, Board &board);

	/*!
	 * \brief Finds jumps/moves for any figure on the specified location.
	 * \param location Location of certain figure on the board.
	 * \param location Location of pawn on the board.
	 * \param board Board to search the moves on.
	 * \return Method returns all found moves (including jumps of course) for selected figure
	 * specified by its location on the board.
	 */
	static QList<Move> getAllMoves(const Location &location, Board &board);

	/*!
	 * \brief Gets all available moves for certain Figure::Color.
	 *
	 * This method finds all available moves for certain Figure::Color in according to
	 * game rules. This method should be used by every CPU player representation.
	 * This is top-level method for use. You should use primarily this method for finding moves.
	 * \param color Color of player that wants to find all available moves.
	 * \param board Board to search the moves on.
	 * \return Method returns list of all found moves for selected Figure::Color.
	 */
	static QList<Move> getMoves(const Figure::Color &color, Board &board);

	/*!
	 * \brief Decides whether certain figure is located in queen-promotion area of the board.
	 * \param location Location of figure to decide about.
	 * \param color_of_figure Color of figure to decide about.
	 * \param board Board where figure is located on.
	 * \return Method returns true if figure is on position, on which it could be
	 * promoted to queen if it is actually a pawn.
	 */
	static bool isInQueenArea(const Location &location,
				  const Figure::Color &color_of_figure,
				  const Board &board);

	/*!
	 * \brief Contains available directions for pawns.
	 */
	static QList<Location> *s_pawnDirections;

	/*!
	 * \brief Contains available directions for queens.
	 */
	static QList<Location> *s_queenDirections;

};

#endif // REFEREE_H
