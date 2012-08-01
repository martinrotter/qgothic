#ifndef PLAYER_H
#define PLAYER_H

#include "figure.h"


class QString;

/*!
 * \brief Player representation.
 *
 * Player class represents each player of QGothic board game. Player can be human or cpu-controlled
 * entity.
 */
class Player {

    public:
	/*!
	 * \brief Possible kinds of player.
	 *
	 * PlayerState enum defines all possible kinds of player able to play QGothic board game.
	 */
	enum State {
	    HUMAN = 0,	//!< Human-controlled player.
	    EASY,	//!< Player with EASY difficulty.
	    MEDIUM,	//!< Player with MEDIUM difficulty.
	    HARD	//!< Player with HARD difficulty.
	};

	/*!
	 * \brief Player class constructor.
	 * \param state State of newly constructed Player object.
	 * \param color Color of newly constructed Player object.
	 */
	Player(Player::State state, Figure::Color color);

	Player();

	void setColor(Figure::Color color);
	Figure::Color getColor() const;
	void setState(Player::State state);
	Player::State getState() const;

	/*!
	 * \brief Textual representation of Player object.
	 * \return Method returns textual representation of Player object.
	 */
	QString toString() const;

	/*!
	 * \brief Operator of equality for Player objects.
	 * \param rhs Player object to compare with *this.
	 * \return Operator returns true if both Player objects have same color & state.
	 */
	bool operator==(const Player &rhs) const;

	/*!
	 * \brief Operator of inequality for Player objects.
	 * \param rhs Player object to compare with *this.
	 * \return Operator returns true if Player objects don't have same either color or state.
	 */
	bool operator!=(const Player &rhs) const;

    private:
	Figure::Color m_color;
	Player::State m_state;

};

#endif // PLAYER_H
