#ifndef HISTORYITEM_H
#define HISTORYITEM_H

#include "move.h"


/*!
 * \brief Item of History representation.
 * \ingroup Core
 */
class HistoryItem {

    public:
	/*!
	 * \brief Container for all available information to be displayed from HistoryItem.
	 *
	 * \note This is used eg. in View, which are obtaining data from HistoryModel.
	 */
	enum Section {
	    ID	    = 0,    //!< ID is displayed.
	    PLAYER  = 1,    //!< Information about current Player is displayed.
	    MOVE    = 2	    //!< Information about current Move is displayed.
	};

	HistoryItem(Move m_move, int id, int moves);
	~HistoryItem();

	/*!
	 * \brief Move getter.
	 * \return Method returns Move made somewhere in History.
	 */
	Move *getMove() const;

	/*!
	 * \brief Moves without jump getter.
	 */
	int getMovesWithoutJump() const;

	/*!
	 * \brief Ordinal number getter.
	 */
	int getOrdinalNumber() const;

	/*!
	 * \brief Top-level access method for getting data about this HistoryItem of History.
	 * \param section Specifies, which data should be displayed by View.
	 * \return Method returns data for displaying.
	 */
	QString getInfo(HistoryItem::Section section) const;

    private:
	Move *m_move;
	int m_movesWithoutJump;
	int m_ordinalNumber;

};

#endif // HISTORYITEM_H
