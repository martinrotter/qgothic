#ifndef FIGURE_H
#define FIGURE_H

#include <QList>


/*!
 * \brief Onboard Figure representation.
 * \ingroup Core
 */
class Figure {

    public:

	/*!
	 * \brief Color of figures
	 */
	enum Color {
	    WHITE = 0,
	    BLACK = 1,
	    UNKNOWN = 3
	};

	/*!
	 * \brief Type of figures
	 */
	enum Type {
	    WHITE_PAWN	= -10,
	    WHITE_QUEEN	= -20,
	    EMPTY	= 100,
	    BLACK_PAWN	= 10,
	    BLACK_QUEEN	= 20
	};

	/*!
	 * \brief Prices of figures.
	 */
	enum Price {
	    PAWN = 1,
	    QUEEN = 4
	};

	static QString toString(Figure::Type figure);
	static QList<Figure::Type> getTypeByColor(const Figure::Color &color);
	static Figure::Color getColorByType(const Figure::Type &type);
	static Figure::Color negateColor(const Figure::Color &color);
	static Figure::Type promote(const Figure::Type &type);
	static Figure::Type degrade(const Figure::Type &type);

    private:
	Figure();

};

#endif // FIGURE_H
