#include "location.h"
#include "board.h"

#include <QString>


Location::Location() : m_x(0), m_y(0) {
}

Location::Location(const Location &location) : m_x(location.getX()), m_y(location.getY()) {
}

Location::Location(int x, int y) : m_x(x), m_y(y) {
}

Location Location::operator+(const Location &rhs) const {
    return Location(getX() + rhs.getX(),
		    getY() + rhs.getY());
}

bool Location::operator==(const Location &rhs) const {
    return getX() == rhs.getX() && getY() == rhs.getY();
}

bool Location::operator!=(const Location &rhs) const {
    return !(*this == rhs);
}

Location &Location::operator+=(const Location &rhs) {
    m_x += rhs.getX();
    m_y += rhs.getY();
    return *this;
}

Location Location::operator!() const {
    return Location(getX(), -getY());
}

bool Location::isOnBoard(const Board &board) const {
    return m_x < board.getSize() && m_x >= 0 && m_y < board.getSize() && m_y >= 0;
}

bool Location::isFree(const Board &board) const {
    return board(*this) == Figure::EMPTY;
}

bool Location::isInvalid() const {
    return m_x == -1 && m_y == -1;
}

QString Location::toString() const {
    return QString("%1%2").arg(QChar(getX() + 'A')).arg(getY() + 1);
}

Location &Location::setX(int x) {
    m_x = x;
    return *this;
}

Location &Location::setY(int y) {
    m_y = y;
    return *this;
}

int Location::getX() const {
    return m_x;
}

int Location::getY() const {
    return m_y;
}
