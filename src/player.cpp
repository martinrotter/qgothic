#include "player.h"

#include <QString>


Player::Player(State state, Figure::Color color) : m_color(color), m_state(state) {
}

Player::Player() : m_color(Figure::WHITE), m_state(Player::EASY) {
}

Player &Player::setColor(Figure::Color color) {
    m_color = color;
    return *this;
}

Figure::Color Player::getColor() const {
    return m_color;
}

Player &Player::setState(Player::State state) {
    m_state = state;
    return *this;
}

Player::State Player::getState() const {
    return m_state;
}

QString Player::toString() const {
    return QString("State: %1\n"
		   "Color: %2").arg(m_state).arg(m_color);
}

bool Player::operator==(const Player &rhs) const {
    return m_color == rhs.getColor() && m_state == rhs.getState();
}

bool Player::operator!=(const Player &rhs) const {
    return !(*this == rhs);
}
