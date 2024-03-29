#include "player.h"

#include <QString>


Player::Player(State state, Figure::Color color) : m_color(color), m_state(state) {
}

Player::Player() : m_color(Figure::WHITE), m_state(Player::EASY) {
}

void Player::setColor(Figure::Color color) {
    m_color = color;
}

Figure::Color Player::getColor() const {
    return m_color;
}

void Player::setState(Player::State state) {
    m_state = state;
}

Player::State Player::getState() const {
    return m_state;
}

bool Player::operator==(const Player &rhs) const {
    return m_color == rhs.getColor() && m_state == rhs.getState();
}

bool Player::operator!=(const Player &rhs) const {
    return !(*this == rhs);
}
