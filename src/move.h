#ifndef MOVE_H
#define MOVE_H

#include "location.h"
#include "figure.h"

#include <QList>
#include <QPair>


typedef QPair<Location, Figure::Type> JumpedFigure;

class Move {

    public:
	Move();
	Move(const Move &move);
	~Move();

	Move &setFrom(const Location &location);
	Location &getFrom() const;
	Move &setTo(const Location &location);
	Location &getTo() const;
	Move &setPromoted(bool promoted);
	bool getPromoted() const;
	Move &setFigureType(Figure::Type type_of_figure);
	Figure::Type getFigureType() const;
	QList<JumpedFigure>  &getJumpedFigures() const;

	void addJumpedFigure(int x, int y, Figure::Type type_of_figure);
	void addJumpedFigure(const Location &location, Figure::Type type_of_figure);
	void addJumpedFigures(const QList<JumpedFigure> &figures);
	bool containsJumps() const;
	int price() const;

	// Move is invalid if its m_from X coord is negative number.
	bool isValid() const;

	QString toString() const;
	QString toFiguresString() const;
	QString toShortString() const;

	bool operator==(const Move &rhs) const;
	bool operator!=(const Move &rhs) const;

	static Move getInvalidMove();

    private:
	Location *m_from;
	Location *m_to;
	Figure::Type m_typeOfFigure;
	bool m_promoted;    //!< True if pawn was promoted to queen at the end of this move.
	QList<JumpedFigure> *m_jumpedFigures;

};

#endif // MOVE_H
