#ifndef MOVE_H
#define MOVE_H

#include "location.h"
#include "figure.h"

#include <QList>
#include <QPair>


typedef QPair<Location, Figure::Type> JumpedFigure;

/*!
 * \brief Move representation.
 */
class Move {

    public:

	/*!
	 * \brief Default constructor.
	 */
	Move();

	/*!
	 * \brief Copy constructor.
	 * \param move Instance to copy.
	 */
	Move(const Move &move);

	~Move();

	/*!
	 * \brief Sets from Location of this move.
	 * \param location Location to set.
	 */
	void setFrom(const Location &location);

	/*!
	 * \brief Gets from Location of this move.
	 * \return Method returns reference to from Location of this Move instance.
	 */
	Location &getFrom() const;

	/*!
	 * \brief Sets to Location of this move.
	 * \param location Location to set.
	 */
	void setTo(const Location &location);

	/*!
	 * \brief Gets to Location of this move.
	 * \return Method returns reference to to Location of this Move instance.
	 */
	Location &getTo() const;

	/*!
	 * \brief Sets if figure was promoted to the queen at the end if this Move.
	 * \param promoted Indication of promotion. True if pawn was promoted to the queen.
	 */
	void setPromoted(bool promoted);

	/*!
	 * \brief Move promotion indicator.
	 * \return Method returns true if pawn was promoted to queen at the end of Move.
	 */
	bool getPromoted() const;

	/*!
	 * \brief Figure type setter for Move.
	 * \param type_of_figure Indicates which figure made this Move.
	 */
	void setFigureType(Figure::Type type_of_figure);

	/*!
	 * \brief Figure type getter for Move.
	 * \return Method returns figure, which made this Move.
	 */
	Figure::Type getFigureType() const;

	/*!
	 * \brief Jumped figures getter.
	 * \return Method returns list of all jumped figures in this Move.
	 */
	QList<JumpedFigure> &getJumpedFigures() const;

	/*!
	 * \brief Adds newly jumped figure to this Move.
	 * \param x X-coordinate of jumped figure on the Board.
	 * \param y Y-coordinate of jumped figure on the Board.
	 * \param type_of_figure Type of jumped figure.
	 */
	void addJumpedFigure(int x, int y, Figure::Type type_of_figure);

	/*!
	 * \brief Adds newly jumped figure to this Move.
	 * \param location Location of jumped figure.
	 * \param type_of_figure Type of jumped figure.
	 */
	void addJumpedFigure(const Location &location, Figure::Type type_of_figure);

	/*!
	 * \brief Adds jumped figures to the current Move.
	 * \param figures Jumped figures to add.
	 */
	void addJumpedFigures(const QList<JumpedFigure> &figures);

	/*!
	 * \brief Counts price of this Move.
	 *
	 * \note Note that price of Move instance is used eg. in AI.
	 *
	 * \return Method returns price of this Move instance.
	 */
	int price() const;

	// Move is invalid if its m_from X coord is negative number.
	bool isValid() const;

	QString toFiguresString() const;
	QString toString() const;

	bool operator==(const Move &rhs) const;
	bool operator!=(const Move &rhs) const;

	/*!
	 * \brief Universal invalid Move instance generator.
	 * \return Method returns invalid Move.
	 *
	 * \note Invalid moves are used to indicate that current player cannot
	 * make any move on the Board.
	 */
	static Move getInvalidMove();

    private:
	Location *m_from;
	Location *m_to;
	Figure::Type m_typeOfFigure;
	bool m_promoted;    //!< True if pawn was promoted to queen at the end of this move.
	QList<JumpedFigure> *m_jumpedFigures;

};

#endif // MOVE_H
