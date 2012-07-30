#ifndef LOCATION_H
#define LOCATION_H


class QString;
class Board;

/*!
 * \brief Location on the playing board.
 *
 * Location class represents position on the playing board.
 */
class Location {

    public:
	Location();
	Location(const Location &location);
	Location(int x, int y);

	Location &setX(int x);
	int getX() const;
	Location &setY(int y);
	int getY() const;

	bool isOnBoard(const Board &board) const;
	bool isFree(const Board &board) const;
	bool isInvalid() const;

	QString toString() const;

	Location operator+(const Location &rhs) const;
	Location &operator+=(const Location &rhs);
	bool operator==(const Location &rhs) const;
	bool operator!=(const Location &rhs) const;
	Location operator!() const;

    private:
	int m_x;
	int m_y;

};

#endif // LOCATION_H
